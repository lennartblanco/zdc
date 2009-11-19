#ifndef X86_WHILE_INC_X
#define X86_WHILE_INC_X

#include "x86.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_while(x86_comp_params_t *params,
                  IrWhile *while_statment,
                  sym_table_t *sym_table);

#endif /* X86_WHILE_INC_X */
