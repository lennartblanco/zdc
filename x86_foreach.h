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

#endif /* X86_FOREACH_INC_X */
