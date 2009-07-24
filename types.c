#include <stdbool.h>

#include "types.h"
#include "ast_basic_type.h"
#include "ir_cast.h"
#include "ir_int_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static bool
is_literal_0or1(IrExpression *expression);

static IrExpression *
implicit_conv_to_int(IrExpression *expression);

static IrExpression *
implicit_conv_to_bool(IrExpression *expression);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
types_implicit_conv(AstDataType *target_type,
                    IrExpression *expression)
{
    IrExpression *res_exp = NULL;

    if (!XDP_IS_AST_BASIC_TYPE(target_type))
    {
        /* non-basic type conversion not implemented */
        assert(false);
    }

    switch (ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(target_type)))
    {
        case void_type:
            /* not implemented */
            assert(false);
        case bool_type:
            /* not implemented */
            res_exp = implicit_conv_to_bool(expression);
            break;
        case int_type:
            res_exp = implicit_conv_to_int(expression);
            break;
        default:
            /* unexpected target type */
            assert(false);
    }

    return res_exp;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

/**
 * Check if expression is numeric literal 0 or 1.
 */
static bool
is_literal_0or1(IrExpression *expression)
{
    gint32 val;

    if (!IR_IS_INT_CONSTANT(expression))
    {
        return false;
    }

    val = ir_int_constant_get_value(IR_INT_CONSTANT(expression));

    return val == 0 || val == 1;
}

static IrExpression *
implicit_conv_to_int(IrExpression *expression)
{
    IrExpression *res_exp = NULL;
    AstDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!XDP_IS_AST_BASIC_TYPE(exp_data_type))
    {
        return NULL;
    }

    switch (ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(exp_data_type)))
    {
        case int_type:
            res_exp = expression;
            break;
        case bool_type:
            res_exp =
              IR_EXPRESSION(
                  ir_cast_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                              expression));
            break;
        default:
            /* invalid implicit conversion, return NULL */
            break;
    }

    return res_exp;
}

static IrExpression *
implicit_conv_to_bool(IrExpression *expression)
{
    IrExpression *res_exp = NULL;
    AstDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!XDP_IS_AST_BASIC_TYPE(exp_data_type))
    {
        return NULL;
    }

    switch (ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(exp_data_type)))
    {
        case int_type:
            if (is_literal_0or1(expression))
            {
                res_exp =
                 IR_EXPRESSION(
                   ir_cast_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                               expression));
            }
            break;
        case bool_type:
            res_exp = expression;
            break;
        default:
            /* invalid implicit conversion, return NULL */
            break;
    }

    return res_exp;
}

