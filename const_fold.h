/**
 * Constant folding functions.
 */

#ifndef CONST_FOLD_INC_X
#define CONST_FOLD_INC_X

#include "ir_expression.h"
#include "ir_binary_operation.h"
#include "ir_conditional.h"
#include "ir_cast.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Perform constant folding on an expression.
 */
IrExpression *
cfold(IrExpression *exp);

/**
 * Perform constant folding on binary integer arithmetic operation.
 */
IrExpression *
cfold_bin_arithm(IrBinaryOperation *bin_op);

/**
 * Perform constant folding on binary conditional (||, &&) operation.
 */
IrExpression *
cfold_bin_conditional(IrBinaryOperation *bin_op);

/**
 * Perform constant folding on binary integer comparison operation.
 */
IrExpression *
cfold_bin_icomp(IrBinaryOperation *bin_op);

/**
 * Perform constant folding on conditional expression.
 */
IrExpression *
cfold_conditional(IrConditional *cond);

IrExpression *
cfold_cast(IrCast *cast_exp);

#endif /* CONST_FOLD_INC_X */
