#include <stdbool.h>

#include "types.h"
#include "dt_enum.h"
#include "dt_array.h"
#include "dt_static_array.h"
#include "dt_pointer.h"
#include "ir_function.h"
#include "ir_cast.h"
#include "ir_basic_constant.h"
#include "const_fold.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
types_implicit_conv(DtDataType *target_type,
                    IrExpression *expression)
{
    if (dt_data_type_is_same(target_type,
                             ir_expression_get_data_type(expression)))
    {
        /* no cast is needed */
        return expression;
    }

    if (!dt_data_type_is_impl_conv(target_type, expression))
    {
        /* can't implicitly convert expression to target type */
        return NULL;
    }

    return cfold_cast(ir_cast_new(target_type, expression));
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

    if (!(dt_is_basic(exp_type)))
    {
        return NULL;
    }

    exp_data_type = dt_basic_get_data_type(DT_BASIC(exp_type));

    switch (exp_data_type) {
        case char_type:
        case bool_type:
        case byte_type:
        case ubyte_type:
        case short_type:
        case ushort_type:
            res_exp =
                cfold_cast(ir_cast_new(types_get_int_type(), expression));
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
    if (dt_basic_is_void(left_type))
    {
        /* converting void types is illegal */
        return false;
    }

    /* 
     * get the data type of right expression
     */

    right_type = ir_expression_get_data_type(right);
    /* only conversions of basic types is implemented */
    if (dt_basic_is_void(right_type))
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

    if (dt_basic_is_uint(left_type) && dt_basic_is_int(right_type))
    {
        /* right operand need to be converted to unsigned */
        *res_right = 
            IR_EXPRESSION(ir_cast_new(types_get_uint_type(), *res_right));

    }
    else if (dt_basic_is_int(left_type) && dt_basic_is_uint(right_type))
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

DtDataType *
types_find_common(DtDataType *type1, DtDataType *type2)
{
    if (dt_data_type_is_same(type1, type2))
    {
        return type1;
    }

    if (dt_basic_is_void(type1) || dt_basic_is_void(type2))
    {
        return types_get_void_type();
    }

    if (dt_data_type_is_integral(type1) && dt_data_type_is_integral(type2))
    {
        assert(false); /* not implmented */
    }
    assert(false); /* not implmented */
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

    if (dt_basic_is_int(exp_type))
    {
         gint32 val = ir_basic_constant_get_int(IR_BASIC_CONSTANT(expression));
         res = (val == 0 || val == 1);
    }
    else if (dt_basic_is_uint(exp_type))
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
