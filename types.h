#ifndef TYPES_INC_X
#define TYPES_INC_X

#include "dt_basic_type.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Get storage size in bytes of an basic data type.
 */
int
types_get_storage_size(basic_data_type_t data_type);

/**
 * Check if expression is numeric literal 0 or 1.
 */
bool
types_is_literal_0or1(IrExpression *expression);

/**
 * Perform implicit conversion of an expression to specified
 * data type. The expression is wrapped into cast operation if
 * needed. If the expression is of the target type, no modifications are
 * done.
 *
 * @param target_type the type to convert to
 * @param expression  expression to convert
 *
 * @return an type converted expression of target_type or NULL
 *         if it is not legal to convert expression to target_type
 */
IrExpression *
types_implicit_conv(DtDataType *target_type,
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
/**
 * Get default initializer expression for a basic data type.
 */
IrExpression *
types_get_default_initializer(basic_data_type_t data_type);

/**
 * Returns true if data_type represent basic type void,
 * false otherwise.
 */
bool
types_is_void(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type void,
 * false otherwise.
 */
bool
types_is_bool(DtDataType *data_type);

/**
 * Get singelton instance of object representing int type.
 */
DtDataType *
types_get_int_type();

/**
 * Get singelton instance of object representing uint type.
 */
DtDataType *
types_get_uint_type();

/**
 * Get singelton instance of object representing bool type.
 */
DtDataType *
types_get_bool_type();

/**
 * Get singelton instance of object representing void type.
 */
DtDataType *
types_get_void_type();

#endif /* TYPES_INC_X */
