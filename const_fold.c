#include "const_fold.h"
#include "ir_basic_constant.h"
#include "ir_array_literal.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrExpression *
fold_int_arithm_bin_op(IrBasicConstant *left,
                       IrBasicConstant *right,
                       ast_binary_op_type_t operation);

static IrExpression *
fold_uint_arithm_bin_op(IrBasicConstant *left,
                        IrBasicConstant *right,
                        ast_binary_op_type_t operation);

static IrExpression *
fold_int_comp_bin_op(IrBasicConstant *left,
                     IrBasicConstant *right,
                     ast_binary_op_type_t operation);

static IrExpression *
fold_uint_comp_bin_op(IrBasicConstant *left,
                      IrBasicConstant *right,
                      ast_binary_op_type_t operation);

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
        return cfold_cast(IR_CAST(exp));
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
        return IR_EXPRESSION(bin_op);
    }

    left = cfold(left);
    right = cfold(right);

    bin_op_type = ir_expression_get_data_type(IR_EXPRESSION(bin_op));

    if (dt_basic_is_int(bin_op_type))
    {
        return
            fold_int_arithm_bin_op(IR_BASIC_CONSTANT(left),
                                   IR_BASIC_CONSTANT(right),
                                   ir_binary_operation_get_operation(bin_op));
    }
    else if (dt_basic_is_uint(bin_op_type))
    {
        return
            fold_uint_arithm_bin_op(IR_BASIC_CONSTANT(left),
                                    IR_BASIC_CONSTANT(right),
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
        return IR_EXPRESSION(bin_op);
    }

    assert(
        dt_basic_is_bool(ir_expression_get_data_type(IR_EXPRESSION(bin_op))));

    left_val = ir_basic_constant_get_bool(IR_BASIC_CONSTANT(left));
    right_val = ir_basic_constant_get_bool(IR_BASIC_CONSTANT(right));

    switch (ir_binary_operation_get_operation(bin_op))
    {
        case ast_or_op:
            res = left_val || right_val;
            break;
        case ast_and_op:
            res = left_val && right_val;
            break;
        default:
            /* unexpected binary boolean operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_basic_constant_new_bool(res, 0));
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
        return IR_EXPRESSION(bin_op);
    }

    left_type = ir_expression_get_data_type(left);

    if (dt_basic_is_int(left_type))
    {
        assert(dt_basic_is_int(ir_expression_get_data_type(right)));
        return 
           fold_int_comp_bin_op(IR_BASIC_CONSTANT(left),
                                IR_BASIC_CONSTANT(right),
                                ir_binary_operation_get_operation(bin_op));
    }
    else if (dt_basic_is_uint(left_type))
    {
        assert(dt_basic_is_uint(ir_expression_get_data_type(right)));
        return 
           fold_uint_comp_bin_op(IR_BASIC_CONSTANT(left),
                                 IR_BASIC_CONSTANT(right),
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
        return IR_EXPRESSION(cond);
    }

    assert(IR_IS_BASIC_CONSTANT(exp));
    if (ir_basic_constant_get_bool(IR_BASIC_CONSTANT(exp)))
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
        return IR_EXPRESSION(cast_exp);
    }

    if (dt_is_basic(val_type))
    {
        return cfold_cast_basic_type(target_type, val_type, val);
    }
    else if (ir_is_array_literal(val))
    {
        return cfold_cast_array_literal(DT_ARRAY(target_type),
                                        IR_ARRAY_LITERAL(val));
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
                       ast_binary_op_type_t operation)
{
    gint32 left_val;
    gint32 right_val;
    gint32 res;

    left_val = ir_basic_constant_get_int(left);
    right_val = ir_basic_constant_get_int(right);

    switch (operation) {
        case ast_plus_op:
            res = left_val + right_val;
            break;
        case ast_minus_op:
            res = left_val - right_val;
            break;
        case ast_mult_op:
            res = left_val * right_val;
            break;
        case ast_division_op:
            res = left_val / right_val;
            break;
        case ast_modulo_op:
            res = left_val % right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_basic_constant_new_int(res, 0));
}

static IrExpression *
fold_uint_arithm_bin_op(IrBasicConstant *left,
                        IrBasicConstant *right,
                        ast_binary_op_type_t operation)
{
    guint32 left_val;
    guint32 right_val;
    guint32 res;

    left_val = ir_basic_constant_get_uint(left);
    right_val = ir_basic_constant_get_uint(right);

    switch (operation) {
        case ast_plus_op:
            res = left_val + right_val;
            break;
        case ast_minus_op:
            res = left_val - right_val;
            break;
        case ast_mult_op:
            res = left_val * right_val;
            break;
        case ast_division_op:
            res = left_val / right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_basic_constant_new_uint(res, 0));
}

static IrExpression *
fold_int_comp_bin_op(IrBasicConstant *left,
                     IrBasicConstant *right,
                     ast_binary_op_type_t operation)
{
    gint32 left_val;
    gint32 right_val;
    bool res;

    left_val = ir_basic_constant_get_int(left);
    right_val = ir_basic_constant_get_int(right);

    switch (operation) {
        case ast_less_op:           /*  <  */
            res = left_val < right_val;
            break;
        case ast_greater_op:        /*  >  */
            res = left_val > right_val;
            break;
        case ast_less_or_eq_op:     /* <=  */
            res = left_val <= right_val;
            break;
        case ast_greater_or_eq_op:  /* >=  */
            res = left_val >= right_val;
            break;
        case ast_equal_op:          /* ==  */
            res = left_val == right_val;
            break;
        case ast_not_equal_op:      /* !=  */
            res = left_val != right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_basic_constant_new_bool(res, 0));
}

