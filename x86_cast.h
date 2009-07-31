#ifndef X86_CAST_INC_X
#define X86_CAST_INC_X

#include "ir_cast.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_cast(x86_comp_params_t *params,
                 IrCast *cast,
                 sym_table_t *sym_table);

#endif /* X86_CAST_INC_X */
