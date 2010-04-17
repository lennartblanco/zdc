#ifndef X86_DATA_INC_X
#define X86_DATA_INC_X

#include "x86.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Generate a .data section defining all compile-time constant expressions.
 *
 * @param data_section_exprs a list of IrExpression object to place into
 *                           .data section.
 */
void
x86_gen_data_section(x86_comp_params_t *params,
                     GSList *data_section_exprs);

#endif /* X86_DATA_INC_X */