static IrExpression *
fold_uint_comp_bin_op(IrBasicConstant *left,
                      IrBasicConstant *right,
                      ast_binary_op_type_t operation)
{
    guint32 left_val;
    guint32 right_val;
    bool res;

    left_val = ir_basic_constant_get_uint(left);
    right_val = ir_basic_constant_get_uint(right);

    switch (operation) {
        case ast_less_op:           /*  <  */
            res = left_val < right_val;
            break;
        case ast_greater_op:        /*  >  */
            res = left_val > right_val;
            break;
        case ast_less_or_eq_op:     /* <=  */
            res = left_val <= right_val;
            break;
        case ast_greater_or_eq_op:  /* >=  */
            res = left_val >= right_val;
            break;
        case ast_equal_op:          /* ==  */
            res = left_val == right_val;
            break;
        case ast_not_equal_op:      /* !=  */
            res = left_val != right_val;
            break;
        default:
            /* unexpected int binary operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_basic_constant_new_bool(res, 0));
}

static IrExpression *
cfold_cast_basic_type(DtDataType *target_type,
                      DtDataType *val_type,
                      IrExpression *val)
{
    assert(dt_is_basic(target_type));
    assert(dt_is_basic(val_type));

    if (dt_basic_get_data_type(DT_BASIC(target_type)) ==
        dt_basic_get_data_type(DT_BASIC(val_type)))
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
            res = (gint32)ir_basic_constant_get_bool(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint32)ir_basic_constant_get_uint(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_char(val_type))
        {
            res = (gint32)ir_basic_constant_get_char(IR_BASIC_CONSTANT(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return IR_EXPRESSION(ir_basic_constant_new_int(res, 0));
    }
    else if (dt_basic_is_uint(target_type))
    {
        guint32 res;

        if (dt_basic_is_bool(val_type))
        {
            res = (guint32)ir_basic_constant_get_bool(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_int(val_type))
        {
            res = (guint32)ir_basic_constant_get_int(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_char(val_type))
        {
            res = (guint32)ir_basic_constant_get_char(IR_BASIC_CONSTANT(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return IR_EXPRESSION(ir_basic_constant_new_uint(res, 0));
    }
    else if (dt_basic_is_short(target_type))
    {
        gint16 res;

        if (dt_basic_is_int(val_type))
        {
            res = (gint16)ir_basic_constant_get_int(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint16)ir_basic_constant_get_uint(IR_BASIC_CONSTANT(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return IR_EXPRESSION(ir_basic_constant_new_short(res));
    }
    else if (dt_basic_is_ushort(target_type))
    {
        guint16 res;

        if (dt_basic_is_int(val_type))
        {
            res = (guint16)ir_basic_constant_get_int(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (guint16)ir_basic_constant_get_uint(IR_BASIC_CONSTANT(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return IR_EXPRESSION(ir_basic_constant_new_ushort(res));
    }
    else if (dt_basic_is_byte(target_type))
    {
        gint8 res;

        if (dt_basic_is_int(val_type))
        {
            res = (gint8)ir_basic_constant_get_int(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint8)ir_basic_constant_get_uint(IR_BASIC_CONSTANT(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return IR_EXPRESSION(ir_basic_constant_new_byte(res));
    }
    else if (dt_basic_is_ubyte(target_type))
    {
        guint8 res;

        if (dt_basic_is_int(val_type))
        {
            res = (guint8)ir_basic_constant_get_int(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (guint8)ir_basic_constant_get_uint(IR_BASIC_CONSTANT(val));
        }
        else
        {
            assert(false); /* unexpected value type */
        }
        return IR_EXPRESSION(ir_basic_constant_new_ubyte(res));
    }
    else if (dt_basic_is_bool(target_type))
    {
        bool res;

        if (dt_basic_is_int(val_type))
        {
            res = (bool)ir_basic_constant_get_int(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (bool)ir_basic_constant_get_uint(IR_BASIC_CONSTANT(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return IR_EXPRESSION(ir_basic_constant_new_bool(res, 0));

    }
    else if (dt_basic_is_char(target_type))
    {
        guint8 res;

        if (dt_basic_is_bool(val_type))
        {
            res = (guint8)ir_basic_constant_get_bool(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_int(val_type))
        {
            res = (guint8)ir_basic_constant_get_int(IR_BASIC_CONSTANT(val));
        }
        else if (dt_basic_is_uint(val_type))
        {
            res = (gint8)ir_basic_constant_get_uint(IR_BASIC_CONSTANT(val));
        }
        else
        {
            /* unexpected value type */
            assert(false);
        }
        return IR_EXPRESSION(ir_basic_constant_new_char(res, 0));

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
    ir_literal_set_data_label(IR_LITERAL(new_array_lit),
                              ir_literal_get_data_label(
                                  IR_LITERAL(array_lit)));

    return IR_EXPRESSION(new_array_lit);
}
