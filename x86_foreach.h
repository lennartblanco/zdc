#ifndef X86_FOREACH_INC_X
#define X86_FOREACH_INC_X

#include "x86.h"
#include "ir_foreach.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_foreach(x86_comp_params_t *params,
                    IrForeach *foreach,
                    sym_table_t *sym_table);

int
x86_foreach_assign_addrs(x86_comp_params_t *params,
                         int first_offset,
                         IrForeach *foreach);

#endif /* X86_FOREACH_INC_X */
