#include "label_gen.h"
#include "ir_compile_unit.h"

#ifndef X86_INC_X
#define X86_INC_X

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef struct x86_comp_params_s
{
    FILE *out;
    label_gen_t label_gen;
} x86_comp_params_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * generate x86 assembly from IR
 */
void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream,
             const char *source_file);

void
x86_compile_expression(x86_comp_params_t *params,
                       IrExpression *expression,
                       sym_table_t *sym_table);

void
x86_compile_code_block(x86_comp_params_t *params,
                       IrCodeBlock *code_block,
                       char *return_label);


#endif /* X86_INC_X */
