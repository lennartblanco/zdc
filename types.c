#include <stdbool.h>

#include "types.h"
#include "types_arrays.h"
#include "dt_enum.h"
#include "dt_array.h"
#include "dt_static_array.h"
#include "dt_pointer.h"
#include "ir_function.h"
#include "ir_cast.h"
#include "ir_basic_constant.h"
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

static IrExpression *
implicit_conv_to_pointer(DtPointer *target_type, IrExpression *expression);

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

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_get_data_type(DT_BASIC(exp_data_type)))
    {
        case int_type:
            res_exp = expression;
            break;
        case byte_type:
        case ubyte_type:
        case short_type:
        case ushort_type:
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

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_get_data_type(DT_BASIC(exp_data_type)))
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
implicit_conv_to_short(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    basic_data_type_t exp_basic_type =
        dt_basic_get_data_type(DT_BASIC(exp_data_type));

    if (exp_basic_type == short_type)
    {
        return expression;
    }
    else if (exp_basic_type == ushort_type)
    {
        assert(false); /* not implemented */
    }
    else if (exp_basic_type == int_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        gint32 val = ir_basic_constant_get_int(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXINT16 || val < G_MININT16)
        {
            return NULL;
        }

        return
          IR_EXPRESSION(ir_cast_new(types_get_short_type(), expression));
    }
    else if (exp_basic_type == uint_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        guint32 val =
            ir_basic_constant_get_uint(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXINT16)
        {
            return NULL;
        }
        return
          IR_EXPRESSION(ir_cast_new(types_get_short_type(), expression));
    }

    /* invalid implicit cast */
    return NULL;
}

static IrExpression *
implicit_conv_to_ushort(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    basic_data_type_t exp_basic_type =
        dt_basic_get_data_type(DT_BASIC(exp_data_type));

    if (exp_basic_type == ushort_type)
    {
        return expression;
    }
    else if (exp_basic_type == short_type)
    {
        assert(false); /* not implemented */
    }
    else if (exp_basic_type == int_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        gint32 val = ir_basic_constant_get_int(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXUINT16 || val < 0)
        {
            return NULL;
        }
        return
          IR_EXPRESSION(ir_cast_new(types_get_ushort_type(), expression));
    }
    else if (exp_basic_type == uint_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        guint32 val = ir_basic_constant_get_uint(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXUINT16)
        {
            return NULL;
        }
        return
          IR_EXPRESSION(ir_cast_new(types_get_ushort_type(), expression));
    }

    /* invalid implicit cast */
    return NULL;
}

static IrExpression *
implicit_conv_to_byte(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    basic_data_type_t exp_basic_type =
        dt_basic_get_data_type(DT_BASIC(exp_data_type));

    if (exp_basic_type == byte_type)
    {
        return expression;
    }
    else if (exp_basic_type == int_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        gint32 val = ir_basic_constant_get_int(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXINT8 || val < G_MININT8)
        {
            return NULL;
        }

        return
          IR_EXPRESSION(ir_cast_new(types_get_byte_type(), expression));
    }
    else if (exp_basic_type == uint_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        guint32 val =
            ir_basic_constant_get_uint(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXINT8)
        {
            return NULL;
        }
        return
          IR_EXPRESSION(ir_cast_new(types_get_byte_type(), expression));
    }

    /* invalid implicit cast */
    return NULL;
}

static IrExpression *
implicit_conv_to_ubyte(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    basic_data_type_t exp_basic_type =
        dt_basic_get_data_type(DT_BASIC(exp_data_type));
    if (exp_basic_type == ubyte_type)
    {
        return expression;
    }
    else if (exp_basic_type == int_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        gint32 val = ir_basic_constant_get_int(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXUINT8 || val < 0)
        {
            return NULL;
        }

        return
          IR_EXPRESSION(ir_cast_new(types_get_ubyte_type(), expression));
    }
    else if (exp_basic_type == uint_type)
    {
        if (!IR_IS_BASIC_CONSTANT(expression))
        {
            return NULL;
        }

        guint32 val =
            ir_basic_constant_get_uint(IR_BASIC_CONSTANT(expression));
        if (val > G_MAXUINT8)
        {
            return NULL;
        }
        return
          IR_EXPRESSION(ir_cast_new(types_get_ubyte_type(), expression));
    }

    /* invalid implicit cast */
    return NULL;
}

static IrExpression *
implicit_conv_to_bool(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;
    DtDataType *exp_data_type;
    exp_data_type = ir_expression_get_data_type(expression);

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_get_data_type(DT_BASIC(exp_data_type)))
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

    if (!DT_IS_BASIC(exp_data_type))
    {
        return NULL;
    }

    switch (dt_basic_get_data_type(DT_BASIC(exp_data_type)))
    {
        case char_type:
            res_exp = expression;
            break;
        case bool_type:
            res_exp =
              IR_EXPRESSION(ir_cast_new(types_get_char_type(), expression));
            break;
        case void_type:
        case int_type:
        case uint_type:
            /* invalid implicit conversion, return NULL */
            break;
        default:
            /* unexpected data type */
            assert(false);
    }

    return res_exp;

}

