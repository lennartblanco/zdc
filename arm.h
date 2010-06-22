#include "ir_module.h"

#ifndef ARM_INC_X
#define ARM_INC_X

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * generate ARM assembly from IR
 */
void
arm_gen_code(IrModule *module,
             FILE *out_stream,
             const char *source_file);


#endif /* ARM_INC_X */
