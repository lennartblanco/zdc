#include <stdbool.h>

#include "types.h"
#include "ast_basic_type.h"
#include "ast_static_array_type.h"
#include "ir_cast.h"
#include "ir_int_constant.h"
#include "ir_bool_constant.h"
#include "ir_array_literal.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrExpression *
implicit_conv_to_int(IrExpression *expression);

static IrExpression *
implicit_conv_to_uint(IrExpression *expression);

static IrExpression *
implicit_conv_to_bool(IrExpression *expression);

static IrExpression *
implicit_conv_to_basic_type(AstDataType *target_type, IrExpression *expression);

static IrExpression *
implicit_conv_to_static_array_type(AstDataType *target_type,
                                   IrExpression *expression);

/**
 * preform implicit conversion of an static int array literal to to static bool
 * array
 *
 * @return implicitly converted expression, or NULL if such conversation
 *         is illegal
 */
static IrExpression *
implicit_conv_static_int_array_to_bool(IrExpression *expression);

/**
 * preform implicit conversion of an expression to static bool array
 *
 * @return implicitly converted expression, or NULL if such conversation
 *         is illegal
 */
static IrExpression *
implicit_conv_to_static_bool_array_type(IrExpression *expression);

/**
 * preform implicit conversion of an expression to static int array
 *
 * @return implicitly converted expression, or NULL if such conversation
 *         is illegal
 */
static IrExpression *
implicit_conv_to_static_int_array_type(IrExpression *expression);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

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
        case uint_type:
        case bool_type:
            res_exp =
              IR_EXPRESSION(ir_cast_new(types_get_int_type(), expression));
            break;
        default:
            /* invalid implicit conversion, return NULL */
            break;
    }

    return res_exp;
}

static IrExpression *
implicit_conv_to_uint(IrExpression *expression)
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
        case uint_type:
            res_exp = expression;
            break;
        case int_type:
        case bool_type:
            res_exp =
              IR_EXPRESSION(ir_cast_new(types_get_uint_type(), expression));
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
        case uint_type:
            if (types_is_literal_0or1(expression))
            {
                res_exp =
                 IR_EXPRESSION(ir_cast_new(types_get_bool_type(), expression));
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

static IrExpression *
implicit_conv_to_basic_type(AstDataType *target_type, IrExpression *expression)
{
    assert(XDP_IS_AST_BASIC_TYPE(target_type));
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;

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
        case uint_type:
            res_exp = implicit_conv_to_uint(expression);
            break;
        default:
            /* unexpected target type */
            assert(false);
    }

    return res_exp;
}

static IrExpression *
implicit_conv_static_int_array_to_bool(IrExpression *expression)
{
    assert(IR_IS_ARRAY_LITERAL(expression));

    IrArrayLiteral *array = IR_ARRAY_LITERAL(expression);
    GSList *i;

    /*
     * check if it is possible implicitly cast this array literal
     * to static boolean array
     */
    i = ir_array_literal_get_values(array);
    for (; i != NULL; i = g_slist_next(i))
    {
        if (!types_is_literal_0or1(i->data))
        {
            return NULL;
        }
    }

    IrCast *cast_exp;
    int length;
    AstStaticArrayType *arry_type;

    arry_type =
        XDP_AST_STATIC_ARRAY_TYPE(ir_expression_get_data_type(expression));

    length = 
        ast_static_array_type_get_length(arry_type);

    cast_exp =
        ir_cast_new(XDP_AST_DATA_TYPE(ast_static_array_type_new(bool_type,
                                                                length)),
                    expression);
    return IR_EXPRESSION(cast_exp);
}

static IrExpression *
implicit_conv_to_static_bool_array_type(IrExpression *expression)
{
    assert(IR_IS_ARRAY_LITERAL(expression));

    AstStaticArrayType *arry_type;

    arry_type =
        XDP_AST_STATIC_ARRAY_TYPE(ir_expression_get_data_type(expression));

    switch (ast_static_array_type_get_data_type(arry_type))
    {
        case bool_type:
            return expression;
        case int_type:
            return implicit_conv_static_int_array_to_bool(expression);
        default:
            /* unexpected element basic data type */
            assert(false);
    }
}

