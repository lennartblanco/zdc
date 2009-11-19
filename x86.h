#include "label_gen.h"
#include "ir_compile_unit.h"
#include "ir_while.h"
#include "ir_variable.h"
#include "ir_assigment.h"

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
int
x86_get_variable_storage_size(IrVariable *variable);

void
x86_compile_assigment(x86_comp_params_t *params,
                      IrAssigment *assigment,
                      sym_table_t *sym_table);

#endif /* X86_INC_X */
