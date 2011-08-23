#include <stdio.h>

#include "auxil.h"
#include "parser.h"
#include "sym_table.h"
#include "sem_analyze.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static int
load_module_imports(AstModule *ast_module, GSList *import_paths);

static int
parse_import(AstImport *import, GSList *import_paths, GError **error);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

int
compile_file(const char* input_file, 
             const char* output_file,
             compilation_settings_t settings)
{
    FILE *output_stream;
    AstModule *ast_module;
    GError *error = NULL;
    IrModule *ir_module;

    /*
     * Parse the input source file
     */
    ast_module = parse_file(input_file, &error);
    if (ast_module == NULL)
    {
        /* error while parsing source file */
        if (error->code == PARSER_FILE_READ_ERROR)
        {
            fprintf(stderr, "cannot read source file '%s': %s\n",
                    input_file, error->message);
        }
        g_error_free(error);
        return -1;
    }

    /*
     * Parse module's imports
     */
    if (load_module_imports(ast_module, settings.import_paths) != 0)
    {
        return -1;
    }

    if (settings.print_ast)
    {
        ast_node_print(AST_NODE(ast_module), stdout, 0);
    }

    /*
     * Perform semantic analysis of the code
     */
    ir_module = semantic_analyze(input_file, &(settings.backend), ast_module);
    if (ir_module == NULL)
    {
        /* error during semantic analysis */
        return -2;
    }

    if (settings.print_ir)
    {
        ir_module_print(ir_module, stdout, 0);
    }

    /*
     * Generate target assembly file
     */

    /* open the output file */
    output_stream = fopen(output_file, "w");
    if (output_stream == NULL)
    {
        fprintf(stderr, 
                "error opening file '%s' for writing: %m\n",
                output_file);
        return -1;        
    }

    settings.backend.gen_code(ir_module, output_stream, input_file);

    /* clean up */
    fclose(output_stream);

    return 0;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static int
load_module_imports(AstModule *ast_module, GSList *import_paths)
{
    GSList *i = ast_module_get_imports(ast_module);
    GError *error = NULL;

    for (; i != NULL; i = g_slist_next(i))
    {
        if (parse_import(i->data, import_paths, &error) != 0)
        {
            char *path = ast_import_get_path(i->data);
            switch (error->code)
            {
                case PARSER_FILE_READ_ERROR:
                    fprintf(stderr, "%s:%u: cannot import module '%s': %s\n",
                            ast_module_get_source_file(ast_module),
                            ast_node_get_line_num(i->data),
                            path,
                            error->message);
                    break;
                case PARSER_SYNTAX_ERROR:
                    fprintf(stderr, "%s:%u: failed to import module '%s'\n",
                            ast_module_get_source_file(ast_module),
                            ast_node_get_line_num(i->data),
                            path);
                    break;
                default:
                    /* unexpected error code */
                    assert(false);
            }
            g_free(path);
            g_error_free(error);
            /* failed to import module */
            return -1;
       }
    }

    return 0;
}

static int
parse_import(AstImport *import, GSList *import_paths, GError **error)
{
    GSList *i;
    AstModule *module;
    GError *loc_err = NULL;
    int ret = 0;

    GString *file_path = g_string_new(NULL);
    char *rel_path = ast_import_get_path(import);

    /* look for the module among specified import paths */
    for (i = import_paths; i != NULL; i = g_slist_next(i))
    {
        g_string_append(file_path, i->data);
        g_string_append(file_path, rel_path);

        if (g_file_test(file_path->str, G_FILE_TEST_EXISTS))
        {
            /* found the file */
            break;
        }

        g_string_erase(file_path, 0, -1);
    }

    if (file_path->len == 0)
    {
        /* the file to import not found */
        g_set_error(error,
                    PARSER_ERROR,
                    PARSER_FILE_READ_ERROR,
                    "File not found");
        ret = -1;
        goto exit_parse_import;
    }

    /* load and parse the imported module file */
    module = parse_file(file_path->str, &loc_err);
    if (module == NULL)
    {
        g_propagate_error(error, loc_err);
        ret = -1;
        goto exit_parse_import;
    }
    ast_import_set_module(import, module);

    i = ast_module_get_imports(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        AstImport *imp = AST_IMPORT(i->data);
        if (ast_import_is_private(imp))
        {
            /* skip private imports */
            continue;
        }

        if (parse_import(imp, import_paths, &loc_err) != 0)
        {
            /* error handling not implemented */
            assert(false);
        }
    }

exit_parse_import:
    g_string_free(file_path, true);
    g_free(rel_path);
    return ret;
}

