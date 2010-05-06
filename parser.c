#include <stdlib.h>

#include "parser.h"
#include "lex.h"
#include "entire.h"

#include <assert.h>

static const char *current_source_file;

static bool token_errors;

extern AstModule *module;
extern long yypos;

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static GQuark
parser_error_quark(void);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

AstModule *
parse_file(const char* source_file, GError **error)
{
    YY_BUFFER_STATE yy_buffer;
    FILE *input_stream;
    const char *path_end;

    /* open the input source file */
    input_stream = fopen(source_file, "r");
    if (input_stream == NULL)
    {
        g_set_error(error,
                    PARSER_ERROR,
                    PARSER_FILE_READ_ERROR,
                    "%s", strerror(errno));
        return NULL;
    }

    /* setup token parser to read the opened file */
    current_source_file = source_file;
    yypos = 1;
    token_errors = false;
    yy_buffer = yy_create_buffer(input_stream, YY_BUF_SIZE);
    yy_switch_to_buffer(yy_buffer);

    /* parse the source file */
    if (yyparse() != 0 || token_errors)
    {
        g_set_error(error,
                    PARSER_ERROR,
                    PARSER_SYNTAX_ERROR,
                    "syntax error");
        module = NULL;
        goto exit_parse_file;
    }

    /*
     * store the source file in created ast node
     */
    assert(g_str_has_suffix(source_file, ".d"));

    /* find the end of path */
    path_end = g_strrstr(source_file, "/");
    if (path_end == NULL)
    {
        path_end = source_file;
    }
    else
    {
        path_end += 1;
    }

    ast_module_set_source_file(module, path_end);

exit_parse_file:
    yy_delete_buffer(yy_buffer);
    fclose(input_stream);

    return module;
}

void
yyerror(char *msg, ...)
{
    va_list argp;

    fprintf(stderr, "%s:%ld: ", current_source_file, yypos);

    va_start(argp, msg);
    vfprintf(stderr, msg, argp);
    va_end(argp);

    fprintf(stderr, "\n");
    token_errors = true;
}

int
yywrap(void)
{
   /* stop token parser when EOF is reached */
   return 1;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static GQuark
parser_error_quark(void)
{
    return g_quark_from_static_string ("xdc-parser-error-quark");
}

