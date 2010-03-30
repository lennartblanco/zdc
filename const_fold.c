#include "const_fold.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrExpression *
fold_int_bin_op(IrIntConstant *left,
                IrIntConstant *right,
                ast_binary_op_type_t operation);

static IrExpression *
fold_uint_bin_op(IrUintConstant *left,
                 IrUintConstant *right,
                 ast_binary_op_type_t operation);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
cfold_bin_arithm(IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_arithm(bin_op));

    IrExpression *left = ir_binary_operation_get_left(bin_op);
    IrExpression *right = ir_binary_operation_get_right(bin_op);
    DtDataType *bin_op_type;
  
    if (!ir_expression_is_constant(left) ||
        !ir_expression_is_constant(right))
    {
        return IR_EXPRESSION(bin_op);
    }

    bin_op_type = ir_expression_get_data_type(IR_EXPRESSION(bin_op));

    if (types_is_int(bin_op_type))
    {
        return fold_int_bin_op(IR_INT_CONSTANT(left),
                               IR_INT_CONSTANT(right),
                               ir_binary_operation_get_operation(bin_op));
    }
    else if (types_is_uint(bin_op_type))
    {
        return fold_uint_bin_op(IR_UINT_CONSTANT(left),
                                IR_UINT_CONSTANT(right),
                                ir_binary_operation_get_operation(bin_op));
    }
    /* unexpected data type of arithmetic binary operations */
    assert(false);
}

IrExpression *
cfold_bin_conditional(IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_conditional(bin_op));

    IrExpression *left = ir_binary_operation_get_left(bin_op);
    IrExpression *right = ir_binary_operation_get_right(bin_op);

    if (!ir_expression_is_constant(left) ||
        !ir_expression_is_constant(right))
    {
        return IR_EXPRESSION(bin_op);
    }

    /* not implemented */
    assert(false);

    return IR_EXPRESSION(bin_op);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrExpression *
fold_int_bin_op(IrIntConstant *left,
                IrIntConstant *right,
                ast_binary_op_type_t operation)
{
    gint32 left_val;
    gint32 right_val;
    gint32 res;

    left_val = ir_int_constant_get_value(left);
    right_val = ir_int_constant_get_value(right);

    switch (operation) {
        case ast_plus_op:
            res = left_val + right_val;
            break;
        case ast_minus_op:
            res = left_val - right_val;
            break;
        case ast_mult_op:
            res = left_val * right_val;
            break;
        case ast_division_op:
            res = left_val / right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_int_constant_new(res, 0));
}

static IrExpression *
fold_uint_bin_op(IrUintConstant *left,
                 IrUintConstant *right,
                 ast_binary_op_type_t operation)
{
    guint32 left_val;
    guint32 right_val;
    guint32 res;

    left_val = ir_uint_constant_get_value(left);
    right_val = ir_uint_constant_get_value(right);

    switch (operation) {
        case ast_plus_op:
            res = left_val + right_val;
            break;
        case ast_minus_op:
            res = left_val - right_val;
            break;
        case ast_mult_op:
            res = left_val * right_val;
            break;
        case ast_division_op:
            res = left_val / right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_uint_constant_new(res, 0));
}

