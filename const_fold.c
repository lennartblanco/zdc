#include "const_fold.h"
#include "ir_basic_constant.h"
#include "ir_array_literal.h"
#include "ir_null.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrExpression *
fold_int_arithm_bin_op(IrBasicConstant *left,
                       IrBasicConstant *right,
                       binary_op_type_t operation);

static IrExpression *
fold_uint_arithm_bin_op(IrBasicConstant *left,
                        IrBasicConstant *right,
                        binary_op_type_t operation);

static IrExpression *
fold_int_comp_bin_op(IrBasicConstant *left,
                     IrBasicConstant *right,
                     binary_op_type_t operation);

static IrExpression *
fold_uint_comp_bin_op(IrBasicConstant *left,
                      IrBasicConstant *right,
                      binary_op_type_t operation);

static IrExpression *
cfold_cast_basic_type(DtDataType *target_type,
                      DtDataType *val_type,
                      IrExpression *val);

static IrExpression *
cfold_cast_array_literal(DtArray *target_type,
                         IrArrayLiteral *array_lit);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrExpression *
cfold(IrExpression *exp)
{
    assert(IR_IS_EXPRESSION(exp));

    if (IR_IS_CONSTANT(exp))
    {
        return exp;
    }
    else if (IR_IS_CAST(exp))
    {
        return cfold_cast(ir_cast(exp));
    }
    /* unsupported expression type */
    assert(false);
}

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
        return ir_expression(bin_op);
    }

    left = cfold(left);
    right = cfold(right);

    bin_op_type = ir_expression_get_data_type(ir_expression(bin_op));

    if (dt_basic_is_int(bin_op_type))
    {
        return
            fold_int_arithm_bin_op(ir_basic_constant(left),
                                   ir_basic_constant(right),
                                   ir_binary_operation_get_operation(bin_op));
    }
    else if (dt_basic_is_uint(bin_op_type))
    {
        return
            fold_uint_arithm_bin_op(ir_basic_constant(left),
                                    ir_basic_constant(right),
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
    bool left_val;
    bool right_val;
    bool res;

    if (!ir_expression_is_constant(left) ||
        !ir_expression_is_constant(right))
    {
        return ir_expression(bin_op);
    }

    assert(
        dt_basic_is_bool(ir_expression_get_data_type(ir_expression(bin_op))));

    left_val = ir_basic_constant_get_bool(ir_basic_constant(left));
    right_val = ir_basic_constant_get_bool(ir_basic_constant(right));

    switch (ir_binary_operation_get_operation(bin_op))
    {
        case op_or:
            res = left_val || right_val;
            break;
        case op_and:
            res = left_val && right_val;
            break;
        default:
            /* unexpected binary boolean operation */
            assert(false);
    }

    return ir_expression(ir_basic_constant_new_bool(res, 0));
}

IrExpression *
cfold_bin_icomp(IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_icomp(bin_op));

    IrExpression *left = ir_binary_operation_get_left(bin_op);
    IrExpression *right = ir_binary_operation_get_right(bin_op);
    DtDataType *left_type;

    if (!ir_expression_is_constant(left) ||
        !ir_expression_is_constant(right))
    {
        return ir_expression(bin_op);
    }

    left_type = ir_expression_get_data_type(left);

    if (dt_basic_is_int(left_type))
    {
        assert(dt_basic_is_int(ir_expression_get_data_type(right)));
        return 
           fold_int_comp_bin_op(ir_basic_constant(left),
                                ir_basic_constant(right),
                                ir_binary_operation_get_operation(bin_op));
    }
    else if (dt_basic_is_uint(left_type))
    {
        assert(dt_basic_is_uint(ir_expression_get_data_type(right)));
        return 
           fold_uint_comp_bin_op(ir_basic_constant(left),
                                 ir_basic_constant(right),
                                 ir_binary_operation_get_operation(bin_op));
    }
    /* unexpected data type of integer comparison operations */
    assert(false);
}

IrExpression *
cfold_conditional(IrConditional *cond)
{
    assert(IR_IS_CONDITIONAL(cond));

    IrExpression *exp = ir_conditional_get_cond(cond);

    if (!ir_expression_is_constant(exp))
    {
        /* can't fold anything */
        return ir_expression(cond);
    }

    assert(IR_IS_BASIC_CONSTANT(exp));
    if (ir_basic_constant_get_bool(ir_basic_constant(exp)))
    {
        return ir_conditional_get_true(cond);
    }
    return ir_conditional_get_false(cond);
}

