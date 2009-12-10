#include "ir_module.h"

#ifndef JAVA_TRGT_INC_X
#define JAVA_TRGT_INC_X

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Generate the java assembly and write it to the provided stream.
 *
 * @param comp_unit the IR representation of the compile unit to compile
 * @param out_stream the stream where to write generated stream
 * @param source_file the source file name
 */
void 
java_trgt_gen_code(IrModule *comp_unit,
                   FILE *out_stream, 
                   const char *source_file);

#endif /* JAVA_TRGT_INC_X */
