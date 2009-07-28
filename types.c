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

IrExpression *
types_integer_promotion(IrExpression *expression)
{
    IrExpression *res_exp;

    AstDataType *exp_type;
    basic_data_type_t exp_data_type;

    exp_type = ir_expression_get_data_type(expression);
    if (!(XDP_IS_AST_BASIC_TYPE(exp_type))) {
        return NULL;
    }

    exp_data_type = ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(exp_type));

    switch (exp_data_type) {
        case bool_type:
            res_exp = IR_EXPRESSION(
                          ir_cast_new(
                              XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                              expression));
            break;
        case void_type:
            res_exp = NULL;
            break;
        case int_type:
            /* we don't need to do any type casts */
            res_exp = expression;
            break;
        default:
            /* unexpected basic type */
            assert(false);
    }

    return res_exp;
}

bool
types_usual_arithm_conv(IrExpression *left,
                        IrExpression *right,
                        IrExpression **res_left,
                        IrExpression **res_right)
{
    AstDataType *data_type;
    basic_data_type_t left_data_type;
    basic_data_type_t right_data_type;

    /* 
     * get the data type of left expression
     */

    data_type = ir_expression_get_data_type(left);
    /* only conversions of basic types is implemented */
    assert(XDP_IS_AST_BASIC_TYPE(data_type));
    left_data_type =
        ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(data_type));

    if (left_data_type == void_type)
    {
        /* converting void types is illegal */
        return false;
    }
    assert(left_data_type == int_type || left_data_type == bool_type);

    /* 
     * get the data type of right expression
     */

    data_type = ir_expression_get_data_type(right);
    /* only conversions of basic types is implemented */
    assert(XDP_IS_AST_BASIC_TYPE(data_type));
    right_data_type =
        ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(data_type));

    if (right_data_type == void_type)
    {
        /* converting void types is illegal */
        return false;
    }
    assert(right_data_type == int_type || right_data_type == bool_type);


    *res_left = types_integer_promotion(left);
    *res_right = types_integer_promotion(right);

    return true;
}


bool
types_is_void(AstDataType *data_type)
{
    if (!XDP_IS_AST_BASIC_TYPE(data_type))
    {
        return false;
    }

    return 
      ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(data_type)) == void_type;
}

bool
types_is_bool(AstDataType *data_type)
{
    if (!XDP_IS_AST_BASIC_TYPE(data_type))
    {
        return false;
    }

    return 
      ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(data_type)) == bool_type;
}


AstDataType *
types_get_bool_type()
{
   static AstDataType *bool_data_type = NULL;

   if (bool_data_type == NULL)
   {
      bool_data_type = XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type));
   }

   return bool_data_type;
}

AstDataType *
types_get_void_type()
{
   static AstDataType *void_data_type = NULL;

   if (void_data_type == NULL)
   {
      void_data_type = XDP_AST_DATA_TYPE(ast_basic_type_new(void_type));
   }

   return void_data_type;
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