static IrExpression *
implicit_conv_to_basic_type(DtDataType *target_type, IrExpression *expression)
{
    assert(DT_IS_BASIC(target_type));
    assert(IR_IS_EXPRESSION(expression));

    IrExpression *res_exp = NULL;
    DtDataType *source_type;

    if (IR_IS_FUNCTION(expression))
    {
        /* can't convert function name to basic type */
        return NULL;
    }

    source_type = ir_expression_get_data_type(expression);
    if (DT_IS_ENUM(source_type))
    {
        DtDataType *base_type;

        base_type = dt_enum_get_base_type(DT_ENUM(source_type));
        if (!dt_data_type_is_same(target_type, base_type))
        {
            return NULL;
        }
        return IR_EXPRESSION(ir_cast_new(target_type, expression));
    }

    switch (dt_basic_get_data_type(DT_BASIC(target_type)))
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
        case short_type:
            res_exp = implicit_conv_to_short(expression);
            break;
        case ushort_type:
            res_exp = implicit_conv_to_ushort(expression);
            break;
        case byte_type:
            res_exp = implicit_conv_to_byte(expression);
            break;
        case ubyte_type:
            res_exp = implicit_conv_to_ubyte(expression);
            break;
        default:
            /* unexpected target type */
            assert(false);
    }

    return res_exp;
}

