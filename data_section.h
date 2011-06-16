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
gen_data_section(FILE *out, IrModule *module);

#endif /* DATA_SECTION_INC_X */
