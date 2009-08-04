#ifndef X86_IF_ELSE_INC_X
#define X86_IF_ELSE_INC_X

#include "x86.h"
#include "ir_if_else.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_if_else(x86_comp_params_t *params,
                    IrIfElse *if_else,
                    sym_table_t *sym_table);

int
x86_if_else_assign_addrs(x86_comp_params_t *params,
                         int first_num,
                         IrIfElse *if_else);

#endif /* X86_IF_ELSE_INC_X */
