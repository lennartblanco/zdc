#include <stdbool.h>

#include "types.h"
#include "types_arrays.h"
#include "dt_array_type.h"
#include "ir_array_literal.h"
#include "ir_array_slice.h"
#include "ir_cast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrExpression *
implicit_conv_to_int_array_type(DtArrayType *target_type,
                                IrExpression *expression);

static IrExpression *
implicit_conv_to_uint_array_type(DtArrayType *target_type,
                                 IrExpression *expression);

static IrExpression *
implicit_conv_to_bool_array_type(DtArrayType *target_type,
                                 IrExpression *expression);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrExpression *
implicit_conv_to_int_array_type(DtArrayType *target_type,
                                IrExpression *expression)
{
    assert(DT_IS_ARRAY_TYPE(target_type));
    assert(IR_IS_ARRAY_LITERAL(expression) ||
           IR_IS_ARRAY_SLICE(expression));

    DtBasicType *src_type;

    /* only arrays over basic types as source expression are supported */
    src_type = 
        DT_BASIC_TYPE(
            dt_array_type_get_data_type(
                DT_ARRAY_TYPE(ir_expression_get_data_type(expression))));

    switch (dt_basic_type_get_data_type(src_type))
    {
        case int_type:
            return expression;
        case uint_type:
        case bool_type:
            return IR_EXPRESSION(ir_cast_new(DT_DATA_TYPE(target_type),
                                             expression));
        default:
            /* unexpected element basic data type */
            assert(false);
    }
}

static IrExpression *
implicit_conv_to_uint_array_type(DtArrayType *target_type,
                                 IrExpression *expression)
{
    assert(DT_IS_ARRAY_TYPE(target_type));
    assert(IR_IS_ARRAY_LITERAL(expression) ||
           IR_IS_ARRAY_SLICE(expression));

    DtBasicType *src_type;

    /* only arrays over basic types as source expression are supported */
    src_type = 
        DT_BASIC_TYPE(
            dt_array_type_get_data_type(
                DT_ARRAY_TYPE(ir_expression_get_data_type(expression))));

    switch (dt_basic_type_get_data_type(src_type))
    {
        case uint_type:
            return expression;
        case int_type:
        case bool_type:
            return IR_EXPRESSION(ir_cast_new(DT_DATA_TYPE(target_type),
                                             expression));
        default:
            /* unexpected element basic data type */
            assert(false);
    }
}

static IrExpression *
implicit_conv_to_bool_array_type(DtArrayType *target_type,
                                 IrExpression *expression)
{
    assert(DT_IS_ARRAY_TYPE(target_type));
    assert(IR_IS_ARRAY_LITERAL(expression) ||
           IR_IS_ARRAY_SLICE(expression));

/*@ todo: implement and test int[] and uint[] to  bool[] cases */

    DtBasicType *src_type;

    /* only arrays over basic types as source expression are supported */
    src_type = 
        DT_BASIC_TYPE(
            dt_array_type_get_data_type(
                DT_ARRAY_TYPE(ir_expression_get_data_type(expression))));

    switch (dt_basic_type_get_data_type(src_type))
    {
        case bool_type:
            return expression;
        case int_type:
            /* not implemented */
            assert(false);
        default:
            /* unexpected element basic data type */
            assert(false);
    }
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
types_arrays_implicit_conv(DtArrayType *target_type,
                           IrExpression *expression)
{
    assert(DT_IS_ARRAY_TYPE(target_type));
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *source_type;
    basic_data_type_t bdt;

    source_type = ir_expression_get_data_type(expression);

    if (!DT_IS_ARRAY_TYPE(source_type) &&
        !DT_IS_STATIC_ARRAY_TYPE(source_type))
    {
        return NULL;
    }

    DtArrayType *src_arry_type;
    DtArrayType *trg_arry_type;

    src_arry_type = DT_ARRAY_TYPE(source_type);
    trg_arry_type = DT_ARRAY_TYPE(target_type);

    /*
     * if both source and target data types are static arrays,
     * check if thier lengths is matching
     */
    if (DT_IS_STATIC_ARRAY_TYPE(src_arry_type) &&
        DT_IS_STATIC_ARRAY_TYPE(trg_arry_type))
    {
       guint src_len;
       guint dst_len;

       src_len = 
          dt_static_array_type_get_length(DT_STATIC_ARRAY_TYPE(src_arry_type));
       dst_len = 
          dt_static_array_type_get_length(DT_STATIC_ARRAY_TYPE(trg_arry_type));

       if (src_len != dst_len)
       {
          return NULL;
       }
    }

    bdt =
        dt_basic_type_get_data_type(
            DT_BASIC_TYPE(dt_array_type_get_data_type(trg_arry_type)));
    switch (bdt)
    {
        case int_type:
            return implicit_conv_to_int_array_type(trg_arry_type, expression);
        case uint_type:
            return implicit_conv_to_uint_array_type(trg_arry_type, expression);
        case bool_type:
            return implicit_conv_to_bool_array_type(trg_arry_type, expression);
        default:
            /* unexpected element type */
            assert(false);
    }
}

