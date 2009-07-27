#ifndef TYPES_INC_X
#define TYPES_INC_X

#include "ast_data_type.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Perform implicit conversion of an expression to specified
 * data type. The expression is wrapped into cast operation if
 * needed. The expression is of the target type, no modifications are
 * done.
 *
 * @param target_type the type to convert to
 * @param expression  expression to convert
 *
 * @return an type converted expression of target_type or NULL
 *         if it is not legal to convert expression to target_type
 */
IrExpression *
types_implicit_conv(AstDataType *target_type,
                    IrExpression *expression);

/**
 * Return an expression that will preform integer promotion on input
 * expression.
 *
 * If specified expression is of void type or is not basic type, NULL is
 * returned.
 */
IrExpression *
types_integer_promotion(IrExpression *expression);

bool
types_usual_arithm_conv(IrExpression *left,
                        IrExpression *right,
                        IrExpression **res_left,
                        IrExpression **res_right);

#endif /* TYPES_INC_X */
