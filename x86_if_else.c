#include <stdbool.h>

#include "x86_if_else.h"
#include "x86_code_block.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

/**
 * @param true_label the label to jump to after if-blocks body have been
 *                   executed
 */
static void
x86_compile_if_block(x86_comp_params_t *params,
                     IrIfBlock *if_block,
                     char *true_label,
                     sym_table_t *sym_table);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

int
x86_if_else_assign_addrs(x86_comp_params_t *params,
                         int first_num,
                         IrIfElse *if_else)
{
    GSList *i;
    IrCodeBlock *else_body;
    int last_num = first_num;

    /* assign numbers to bodies of all if and if-else clauses */
    i = ir_if_else_get_if_else_blocks(if_else);
    for (; i != NULL; i = g_slist_next(i))
    {
        int num = 0;
        IrIfBlock *if_block = i->data;
        num = x86_code_block_assign_addrs(params,
                                          first_num,
                                          ir_if_block_get_body(if_block));
        if (num < last_num)
        {
            last_num = num;
        }
    }

    /* assign numbers to else clause's body */
    else_body = ir_if_else_get_else_body(if_else);
    if (else_body != NULL)
    {
        int num = 0;
        num = x86_code_block_assign_addrs(params, first_num, else_body);
        if (num < last_num)
        {
            last_num = num;
        }
    }

    return last_num;
}


void
x86_compile_if_else(x86_comp_params_t *params,
                    IrIfElse *if_else,
                    sym_table_t *sym_table)
{
    GSList *i;
    char end_label[LABEL_MAX_LEN];
    IrCodeBlock *else_body;

    label_gen_next(&(params->label_gen), end_label);

    i = ir_if_else_get_if_else_blocks(if_else);
    for (; i != NULL; i = g_slist_next(i))
    {
        x86_compile_if_block(params,
                             IR_IF_BLOCK(i->data),
                             end_label,
                             sym_table);
    }

    else_body = ir_if_else_get_else_body(if_else);
    if (else_body != NULL)
    {
        x86_compile_code_block(params,
                               else_body);
    }
    
    fprintf(params->out,
            "# if-else end\n"
            "%s:\n",
            end_label);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
x86_compile_if_block(x86_comp_params_t *params,
                     IrIfBlock *if_block,
                     char *true_label,
                     sym_table_t *sym_table)
{
    char skip_label[LABEL_MAX_LEN];

    label_gen_next(&(params->label_gen), skip_label);

    fprintf(params->out,
            "# eval if-block condition\n");

    x86_compile_expression(params,
                           ir_if_block_get_condition(if_block),
                           sym_table);

    fprintf(params->out,
            "# skip if-block body if condition was evaluated to false\n"
            "    popl %%eax\n"
            "    cmp $1, %%eax\n"
            "    jne %s\n",
            skip_label);

    x86_compile_code_block(params,
                           ir_if_block_get_body(if_block));

    fprintf(params->out,
            "    jmp %s\n"
            "%s:\n",
            true_label,
            skip_label);
}
