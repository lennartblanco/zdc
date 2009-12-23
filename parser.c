#include <stdlib.h>

#include "parser.h"
#include "lex.h"
#include "entire.h"

#include <assert.h>

static const char *current_source_file;

extern AstModule *module;
extern long yypos;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

AstModule *
parse_file(const char* source_file)
{
    YY_BUFFER_STATE yy_buffer;
    FILE *input_stream;
    const char *path_end;

    /* open the input source file */
    input_stream = fopen(source_file, "r");
    if (input_stream == NULL)
    {
        fprintf(stderr, 
                "error opening file '%s' for reading: %m\n",
                source_file);
        return NULL;
    }

    /* setup token parser to read the opened file */
    current_source_file = source_file;
    yypos = 1;
    yy_buffer = yy_create_buffer(input_stream, YY_BUF_SIZE);
    yy_switch_to_buffer(yy_buffer);
 
    /* parse the source file */
    yyparse();
    yy_delete_buffer(yy_buffer);
    fclose(input_stream);

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

    return module;
}

void
yyerror(char *msg)
{
   fprintf(stderr, "%s:%ld: %s\n", current_source_file, yypos, msg);
   exit(1);
}

int
yywrap(void)
{
   /* stop token parser when EOF is reached */
   return 1;
}

