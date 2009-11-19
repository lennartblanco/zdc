#ifndef X86_FUNC_CALL_INC_X
#define X86_FUNC_CALL_INC_X

#include "x86.h"
#include "ir_function_call.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_func_call(x86_comp_params_t *params,
                      IrFunctionCall *func_call,
                      sym_table_t *sym_table,
                      bool retain_return_value);

#endif /* X86_FUNC_CALL_INC_X */
