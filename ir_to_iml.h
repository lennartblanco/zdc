#ifndef IR_TO_IML_INC_X
#define IR_TO_IML_INC_X

#include "ir_function_def.h"
#include "ir_assignment.h"
#include "ir_function_call.h"
#include "ir_foreach.h"
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

void
iml_add_assignment(IrFunctionDef *function,
                  IrExpression *lvalue,
                  IrExpression *value);

ImlOperand *
iml_add_expression_eval(IrFunctionDef *function,
                        IrExpression *ir_expression,
                        ImlVariable *dest,
                        bool discard_result);

ImlOperand *
iml_add_func_call_eval(IrFunctionDef *function,
                       IrFunctionCall *func_call,
                       ImlVariable *res);

void
iml_add_while_head(IrFunctionDef *function,
                   IrExpression *condition,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end);

void
iml_add_while_tail(IrFunctionDef *function,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end);

void
iml_add_foreach_head(IrFunctionDef *function,
                     IrForeach *foreach,
                     ImlVariable **index,
                     ImlVariable **length,
                     iml_operation_t **loop_head,
                     iml_operation_t *loop_end);

void
iml_add_foreach_tail(IrFunctionDef *function,
                     ImlVariable *index,
                     ImlVariable *length,
                     iml_operation_t *loop_label);

/**
 * Generate head iml operation for an foreach loop over a range, e.g.
 *   foreach(i; lower..upper) { }
 *
 * @param index         the loop's index variable
 * @param lower_exp the loop's range lower expressions
 * @param loop_test_exp a boolean expression for testing if
 *                       the loop should be aborted
 * @return the temporary opperand used by generated iml code,
 *         this operand must be passed on to iml_add_foreach_range_tail()
 */
ImlOperand *
iml_add_foreach_range_head(IrFunctionDef *function,
                           IrVariable *index,
                           IrExpression *lower_exp,
                           IrExpression *loop_test_exp,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end);
void
iml_add_foreach_range_tail(IrFunctionDef *function,
                           IrExpression *inc_exp,
                           ImlOperand *head_temp_op,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end);

void
iml_add_for_head(IrFunctionDef *function,
                 IrExpression *test,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end);

void
iml_add_for_tail(IrFunctionDef *function,
                 IrExpression *step,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end);

#endif /* IR_TO_IML_INC_X */
