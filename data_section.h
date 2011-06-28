#ifndef DATA_SECTION_INC_X
#define DATA_SECTION_INC_X

#include "ir_module.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Generate a .data section defining all compile-time constant expressions.
 */
void
data_section_add_literals(FILE *output, IrModule *ir_module);

#endif /* DATA_SECTION_INC_X */
