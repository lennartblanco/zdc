#ifndef IR_TO_IML_INC_X
#define IR_TO_IML_INC_X

#include "ir_function_def.h"
#include "ir_assignment.h"
#include "ir_function_call.h"
#include "ir_foreach.h"
#include "ir_variable.h"
#include "ir_var_value.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * @param is_function_parameter flag used to indicate that the variable is
 *                              functions parameter
 *                              if not set, then the variable is local
 */
void
add_to_func_frame(iml_function_t *function,
                  IrVariable *variable,
                  bool is_function_parameter);

void
iml_add_assignment(iml_function_t *function,
                   IrExpression *lvalue,
                   IrExpression *value);

ImlOperand *
iml_add_expression_eval(iml_function_t *function,
                        IrExpression *ir_expression,
                        ImlVariable *dest,
                        bool discard_result);

ImlOperand *
iml_add_call_eval(iml_function_t *function, IrCall *call, ImlVariable *res);

void
iml_add_while_head(iml_function_t *function,
                   IrExpression *condition,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end);

void
iml_add_while_tail(iml_function_t *function,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end);

void
iml_add_foreach_head(iml_function_t *function,
                     IrForeach *foreach,
                     ImlVariable **index,
                     ImlVariable **length,
                     iml_operation_t *loop_head,
                     iml_operation_t *loop_end);

void
iml_add_foreach_tail(iml_function_t *function,
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
iml_add_foreach_range_head(iml_function_t *function,
                           IrVarValue *index,
                           IrExpression *lower_exp,
                           IrExpression *loop_test_exp,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end);
void
iml_add_foreach_range_tail(iml_function_t *function,
                           IrExpression *inc_exp,
                           ImlOperand *head_temp_op,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end);

void
iml_add_for_head(iml_function_t *function,
                 IrExpression *test,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end);

void
iml_add_for_tail(iml_function_t *function,
                 IrExpression *step,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end);

#endif /* IR_TO_IML_INC_X */