static IrExpression *
implicit_conv_to_static_int_array_type(IrExpression *expression)
{
    assert(IR_IS_ARRAY_LITERAL(expression));

    AstStaticArrayType *arry_type;

    arry_type =
        XDP_AST_STATIC_ARRAY_TYPE(ir_expression_get_data_type(expression));

    switch (ast_static_array_type_get_data_type(arry_type))
    {
        case int_type:
            return expression;
        case bool_type:
        {
            IrCast *cast_exp;
            int length;
            length = ast_static_array_type_get_length(arry_type);
            cast_exp =
              ir_cast_new(XDP_AST_DATA_TYPE(ast_static_array_type_new(int_type,
                                                                      length)),
                          expression);
            return IR_EXPRESSION(cast_exp);

        }
        default:
            /* unexpected element basic data type */
            assert(false);
    }
}

static IrExpression *
implicit_conv_to_static_array_type(AstDataType *target_type,
                                   IrExpression *expression)
{
    assert(XDP_IS_AST_STATIC_ARRAY_TYPE(target_type));
    assert(IR_IS_EXPRESSION(expression));

    AstDataType *source_type;

    source_type = ir_expression_get_data_type(expression);
    if (XDP_IS_AST_BASIC_TYPE(source_type))
    {
        /*
         * conversation of basic type expression to
         * static arrays not implemented 
         */
        assert(false);
    }

    if (!XDP_IS_AST_STATIC_ARRAY_TYPE(source_type))
    {
        return NULL;
    }

    AstStaticArrayType *src_arry_type;
    AstStaticArrayType *dst_arry_type;

    src_arry_type = XDP_AST_STATIC_ARRAY_TYPE(source_type);
    dst_arry_type = XDP_AST_STATIC_ARRAY_TYPE(target_type);

    if (ast_static_array_type_get_length(src_arry_type) !=
        ast_static_array_type_get_length(dst_arry_type))
    {
        return NULL;
    }

    switch (ast_static_array_type_get_data_type(dst_arry_type))
    {
        case int_type:
            return implicit_conv_to_static_int_array_type(expression);
        case bool_type:
            return implicit_conv_to_static_bool_array_type(expression);
        default:
            /* unexpected element type */
            assert(false);
    }
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
types_get_default_initializer(basic_data_type_t data_type)
{
    switch (data_type)
    {
        case int_type:
            return IR_EXPRESSION(ir_int_constant_new(0));
        case bool_type:
            return IR_EXPRESSION(ir_bool_constant_new(false));
        default:
            assert(false);
    }
}

int
types_get_storage_size(basic_data_type_t data_type)
{
    switch (data_type)
    {
        case int_type:
            return 4;
        case bool_type:
            return 1;
        default:
            /* unexpected basic data type */
            assert(false);
    }
}

IrExpression *
types_implicit_conv(AstDataType *target_type,
                    IrExpression *expression)
{

    if (XDP_IS_AST_BASIC_TYPE(target_type))
    {
        return implicit_conv_to_basic_type(target_type, expression);
    }
    else if (XDP_IS_AST_STATIC_ARRAY_TYPE(target_type))
    {
        return implicit_conv_to_static_array_type(target_type,
                                                  expression);
    }

    /* unexpected target type */
    assert(false);
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
types_get_int_type()
{
   static AstDataType *int_data_type = NULL;

   if (int_data_type == NULL)
   {
      int_data_type = XDP_AST_DATA_TYPE(ast_basic_type_new(int_type));
   }

   return int_data_type;
}

AstDataType *
types_get_uint_type()
{
   static AstDataType *uint_data_type = NULL;

   if (uint_data_type == NULL)
   {
      uint_data_type = XDP_AST_DATA_TYPE(ast_basic_type_new(uint_type));
   }

   return uint_data_type;
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

bool
types_is_literal_0or1(IrExpression *expression)
{
    gint32 val;

    /* todo add support for uint constants */
    if (!IR_IS_INT_CONSTANT(expression))
    {
        return false;
    }

    val = ir_int_constant_get_value(IR_INT_CONSTANT(expression));

    return val == 0 || val == 1;
}

