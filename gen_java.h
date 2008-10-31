#include "ir.h"


#ifndef GEN_JAVA_INC_X
#define GEN_JAVA_INC_X

/**
 * Generate the java assembly and write it to the provided stream.
 *
 * @param comp_unit the IR representation of the compile unit to compile
 * @param out_stream the stream where to write generated stream
 * @param klass_name the class name where to place the generate code
 */
void 
gen_java_code(ir_compile_unit_t *comp_unit,
              FILE *out_stream, 
              const char *klass_name);

#endif
