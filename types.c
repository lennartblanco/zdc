#include <stdbool.h>

#include "types.h"
#include "types_arrays.h"
#include "dt_array_type.h"
#include "dt_static_array_type.h"
#include "ir_cast.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_char_constant.h"
#include "ir_bool_constant.h"
#include "const_fold.h"

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
implicit_conv_to_char(IrExpression *expression);

static IrExpression *
implicit_conv_to_basic_type(DtDataType *target_type, IrExpression *expression);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrExpression *
implicit_conv_to_int(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;
    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC_TYPE(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_type_get_data_type(DT_BASIC_TYPE(exp_data_type)))
    {
        case int_type:
            res_exp = expression;
            break;
        case uint_type:
        case char_type:
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
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;
    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC_TYPE(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_type_get_data_type(DT_BASIC_TYPE(exp_data_type)))
    {
        case uint_type:
            res_exp = expression;
            break;
        case int_type:
        case char_type:
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
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;
    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC_TYPE(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_type_get_data_type(DT_BASIC_TYPE(exp_data_type)))
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
implicit_conv_to_char(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;
    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC_TYPE(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_type_get_data_type(DT_BASIC_TYPE(exp_data_type)))
    {
        case int_type:
        case uint_type:
        case bool_type:
            assert(false); /* not implemented */
            break;
        case char_type:
            res_exp = expression;
            break;
        default:
            /* invalid implicit conversion, return NULL */
            break;
    }

    return res_exp;

}

static IrExpression *
implicit_conv_to_basic_type(DtDataType *target_type, IrExpression *expression)
{
    assert(DT_IS_BASIC_TYPE(target_type));
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;

    switch (dt_basic_type_get_data_type(DT_BASIC_TYPE(target_type)))
    {
        case void_type:
            /* not implemented */
            assert(false);
        case bool_type:
            res_exp = implicit_conv_to_bool(expression);
            break;
        case char_type:
            res_exp = implicit_conv_to_char(expression);
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

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
types_get_default_initializer(DtBasicType *data_type)
{
    assert(DT_IS_BASIC_TYPE(data_type));

    switch (dt_basic_type_get_data_type(data_type))
    {
        case int_type:
            return IR_EXPRESSION(ir_int_constant_new(0, 0));
        case uint_type:
            return IR_EXPRESSION(ir_uint_constant_new(0, 0));
        case bool_type:
            return IR_EXPRESSION(ir_bool_constant_new(false, 0));
        case char_type:
            return IR_EXPRESSION(ir_char_constant_new(255, 0));
        default:
            assert(false);
    }
}

IrExpression *
types_implicit_conv(DtDataType *target_type,
                    IrExpression *expression)
{
    IrExpression *res;
    if (DT_IS_BASIC_TYPE(target_type))
    {
        res = implicit_conv_to_basic_type(target_type, expression);
    }
    else if (DT_IS_ARRAY_TYPE(target_type))
    {
        res = types_arrays_implicit_conv(DT_ARRAY_TYPE(target_type),
                                         expression);
    }
    else
    {
        /* unexpected target type */
        assert(false);
    }

    if (IR_IS_CAST(res))
    {
        res = cfold_cast(IR_CAST(res));
    }

    return res;
}

IrExpression *
types_integer_promotion(IrExpression *expression)
{
    IrExpression *res_exp;

    DtDataType *exp_type;
    basic_data_type_t exp_data_type;

    exp_type = ir_expression_get_data_type(expression);
    if (!(DT_IS_BASIC_TYPE(exp_type))) {
        return NULL;
    }

    exp_data_type = dt_basic_type_get_data_type(DT_BASIC_TYPE(exp_type));

    switch (exp_data_type) {
        case char_type:
        case bool_type:
            res_exp =
                IR_EXPRESSION(ir_cast_new(types_get_int_type(), expression));
            break;
        case void_type:
            res_exp = NULL;
            break;
        case int_type:
        case uint_type:
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
    DtDataType *data_type;
    basic_data_type_t left_data_type;
    basic_data_type_t right_data_type;

    /* 
     * get the data type of left expression
     */

    data_type = ir_expression_get_data_type(left);
    /* only conversions of basic types is implemented */
    assert(DT_IS_BASIC_TYPE(data_type));
    left_data_type =
        dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type));

    if (left_data_type == void_type)
    {
        /* converting void types is illegal */
        return false;
    }
    assert(left_data_type == int_type  ||
           left_data_type == uint_type ||
           left_data_type == char_type ||
           left_data_type == bool_type);

    /* 
     * get the data type of right expression
     */

    data_type = ir_expression_get_data_type(right);
    /* only conversions of basic types is implemented */
    assert(DT_IS_BASIC_TYPE(data_type));
    right_data_type =
        dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type));

    if (right_data_type == void_type)
    {
        /* converting void types is illegal */
        return false;
    }
    assert(right_data_type == int_type  ||
           right_data_type == uint_type ||
           right_data_type == char_type ||
           right_data_type == bool_type);


    /* do integer promotions of both operands */
    *res_left = types_integer_promotion(left);
    *res_right = types_integer_promotion(right);

    /* fetch data-types of integer promoted operands */
    data_type = ir_expression_get_data_type(left);
    assert(DT_IS_BASIC_TYPE(data_type)); /* non-basic types not implemened */
    left_data_type =
        dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type));
    assert(DT_IS_BASIC_TYPE(data_type)); /* non-basic types not implemened */
    right_data_type =
        dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type));

    /* more usual arithmetic conversions rules not implemented */
    assert(right_data_type == left_data_type);


    return true;
}


