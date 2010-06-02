#include <stdio.h>

#include "auxil.h"
#include "parser.h"
#include "java_trgt.h"
#include "x86.h"
#include "sym_table.h"
#include "sem_analyze.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static int
parse_import(AstImport *import, GError **error);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

int
compile_file(const char* input_file, 
             const char* output_file,
             compile_options_t options)
{
    FILE *output_stream;
    AstModule *ast_module;
    GError *error = NULL;
    IrModule *ir_module;
    GSList *i;

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
    i = ast_module_get_imports(ast_module);
    for (; i != NULL; i = g_slist_next(i))
    {
        if (parse_import(i->data, &error) != 0)
        {
            char *path = ast_import_get_path(i->data);
            switch (error->code)
            {
                case PARSER_FILE_READ_ERROR:
                    fprintf(stderr, "%s:%u: cannot import module '%s': %s\n",
                            input_file,
                            ast_node_get_line_num(i->data),
                            path,
                            error->message);
                    break;
                case PARSER_SYNTAX_ERROR:
                    fprintf(stderr, "%s:%u: failed to import module '%s'\n",
                            input_file, ast_node_get_line_num(i->data), path);
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
    if (options.print_ast)
    {
        ast_node_print(AST_NODE(ast_module), stdout, 0);
    }

    /*
     * Perform semantic analysis of the code
     */
    ir_module = semantic_analyze(input_file, ast_module);
    if (ir_module == NULL)
    {
        /* error during semantic analysis */
        return -2;
    }

    if (options.print_ir)
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

    switch (options.target_arch)
    {
//        case arch_java:
//            /* use the output file name as the basis for class name */
//            java_trgt_gen_code(ir_compile_unit, output_stream, input_file);
//            break;
        case arch_x86:
            x86_gen_code(ir_module, output_stream, input_file);
            break;
        default:
            /* unexpected target architecture */
            assert(false);
    }


   /* clean up */
   fclose(output_stream);

   return 0;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static int
parse_import(AstImport *import, GError **error)
{
   char *path;
   AstModule *module;
   GError *loc_err = NULL;
   int ret = 0;

   path = ast_import_get_path(import);
   module = parse_file(path, &loc_err);
   if (module == NULL)
   {
       g_propagate_error(error, loc_err);
       ret = -1;
       goto exit_parse_import;
   }
   ast_import_set_module(import, module);

exit_parse_import:
   g_free(path);
   return ret;
}

