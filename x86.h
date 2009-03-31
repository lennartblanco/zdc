#include "ir_compile_unit.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * generate x86 assembly from IR
 */
void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream);