static IrExpression *
implicit_conv_to_pointer(DtPointer *target_type, IrExpression *expression)
{
    assert(DT_IS_POINTER(target_type));
    assert(IR_IS_EXPRESSION(expression));

    if (dt_data_type_is_same(DT_DATA_TYPE(target_type),
                              ir_expression_get_data_type(expression)))
    {
        return expression;
    }

    return NULL;
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
types_implicit_conv(DtDataType *target_type,
                    IrExpression *expression)
{
    IrExpression *res;

    if (DT_IS_BASIC(target_type))
    {
        res = implicit_conv_to_basic_type(target_type, expression);
    }
    else if (DT_IS_ARRAY(target_type))
    {
        res = types_arrays_implicit_conv(DT_ARRAY(target_type), expression);
    }
    else if (DT_IS_ENUM(target_type))
    {
        if (dt_data_type_is_same(target_type,
                                 ir_expression_get_data_type(expression)))
        {
            res = expression;
        }
        else
        {
            res = NULL;
        }
    }
    else if (DT_IS_POINTER(target_type))
    {
        res = implicit_conv_to_pointer(DT_POINTER(target_type),
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

    if (DT_IS_ENUM(exp_type))
    {
        exp_type = dt_enum_get_base_type(DT_ENUM(exp_type));
    }

    if (!(DT_IS_BASIC(exp_type)))
    {
        return NULL;
    }

    exp_data_type = dt_basic_get_data_type(DT_BASIC(exp_type));

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
types_pointer_arithm_conv(IrExpression *left,
                          ast_binary_op_type_t operation,
                          IrExpression *right,
                          IrExpression **res_left,
                          IrExpression **res_right)
{
    assert(operation == ast_plus_op || operation == ast_minus_op);

    DtDataType *left_type = ir_expression_get_data_type(left);
    DtDataType *right_type = ir_expression_get_data_type(right);

    assert(DT_IS_POINTER(left_type) || DT_IS_POINTER(right_type));

    /*
     * promote non-pointer operand to integer if needed
     */
    if (!DT_IS_POINTER(left_type))
    {
        *res_left = types_integer_promotion(left);
        if (*res_left == NULL)
        {
            /* can't promote left operand to integer */
            return false;
        }
    }

    if (!DT_IS_POINTER(right_type))
    {
        *res_right = types_integer_promotion(right);
        if (*res_right == NULL)
        {
            /* can't promote right operand to integer */
            return false;
        }
    }

    if (operation == ast_plus_op)
    {
      if (DT_IS_POINTER(left_type) && DT_IS_POINTER(right_type))
      {
          /* adding two pointers is illegal */
          return false;
      }

      if (!DT_IS_POINTER(left_type))
      {
          /*
           * if left operand is integral and right is pointer,
           * the operands are reversed
           */
          IrExpression *tmp;

          tmp = *res_left;
          *res_left = *res_right;
          *res_right = tmp;
      }

    }
    else
    {
        assert(operation == ast_minus_op);
        if (!DT_IS_POINTER(left_type))
        {
            /* it's illegal to substract pointer from a non-pointer */
            return false;
        }
    }

    return true;
}

bool
types_usual_arithm_conv(IrExpression *left,
                        IrExpression *right,
                        IrExpression **res_left,
                        IrExpression **res_right)
{
    DtDataType *left_type;
    DtDataType *right_type;

    /* 
     * get the data type of left expression
     */

    left_type = ir_expression_get_data_type(left);
    if (types_is_void(left_type))
    {
        /* converting void types is illegal */
        return false;
    }

    /* 
     * get the data type of right expression
     */

    right_type = ir_expression_get_data_type(right);
    /* only conversions of basic types is implemented */
    if (types_is_void(right_type))
    {
        /* converting void types is illegal */
        return false;
    }

    /* do integer promotion of left operand */
    *res_left = types_integer_promotion(left);
    if (*res_left == NULL)
    {
        /* can't promote left operand to integer */
        return false;
    }

    /* do integer promotion of right operand */
    *res_right = types_integer_promotion(right);
    if (*res_right == NULL)
    {
        /* can't promote right operand to integer */
        return false;
    }


    /* fetch data-types of integer promoted operands */
    left_type = ir_expression_get_data_type(*res_left);
    right_type = ir_expression_get_data_type(*res_right);

    if (dt_data_type_is_same(left_type, right_type))
    {
        /* operands are of the same type, we are done */
        return true;
    }

    if (types_is_uint(left_type) && types_is_int(right_type))
    {
        /* right operand need to be converted to unsigned */
        *res_right = 
            IR_EXPRESSION(ir_cast_new(types_get_uint_type(), *res_right));

    }
    else if (types_is_int(left_type) && types_is_uint(right_type))
    {
        /* left operand need to be converted to unsigned */
        *res_left =
            IR_EXPRESSION(ir_cast_new(types_get_uint_type(), *res_left));
    }
    else
    {
        /* unexpected operand types combination */
        assert(false);
    }

    return true;
}


bool
types_is_void(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == void_type;
}

bool
types_is_bool(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == bool_type;
}

bool
types_is_char(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == char_type;
}


bool
types_is_int(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == int_type;
}

bool
types_is_uint(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == uint_type;
}

bool
types_is_short(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == short_type;
}

bool
types_is_ushort(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == ushort_type;
}

bool
types_is_byte(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == byte_type;
}

bool
types_is_ubyte(DtDataType *data_type)
{
    if (!DT_IS_BASIC(data_type))
    {
        return false;
    }

    return dt_basic_get_data_type(DT_BASIC(data_type)) == ubyte_type;
}

DtDataType *
types_get_int_type()
{
   static DtDataType *int_data_type = NULL;

   if (int_data_type == NULL)
   {
      int_data_type = DT_DATA_TYPE(dt_basic_new(int_type));
   }

   return int_data_type;
}

DtDataType *
types_get_uint_type()
{
   static DtDataType *uint_data_type = NULL;

   if (uint_data_type == NULL)
   {
      uint_data_type = DT_DATA_TYPE(dt_basic_new(uint_type));
   }

   return uint_data_type;
}

DtDataType *
types_get_short_type()
{
   static DtDataType *short_data_type = NULL;

   if (short_data_type == NULL)
   {
       short_data_type = DT_DATA_TYPE(dt_basic_new(short_type));
   }

   return short_data_type;
}

DtDataType *
types_get_ushort_type()
{
   static DtDataType *ushort_data_type = NULL;

   if (ushort_data_type == NULL)
   {
       ushort_data_type = DT_DATA_TYPE(dt_basic_new(ushort_type));
   }

   return ushort_data_type;
}

DtDataType *
types_get_byte_type()
{
   static DtDataType *byte_data_type = NULL;

   if (byte_data_type == NULL)
   {
       byte_data_type = DT_DATA_TYPE(dt_basic_new(byte_type));
   }

   return byte_data_type;
}

DtDataType *
types_get_ubyte_type()
{
   static DtDataType *ubyte_data_type = NULL;

   if (ubyte_data_type == NULL)
   {
       ubyte_data_type = DT_DATA_TYPE(dt_basic_new(ubyte_type));
   }

   return ubyte_data_type;
}

DtDataType *
types_get_char_type()
{
   static DtDataType *char_data_type = NULL;

   if (char_data_type == NULL)
   {
      char_data_type = DT_DATA_TYPE(dt_basic_new(char_type));
   }

   return char_data_type;
}


DtDataType *
types_get_bool_type()
{
   static DtDataType *bool_data_type = NULL;

   if (bool_data_type == NULL)
   {
      bool_data_type = DT_DATA_TYPE(dt_basic_new(bool_type));
   }

   return bool_data_type;
}

DtDataType *
types_get_void_type()
{
   static DtDataType *void_data_type = NULL;

   if (void_data_type == NULL)
   {
      void_data_type = DT_DATA_TYPE(dt_basic_new(void_type));
   }

   return void_data_type;
}

bool
types_is_literal_0or1(IrExpression *expression)
{
    bool res;

    if (!IR_IS_BASIC_CONSTANT(expression))
    {
        return false;
    }

    DtDataType *exp_type = ir_expression_get_data_type(expression);

    if (types_is_int(exp_type))
    {
         gint32 val = ir_basic_constant_get_int(IR_BASIC_CONSTANT(expression));
         res = (val == 0 || val == 1);
    }
    else if (types_is_uint(exp_type))
    {
         guint32 val =
             ir_basic_constant_get_uint(IR_BASIC_CONSTANT(expression));
         res = (val == 0 || val == 1);
    }
    else
    {
        assert(false); /* unexpected expression data type */
    }

    return res;
}
