#ifndef TYPES_INC_X
#define TYPES_INC_X

#include <stdbool.h>

#include "ast_binary_operation.h"
#include "dt_basic.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Check if expression is numeric literal 0 or 1.
 */
bool
types_is_literal_0or1(IrExpression *expression);

/**
 * Perform implicit conversion of an expression to specified
 * data type. The expression is wrapped into cast operation if
 * needed. If possible, constant folding will be performed on resulting
 * wrapped expression.
 *
 * If the expression is of the target type, no modifications are
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

bool
types_pointer_arithm_conv(IrExpression *left,
                          ast_binary_op_type_t operation,
                          IrExpression *right,
                          IrExpression **res_left,
                          IrExpression **res_right);
/**
 * Find common type for type1 and type2, using this algorithm:
 *
 * - if type1 is same as type2, return that type
 * - if type1 or type2 is void, return void type
 * - if both  type1 and type2 are integral types, first promote anything smaller
 *     than 32-bit to int, then return the larger type, with a preference
 *     for unsigned type if tied in size (not implemented)
 * - else try implicitly converting type1 to type2's type and type2 to type1's
 *     type; if exactly one of these succeeds, retutn the type of the
 *     successful conversion target (not implemented)
 *
 * - otherwise no common type can be found
 *
 * @return the common type or NULL if no common type can be found
 */
DtDataType *
types_find_common(DtDataType *type1, DtDataType *type2);

/**
 * Returns true if data_type represent basic type void,
 * false otherwise.
 */
bool
types_is_void(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type bool,
 * false otherwise.
 */
bool
types_is_bool(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type char,
 * false otherwise.
 */
bool
types_is_char(DtDataType *data_type);


/**
 * Returns true if data_type represent basic type int,
 * false otherwise.
 */
bool
types_is_int(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type uint,
 * false otherwise.
 */
bool
types_is_uint(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type short,
 * false otherwise.
 */
bool
types_is_short(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type ushort,
 * false otherwise.
 */
bool
types_is_ushort(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type byte,
 * false otherwise.
 */
bool
types_is_byte(DtDataType *data_type);

/**
 * Returns true if data_type represent basic type ubyte,
 * false otherwise.
 */
bool
types_is_ubyte(DtDataType *data_type);

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
 * Get singelton instance of object representing short type.
 */
DtDataType *
types_get_short_type();

/**
 * Get singelton instance of object representing ushort type.
 */
DtDataType *
types_get_ushort_type();

/**
 * Get singelton instance of object representing byte type.
 */
DtDataType *
types_get_byte_type();

/**
 * Get singelton instance of object representing ubyte type.
 */
DtDataType *
types_get_ubyte_type();

/**
 * Get singelton instance of object representing char type.
 */
DtDataType *
types_get_char_type();

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
