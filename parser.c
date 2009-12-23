#include <stdlib.h>

#include "parser.h"
#include "lex.h"
#include "entire.h"

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