bool
types_is_void(DtDataType *data_type)
{
    if (!DT_IS_BASIC_TYPE(data_type))
    {
        return false;
    }

    return dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type)) == void_type;
}

bool
types_is_bool(DtDataType *data_type)
{
    if (!DT_IS_BASIC_TYPE(data_type))
    {
        return false;
    }

    return dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type)) == bool_type;
}

bool
types_is_char(DtDataType *data_type)
{
    if (!DT_IS_BASIC_TYPE(data_type))
    {
        return false;
    }

    return dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type)) == char_type;
}


bool
types_is_int(DtDataType *data_type)
{
    if (!DT_IS_BASIC_TYPE(data_type))
    {
        return false;
    }

    return dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type)) == int_type;
}

bool
types_is_uint(DtDataType *data_type)
{
    if (!DT_IS_BASIC_TYPE(data_type))
    {
        return false;
    }

    return dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type)) == uint_type;
}

DtDataType *
types_get_int_type()
{
   static DtDataType *int_data_type = NULL;

   if (int_data_type == NULL)
   {
      int_data_type = DT_DATA_TYPE(dt_basic_type_new(int_type));
   }

   return int_data_type;
}

DtDataType *
types_get_uint_type()
{
   static DtDataType *uint_data_type = NULL;

   if (uint_data_type == NULL)
   {
      uint_data_type = DT_DATA_TYPE(dt_basic_type_new(uint_type));
   }

   return uint_data_type;
}

DtDataType *
types_get_char_type()
{
   static DtDataType *char_data_type = NULL;

   if (char_data_type == NULL)
   {
      char_data_type = DT_DATA_TYPE(dt_basic_type_new(char_type));
   }

   return char_data_type;
}


DtDataType *
types_get_bool_type()
{
   static DtDataType *bool_data_type = NULL;

   if (bool_data_type == NULL)
   {
      bool_data_type = DT_DATA_TYPE(dt_basic_type_new(bool_type));
   }

   return bool_data_type;
}

DtDataType *
types_get_void_type()
{
   static DtDataType *void_data_type = NULL;

   if (void_data_type == NULL)
   {
      void_data_type = DT_DATA_TYPE(dt_basic_type_new(void_type));
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

