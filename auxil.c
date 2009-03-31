#include <stdlib.h>
#include <stdio.h>

#include "auxil.h"
#include "lex.h"
#include "entire.h"
#include "ast_compile_unit.h"
#include "java_trgt.h"
#include "x86.h"
#include "sym_table.h"
#include "sem_analyze.h"

#include <assert.h>

extern AstCompileUnit *compile_unit;

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

int
compile_file(const char* input_file, 
             const char* output_file,
             compile_options_t options)
{
    YY_BUFFER_STATE yy_buffer;
    FILE *input_stream;
    FILE *output_stream;
    IrCompileUnit *ir_compile_unit;

    /* open the input source file */
    input_stream = fopen(input_file, "r");
    if (input_stream == NULL)
    {
        fprintf(stderr, 
                "error opening file '%s' for reading: %m\n",
                input_file);
        return -1;        
    }

    /* setup token parser to read the opened file */
    yy_buffer = yy_create_buffer(input_stream, YY_BUF_SIZE);
    yy_switch_to_buffer(yy_buffer);

    /* open the output file */
    output_stream = fopen(output_file, "w");
    if (output_stream == NULL)
    {
        fprintf(stderr, 
                "error opening file '%s' for writing: %m\n",
                output_file);
        fclose(input_stream);
        return -1;        
    }
   
    /* parse the source file */
    yyparse();
    if (options.print_ast)
    {
        ast_node_print(XDP_AST_NODE(compile_unit), stdout);
    }

    ir_compile_unit = semantic_analyze(input_file, compile_unit);
    if (ir_compile_unit == NULL)
    {
        /* error during semantic analysis */
        goto clean_and_exit;  
    }

    if (options.print_ir)
    {
        ir_compile_unit_print(ir_compile_unit, stdout, 0);
    }

    switch (options.target_arch)
    {
        case arch_java:
            /* use the output file name as the basis for class name */
            java_trgt_gen_code(ir_compile_unit, output_stream, input_file);
            break;
        case arch_x86:
            x86_gen_code(ir_compile_unit, output_stream, input_file);
            break;
        default:
            /* unexpected target architecture */
            assert(false);
    }


clean_and_exit:
   /* clean up */
   fclose(output_stream);
   yy_delete_buffer(yy_buffer);
   fclose(input_stream);
   

   return 0;
}

void
yyerror(char *msg)
{
   extern long yypos;

   printf("line %ld: %s\n", yypos, msg);
   exit(1);
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


