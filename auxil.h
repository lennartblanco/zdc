#ifndef AUXIL_INC_X
#define AUXIL_INC_X

#include <stdbool.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef enum arch_types_e
{
   arch_java = 0,
   arch_x86  = 1
} arch_types_t;

typedef struct compile_options_s
{
    arch_types_t target_arch;
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

void
yyerror(char *msg);

int
yywrap(void);

#endif /* AUXIL_INC_X */
