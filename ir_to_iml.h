#ifndef IR_TO_IML_INC_X
#define IR_TO_IML_INC_X

#include "ir_function_def.h"
#include "ir_function_call.h"
#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * @param is_function_parameter flag used to indicate that the variable is
 *                              functions parameter
 *                              if not set, then the variable is local
 */
void
add_to_func_frame(IrFunctionDef *parent_function,
                  IrVariable *variable,
                  bool is_function_parameter);

ImlOperand *
iml_add_expression_eval(IrFunctionDef *function,
                        IrExpression *ir_expression);

ImlOperand *
iml_add_func_call_eval(IrFunctionDef *function, IrFunctionCall *func_call);

#endif /* IR_TO_IML_INC_X */
