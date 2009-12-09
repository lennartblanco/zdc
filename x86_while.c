#include "x86.h"
#include "x86_code_block.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_while(x86_comp_params_t *params,
                  IrWhile *while_statment,
                  sym_table_t *sym_table)
{
    char start_label[LABEL_MAX_LEN];
    char end_label[LABEL_MAX_LEN];

    label_gen_next(&(params->label_gen), start_label);
    label_gen_next(&(params->label_gen), end_label);

    fprintf(params->out,
            "# while loop\n"
            "%s: # loop start\n"
            "# eval loop condition\n",
            start_label);

    x86_compile_expression(params,
                          ir_while_get_loop_condition(while_statment),
                          sym_table);

    fprintf(params->out,
            "# exit loop if loop condition is false\n"
            "    popl %%eax\n"
            "    cmp $1, %%eax\n"
            "    jne %s\n"
            "# loop body\n",
            end_label);

    x86_compile_code_block(params, ir_while_get_body(while_statment));

    fprintf(params->out,
            "    jmp %s\n"
            "%s: # loop end\n",
            start_label,
            end_label);
}

int
x86_while_assign_addrs(x86_comp_params_t *params,
                       int first_num,
                       IrWhile *while_statment)
{
    return x86_code_block_assign_addrs(params,
                                       first_num,
                                       ir_while_get_body(while_statment));
}



