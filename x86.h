#include "ir_compile_unit.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

#ifndef X86_INC_X
#define X86_INC_X

/**
 * generate x86 assembly from IR
 */
void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream,
             const char *source_file);

#endif /* X86_INC_X */
