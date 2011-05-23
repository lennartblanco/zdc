#include <stdbool.h>

#include "types.h"
#include "types_arrays.h"
#include "dt_array.h"
#include "dt_static_array.h"
#include "dt_enum.h"
#include "ir_array_literal.h"
#include "ir_array_slice.h"
#include "ir_cast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrExpression *
implicit_conv_to_int_array_type(DtArray *target_type,
                                DtArray *source_type,
                                IrExpression *expression);

static IrExpression *
implicit_conv_to_uint_array_type(DtArray *target_type,
                                 DtArray *source_type,
                                 IrExpression *expression);

static IrExpression *
implicit_conv_to_bool_array_type(DtArray *target_type,
                                 DtArray *source_type,
                                 IrExpression *expression);

static IrExpression *
implicit_conv_to_char_array_type(DtArray *target_type,
                                 DtArray *source_type,
                                 IrExpression *expression);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrExpression *
implicit_conv_to_int_array_type(DtArray *target_type,
                                DtArray *source_type,
                                IrExpression *expression)
{
    assert(DT_IS_ARRAY(target_type));
    assert(DT_IS_ARRAY(source_type));

    DtBasic *element_type;

    /* only arrays over basic types as source expression are supported */
    element_type = DT_BASIC(dt_array_get_data_type(DT_ARRAY(source_type)));

    switch (dt_basic_get_data_type(element_type))
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
implicit_conv_to_uint_array_type(DtArray *target_type,
                                 DtArray *source_type,
                                 IrExpression *expression)
{
    assert(DT_IS_ARRAY(target_type));
    assert(DT_IS_ARRAY(source_type));

    DtBasic *element_type;

    /* only arrays over basic types as source expression are supported */
    element_type = DT_BASIC(dt_array_get_data_type(DT_ARRAY(source_type)));

    switch (dt_basic_get_data_type(element_type))
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
implicit_conv_to_bool_array_type(DtArray *target_type,
                                 DtArray *source_type,
                                 IrExpression *expression)
{
    assert(DT_IS_ARRAY(target_type));
    assert(DT_IS_ARRAY(source_type));

/*@ todo: implement and test int[] and uint[] to  bool[] cases */

    DtBasic *element_type;

    /* only arrays over basic types as source expression are supported */
    element_type = DT_BASIC(dt_array_get_data_type(DT_ARRAY(source_type)));

    switch (dt_basic_get_data_type(element_type))
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

static IrExpression *
implicit_conv_to_char_array_type(DtArray *target_type,
                                 DtArray *source_type,
                                 IrExpression *expression)
{
    assert(DT_IS_ARRAY(target_type));
    assert(DT_IS_ARRAY(source_type));

    DtBasic *element_type;

    /* only arrays over basic types as source expression are supported */
    element_type = DT_BASIC(dt_array_get_data_type(DT_ARRAY(source_type)));

    switch (dt_basic_get_data_type(element_type))
    {
        case char_type:
            return expression;
        case bool_type:
        case int_type:
        case uint_type:
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
types_arrays_implicit_conv(DtArray *target_type,
                           IrExpression *expression)
{
    assert(DT_IS_ARRAY(target_type));
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *source_type;
    basic_data_type_t bdt;

    source_type = ir_expression_get_data_type(expression);

    if (DT_IS_ENUM(source_type))
    {
        source_type = dt_enum_get_base_type(DT_ENUM(source_type));
    }

    if (!DT_IS_ARRAY(source_type) &&
        !DT_IS_STATIC_ARRAY_TYPE(source_type))
    {
        return NULL;
    }

    DtArray *src_arry_type;
    DtArray *trg_arry_type;

    src_arry_type = DT_ARRAY(source_type);
    trg_arry_type = DT_ARRAY(target_type);

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
          dt_static_array_get_length(DT_STATIC_ARRAY(src_arry_type));
       dst_len = 
          dt_static_array_get_length(DT_STATIC_ARRAY(trg_arry_type));

       if (src_len != dst_len)
       {
          return NULL;
       }
    }

    bdt =
        dt_basic_get_data_type(
            DT_BASIC(dt_array_get_data_type(trg_arry_type)));
    switch (bdt)
    {
        case int_type:
            return implicit_conv_to_int_array_type(trg_arry_type,
                                                   DT_ARRAY(source_type),
                                                   expression);
        case uint_type:
            return implicit_conv_to_uint_array_type(trg_arry_type,
                                                    DT_ARRAY(source_type),
                                                    expression);
        case bool_type:
            return implicit_conv_to_bool_array_type(trg_arry_type,
                                                    DT_ARRAY(source_type),
                                                    expression);
        case char_type:
            return implicit_conv_to_char_array_type(trg_arry_type,
                                                    DT_ARRAY(source_type),
                                                    expression);
        default:
            /* unexpected element type */
            assert(false);
    }
}

