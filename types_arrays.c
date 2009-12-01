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

/**
 * Convert a basic Data Type to a array. That is typecast
 * the expression to element type of the array if possible.
 */
static IrExpression *
implicit_conv_bt_to_array_type(DtArrayType *target_type,
                               IrExpression *expression);

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
implicit_conv_bt_to_array_type(DtArrayType *target_type,
                               IrExpression *expression)
{
    assert(DT_IS_ARRAY_TYPE(target_type));
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *source_type;
    DtDataType *element_type;
    basic_data_type_t bdt;
    IrExpression *res_expression;

    source_type = ir_expression_get_data_type(expression);

    bdt = dt_array_type_get_data_type(target_type);
    element_type = DT_DATA_TYPE(dt_basic_type_new(bdt));

    res_expression = types_implicit_conv(element_type, expression);

    g_object_unref(element_type);

    return res_expression;
}

static IrExpression *
implicit_conv_to_int_array_type(DtArrayType *target_type,
                                IrExpression *expression)
{
    assert(DT_IS_ARRAY_TYPE(target_type));
    assert(IR_IS_ARRAY_LITERAL(expression) ||
           IR_IS_ARRAY_SLICE(expression));

    DtArrayType *arry_type;

    arry_type = DT_ARRAY_TYPE(ir_expression_get_data_type(expression));

    switch (dt_array_type_get_data_type(arry_type))
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

    DtArrayType *arry_type;

    arry_type = DT_ARRAY_TYPE(ir_expression_get_data_type(expression));

    switch (dt_array_type_get_data_type(arry_type))
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

    DtArrayType *arry_type;

/* todo: implement and test int[] and uint[] to  bool[] cases */

    arry_type = DT_ARRAY_TYPE(ir_expression_get_data_type(expression));
    switch (dt_array_type_get_data_type(arry_type))
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

    source_type = ir_expression_get_data_type(expression);

    /* basic type to array assigment */
    if (DT_IS_BASIC_TYPE(source_type))
    {
        return implicit_conv_bt_to_array_type(target_type, expression);
    }

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


    switch (dt_array_type_get_data_type(trg_arry_type))
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

