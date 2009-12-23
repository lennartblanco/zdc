#include <stdio.h>

#include "auxil.h"
#include "parser.h"
#include "java_trgt.h"
#include "x86.h"
#include "sym_table.h"
#include "sem_analyze.h"

#include <assert.h>

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
    IrModule *ir_module;

    /*
     * Parse the input source file
     */

    ast_module = parse_file(input_file);
    if (ast_module == NULL)
    {
        /* error while parsing source file */
        return -1;
    }

    if (options.print_ast)
    {
        ast_node_print(AST_NODE(ast_module), stdout);
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


