#include "label_gen.h"
#include "ir_module.h"
#include "ir_while.h"
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
x86_gen_code(IrModule *module,
             FILE *out_stream,
             const char *source_file);

/**
 * Get the set of available registers on x86 platform.
 *
 * The registers of different categories are returns as list of
 * iml_register_t objects.
 *
 * @param scratch the list of scratch registers
 * @param preserved the list of registers that are preserved across
 *                  function calls
 */
void
x86_get_registers(GSList **scratch,
                  GSList **preserved);

void
x86_compile_expression(x86_comp_params_t *params,
                       IrExpression *expression,
                       sym_table_t *sym_table);

int
x86_get_expression_storage_size(IrExpression *expression);

void
x86_compile_assigment(x86_comp_params_t *params,
                      IrAssigment *assigment,
                      sym_table_t *sym_table);

bool
x86_in_reg_as_last_func_arg(DtDataType *parameter_type);

/**
 * Alligned an address/offset in 32-bit word boundary, 
 * by increasing it if needed.
 *
 * @return word alligned address
 */
int
x86_inc_to_word_boundary(int addr);

#endif /* X86_INC_X */
