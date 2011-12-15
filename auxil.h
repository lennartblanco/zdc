#ifndef AUXIL_INC_X
#define AUXIL_INC_X

#include <stdbool.h>
#include <glib.h>

#include "ir_module.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef void (*get_registers_func_t) (GSList **scratch, GSList **preserved);

typedef void (*assign_var_locations_func_t) (iml_function_t *frame);

typedef void (*gen_code_func_t) (IrModule *module,
                                 FILE *out_stream,
                                 const char *source_file);
/**
 * Contains pointers to back-end hooks needed at
 * different stages of compilation.
 */
typedef struct arch_backend_e
{
    /** hook to fetch the list of available registers */
    get_registers_func_t get_registers;
    /**
     * hook which is called after register allocation for each frame
     * is finished. allows the backend to assign registers/offsets for
     * parameter variables and variables that did not get register allocations
     */
    assign_var_locations_func_t assign_var_locations;
    /** function invoked to generate assembly code for a module */
    gen_code_func_t gen_code;
} arch_backend_t;

typedef struct compilation_settings_s
{
    GSList *import_paths;
    arch_backend_t backend;
    bool print_ast;
    bool print_ir;
    bool print_iml;
} compilation_settings_t;

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
             compilation_settings_t settings);

#endif /* AUXIL_INC_X */
