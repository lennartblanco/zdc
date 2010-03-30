/**
 * Constant folding functions.
 */

#ifndef CONST_FOLD_INC_X
#define CONST_FOLD_INC_X

#include "ir_expression.h"
#include "ir_binary_operation.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

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

#endif /* CONST_FOLD_INC_X */
