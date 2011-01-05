#include "ir_module.h"
#include "ir_while.h"
#include "ir_assignment.h"
#include "auxil.h"

#ifndef X86_INC_X
#define X86_INC_X

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef struct x86_comp_params_s
{
    FILE *out;
    IrModule *module;
} x86_comp_params_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_init(arch_backend_t *backend);

void
x86_compile_expression(x86_comp_params_t *params,
                       IrExpression *expression,
                       sym_table_t *sym_table);

int
x86_get_expression_storage_size(IrExpression *expression);

/**
 * Alligned an address/offset in 32-bit word boundary, 
 * by increasing it if needed.
 *
 * @return word alligned address
 */
int
x86_inc_to_word_boundary(int addr);

#endif /* X86_INC_X */
