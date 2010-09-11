#ifndef AUXIL_INC_X
#define AUXIL_INC_X

#include <stdbool.h>
#include <glib.h>

#include "ir_module.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef void (*get_registers_func_t) (GSList **scratch, GSList **preserved);

typedef void (*gen_code_func_t) (IrModule *module,
                                 FILE *out_stream,
                                 const char *source_file);
/**
 * Contains pointers to back-end hooks needed at
 * different stages of compilation.
 */
typedef struct arch_backend_e
{
    get_registers_func_t get_registers;
    gen_code_func_t gen_code;
} arch_backend_t;

typedef struct compile_options_s
{
    arch_backend_t backend;
    bool print_ast;
    bool print_ir;
} compile_options_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Compile the D source file and write generated target assembly 
 * to provided output file.
 *
 * @param input_file the file name of the D source file to compile
 * @param output_file the file name where to write generated java assmebly
 * @param options requested compilation options
 *
 * @return 0 if the file was succcessfully compile, -1 on errors
 */
int
compile_file(const char* input_file, 
             const char* output_file,
             compile_options_t options);

#endif /* AUXIL_INC_X */