IrExpression *
cfold_cast(IrCast *cast_exp)
{
    assert(IR_IS_CAST(cast_exp));

    IrExpression *val = ir_cast_get_value(cast_exp);
    DtDataType *val_type = ir_expression_get_data_type(val);
    DtDataType *target_type = ir_cast_get_target_type(cast_exp);

    if (!ir_expression_is_constant(val))
    {
        /* no constant folding possible */
        return ir_expression(cast_exp);
    }

    if (dt_is_basic(val_type))
    {
        return cfold_cast_basic_type(target_type, val_type, val);
    }
    else if (IR_IS_NULL(val))
    {
        return val;
    }
    else if (ir_is_array_literal(val))
    {
        return cfold_cast_array_literal(dt_array(target_type),
                                        ir_array_literal(val));
    }
    /* unexpected target type */
    assert(false);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrExpression *
fold_int_arithm_bin_op(IrBasicConstant *left,
                       IrBasicConstant *right,
                       binary_op_type_t operation)
{
    gint32 left_val;
    gint32 right_val;
    gint32 res;

    left_val = ir_basic_constant_get_int(left);
    right_val = ir_basic_constant_get_int(right);

    switch (operation) {
        case op_plus:
            res = left_val + right_val;
            break;
        case op_minus:
            res = left_val - right_val;
            break;
        case op_mult:
            res = left_val * right_val;
            break;
        case op_division:
            res = left_val / right_val;
            break;
        case op_modulo:
            res = left_val % right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return ir_expression(ir_basic_constant_new_int(res, 0));
}

static IrExpression *
fold_uint_arithm_bin_op(IrBasicConstant *left,
                        IrBasicConstant *right,
                        binary_op_type_t operation)
{
    guint32 left_val;
    guint32 right_val;
    guint32 res;

    left_val = ir_basic_constant_get_uint(left);
    right_val = ir_basic_constant_get_uint(right);

    switch (operation) {
        case op_plus:
            res = left_val + right_val;
            break;
        case op_minus:
            res = left_val - right_val;
            break;
        case op_mult:
            res = left_val * right_val;
            break;
        case op_division:
            res = left_val / right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return ir_expression(ir_basic_constant_new_uint(res, 0));
}

static IrExpression *
fold_int_comp_bin_op(IrBasicConstant *left,
                     IrBasicConstant *right,
                     binary_op_type_t operation)
{
    gint32 left_val;
    gint32 right_val;
    bool res;

    left_val = ir_basic_constant_get_int(left);
    right_val = ir_basic_constant_get_int(right);

    switch (operation) {
        case op_less:           /*  <  */
            res = left_val < right_val;
            break;
        case op_greater:        /*  >  */
            res = left_val > right_val;
            break;
        case op_less_or_eq:     /* <=  */
            res = left_val <= right_val;
            break;
        case op_greater_or_eq:  /* >=  */
            res = left_val >= right_val;
            break;
        case op_equal:          /* ==  */
            res = left_val == right_val;
            break;
        case op_not_equal:      /* !=  */
            res = left_val != right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return ir_expression(ir_basic_constant_new_bool(res, 0));
}

static IrExpression *
fold_uint_comp_bin_op(IrBasicConstant *left,
                      IrBasicConstant *right,
                      binary_op_type_t operation)
{
    guint32 left_val;
    guint32 right_val;
    bool res;

    left_val = ir_basic_constant_get_uint(left);
    right_val = ir_basic_constant_get_uint(right);

    switch (operation) {
        case op_less:           /*  <  */
            res = left_val < right_val;
            break;
        case op_greater:        /*  >  */
            res = left_val > right_val;
            break;
        case op_less_or_eq:     /* <=  */
            res = left_val <= right_val;
            break;
        case op_greater_or_eq:  /* >=  */
            res = left_val >= right_val;
            break;
        case op_equal:          /* ==  */
            res = left_val == right_val;
            break;
        case op_not_equal:      /* !=  */
            res = left_val != right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return ir_expression(ir_basic_constant_new_bool(res, 0));
}

static IrExpression *
cfold_cast_basic_type(DtDataType *target_type,
                      DtDataType *val_type,
                      IrExpression *val)
{
    assert(dt_is_basic(target_type));
    assert(dt_is_basic(val_type));

    if (dt_basic_get_data_type(dt_basic(target_type)) ==
        dt_basic_get_data_type(dt_basic(val_type)))
    {
        return val;
    }

    /*
     * build a new expression, instead of original cast expression
     */
    if (dt_basic_is_int(target_type))
    {
        gint32 res;

        if (dt_basic_is_bool(val_type))
        {
            res = (gint32)ir_basic_constant_get_bool(ir_basic_constant(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint32)ir_basic_constant_get_uint(ir_basic_constant(val));
        }
        else if (dt_basic_is_char(val_type))
        {
            res = (gint32)ir_basic_constant_get_char(ir_basic_constant(val));
        }
        else if (dt_basic_is_ubyte(val_type))
        {
            res = (gint32)ir_basic_constant_get_ubyte(ir_basic_constant(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return ir_expression(ir_basic_constant_new_int(res, 0));
    }
    else if (dt_basic_is_uint(target_type))
    {
        guint32 res;

        if (dt_basic_is_bool(val_type))
        {
            res = (guint32)ir_basic_constant_get_bool(ir_basic_constant(val));
        }
        else if (dt_basic_is_int(val_type))
        {
            res = (guint32)ir_basic_constant_get_int(ir_basic_constant(val));
        }
        else if (dt_basic_is_char(val_type))
        {
            res = (guint32)ir_basic_constant_get_char(ir_basic_constant(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return ir_expression(ir_basic_constant_new_uint(res, 0));
    }
    else if (dt_basic_is_short(target_type))
    {
        gint16 res;

        if (dt_basic_is_int(val_type))
        {
            res = (gint16)ir_basic_constant_get_int(ir_basic_constant(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint16)ir_basic_constant_get_uint(ir_basic_constant(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return ir_expression(ir_basic_constant_new_short(res));
    }
    else if (dt_basic_is_ushort(target_type))
    {
        guint16 res;

        if (dt_basic_is_int(val_type))
        {
            res = (guint16)ir_basic_constant_get_int(ir_basic_constant(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (guint16)ir_basic_constant_get_uint(ir_basic_constant(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return ir_expression(ir_basic_constant_new_ushort(res));
    }
    else if (dt_basic_is_byte(target_type))
    {
        gint8 res;

        if (dt_basic_is_int(val_type))
        {
            res = (gint8)ir_basic_constant_get_int(ir_basic_constant(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint8)ir_basic_constant_get_uint(ir_basic_constant(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return ir_expression(ir_basic_constant_new_byte(res));
    }
    else if (dt_basic_is_ubyte(target_type))
    {
        guint8 res;

        if (dt_basic_is_int(val_type))
        {
            res = (guint8)ir_basic_constant_get_int(ir_basic_constant(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (guint8)ir_basic_constant_get_uint(ir_basic_constant(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return ir_expression(ir_basic_constant_new_ubyte(res));
    }
    else if (dt_basic_is_bool(target_type))
    {
        bool res;

        if (dt_basic_is_int(val_type))
        {
            res = (bool)ir_basic_constant_get_int(ir_basic_constant(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (bool)ir_basic_constant_get_uint(ir_basic_constant(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return ir_expression(ir_basic_constant_new_bool(res, 0));

    }
    else if (dt_basic_is_char(target_type))
    {
        guint8 res;

        if (dt_basic_is_bool(val_type))
        {
            res = (guint8)ir_basic_constant_get_bool(ir_basic_constant(val));
        }
        else if (dt_basic_is_int(val_type))
        {
            res = (guint8)ir_basic_constant_get_int(ir_basic_constant(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint8)ir_basic_constant_get_uint(ir_basic_constant(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return ir_expression(ir_basic_constant_new_char(res, false, 0));

    }
    /* unexpected target type */
    assert(false);
}

static IrExpression *
cfold_cast_array_literal(DtArray *target_type,
                         IrArrayLiteral *array_lit)
{
    DtDataType *element_target_type;
    IrArrayLiteral *new_array_lit;
    GSList *new_array_vals = NULL;
    GSList *i;

    element_target_type = dt_array_get_element_type(target_type);
    /* only arrays literals of basic types supported */
    assert(dt_is_basic(element_target_type));

    new_array_lit = ir_array_literal_new(0);
    for (i = ir_array_literal_get_values(array_lit);
         i != NULL;
         i = g_slist_next(i))
    {
        DtDataType *element_val_type = ir_expression_get_data_type(i->data);

        new_array_vals =
            g_slist_prepend(new_array_vals, 
                            cfold_cast_basic_type(element_target_type,
                                                  element_val_type,
                                                  i->data));
    }
    ir_array_literal_set_values(new_array_lit,
                                g_slist_reverse(new_array_vals));

    /* preserve data section label, if it's set */
    ir_literal_set_data_label(ir_literal(new_array_lit),
                              ir_literal_get_data_label(
                                  ir_literal(array_lit)));

    return ir_expression(new_array_lit);
}
