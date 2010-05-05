#include <stdbool.h>

#include "sem_analyze_validate.h"
#include "types.h"
#include "dt_auto_type.h"
#include "dt_static_array_type.h"
#include "ir_scalar.h"
#include "ir_array.h"
#include "ir_function.h"
#include "ir_assigment.h"
#include "ir_function_call.h"
#include "ir_return.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_array_slice.h"
#include "ir_array_literal.h"
#include "ir_uint_constant.h"
#include "ir_cast.h"
#include "ir_if_else.h"
#include "ir_while.h"
#include "ir_foreach.h"
#include "ir_array_cell.h"
#include "ir_property.h"
#include "const_fold.h"
#include "errors.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrExpression *
validate_function_call(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrFunctionCall *func_call);

static IrExpression *
validate_bin_arithm(compilation_status_t *compile_status,
                    sym_table_t *sym_table,
                    IrBinaryOperation *bin_op);

static IrExpression *
validate_bin_icomp(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrBinaryOperation *bin_op);

static IrExpression *
validate_bin_conditional(compilation_status_t *compile_status,
                         sym_table_t *sym_table,
                         IrBinaryOperation *bin_op);

static IrExpression *
validate_binary_op(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrBinaryOperation *bin_op);

static IrExpression *
validate_unary_op(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrUnaryOperation *operation);

static IrExpression *
validate_sizeof_property(compilation_status_t *compile_status,
                         sym_table_t *sym_table,
                         IrProperty *prop);

static IrExpression *
validate_init_property(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrProperty *prop);

static IrExpression *
validate_length_property(compilation_status_t *compile_status,
                         sym_table_t *sym_table,
                         IrProperty *prop);

static IrExpression *
validate_property(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrProperty *prop);
static void
validate_function_def(compilation_status_t *compile_status,
                      IrFunctionDef *func_def);

static IrExpression *
validate_expression(compilation_status_t *compile_status,
                    sym_table_t *sym_table,
                    IrExpression *expression);

static IrExpression *
validate_array_slice(compilation_status_t *compile_status,
                     sym_table_t *sym_table,
                     IrArraySlice *array_slice);

static IrExpression *
validate_array_literal(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrArrayLiteral *array_literal);

static IrExpression *
validate_scalar(compilation_status_t *compile_status,
                sym_table_t *sym_table,
                IrScalar *scalar);

static bool
validate_lvalue(compilation_status_t *compile_status,
                sym_table_t *sym_table,
                IrLvalue *lvalue);

static void
validate_return(compilation_status_t *compile_status,
                sym_table_t *sym_table,
                IrReturn *ret);

static void
validate_assigment(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrAssigment *assigment);

static void
validate_if_block(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrIfBlock *if_block);

static void
validate_if_else(compilation_status_t *compile_status,
                 sym_table_t *sym_table,
                 IrIfElse *if_else);

static void
validate_while(compilation_status_t *compile_status,
               sym_table_t *sym_table,
               IrWhile *while_statment);

static void
validate_statment(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrStatment *statment);

static void
validate_code_block(compilation_status_t *compile_status,
                    IrCodeBlock *code_block);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/


static IrExpression *
validate_function_call(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrFunctionCall *func_call)
{
    IrSymbol *func_symb;
    char *func_name;
    DtDataType *func_return_type;
    GSList *formal_args;
    GSList *func_call_args;
    GSList *validated_args = NULL;
    GSList *i;
    GSList *j;
    guint counter;
    GError *error = NULL;


    func_name = ir_function_call_get_name(func_call);

    /* look-up function in the symbol table */
    func_symb = sym_table_get_symbol(sym_table, func_name, &error);
    if (func_symb == NULL)
    {
        GString *err_msg = g_string_new(NULL);
        switch (error->code)
        {
            case SYM_TABLE_SYMBOL_NOT_FOUND_ERROR:
                g_string_append_printf(err_msg,
                                       "reference to unknow function '%s'",
                                       func_name);
                break;
            case SYM_TABLE_MULTIPLE_SYMBOLS_FOUND_ERROR:
                g_string_append_printf(
                    err_msg, "ambiguous function call '%s', matches: %s",
                    func_name, error->message);
                break;
            default:
                /* unexpected error code */
                assert(false);
        }
        compile_error(compile_status,
                      IR_NODE(func_call),
                      "%s\n",
                      err_msg->str);
        g_string_free(err_msg, TRUE);
        g_error_free(error);
        return NULL;
    }
    if (!IR_IS_FUNCTION(func_symb))
    {
        compile_error(compile_status,
                      IR_NODE(func_call),
                      "called object '%s' is not a function\n",
                      func_name);
        return NULL;
    }

    /* store linkage attribute type of the callee */
    ir_function_call_set_linkage(func_call,
                                 ir_function_get_linkage(
                                     IR_FUNCTION(func_symb)));

    formal_args = ir_function_get_parameters(IR_FUNCTION(func_symb));
    func_call_args = ir_function_call_get_arguments(func_call);

    /*
     * check that function call have correct number of arguments
     */
    if (g_slist_length(formal_args) != g_slist_length(func_call_args))
    {
        compile_error(compile_status,
                      IR_NODE(func_call),
                      "invalid call to function '%s()', expected %d "
                      "arguments, got %d\n",
                      func_name,
                      g_slist_length(formal_args),
                      g_slist_length(func_call_args));
        return NULL;
    }

    /*
     * validate function call arguments
     */
    for (i = func_call_args, j = formal_args, counter = 1;
         i != NULL;
         i = g_slist_next(i), j = g_slist_next(j), counter += 1)
    {
        IrExpression *arg_exp;
        IrExpression *exp;
        DtDataType *formal_arg_type;

        assert(j != NULL);

        /* validate expression */
        arg_exp = validate_expression(compile_status,
                                      sym_table,
                                      IR_EXPRESSION(i->data));
        if (arg_exp == NULL)
        {
            /* invalid argument expression, bail out */
            return NULL;
        }

        /* check if the type is compatible */
        if (IR_IS_VARIABLE(j->data))
        {
            formal_arg_type = ir_variable_get_data_type(j->data);
        }
        else
        {
            /* an unnamed function parameter, where only type is specified */
            assert(DT_IS_DATA_TYPE(j->data));
            formal_arg_type = j->data;
        }

        exp = types_implicit_conv(formal_arg_type, arg_exp);
        if (exp == NULL)
        {
            compile_error(compile_status,
                          IR_NODE(arg_exp),
                          "in function call to '%s()', argument %d of invalid type\n",
                          func_name, counter);
            compile_error(compile_status,
                          IR_NODE(arg_exp),
                          "argument's type is '%s' got '%s'\n",
                          dt_data_type_get_string(formal_arg_type),
                          dt_data_type_get_string(ir_expression_get_data_type(arg_exp)));
            return NULL;
        }

        /* valid expression */
        validated_args = g_slist_prepend(validated_args, exp);
    }

    /* store validated call arguments */
    ir_function_call_set_arguments(func_call, g_slist_reverse(validated_args));

    /* store function call data type */
    func_return_type = ir_function_get_return_type(IR_FUNCTION(func_symb));
    ir_function_call_set_return_type(func_call, func_return_type);

    return IR_EXPRESSION(func_call);
}

/**
 * validate binary integer arithmetic operation
 */
static IrExpression *
validate_bin_arithm(compilation_status_t *compile_status,
                    sym_table_t *sym_table,
                    IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_arithm(bin_op));

    IrExpression *exp;


    /*
     * integer promote left operand
     */
    exp = ir_binary_operation_get_left(bin_op);
    exp = types_integer_promotion(exp);
    if (exp == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(ir_binary_operation_get_left(bin_op)),
                      "left operand of illegal type\n");
        return NULL;
    }
    ir_binary_operation_set_left(bin_op, exp);

    /*
     * integer promote right operand
     */
    exp = ir_binary_operation_get_right(bin_op);
    exp = types_integer_promotion(exp);
    if (exp == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(ir_binary_operation_get_right(bin_op)),
                      "right operand of illegal type\n");
        return NULL;
    }
    ir_binary_operation_set_right(bin_op, exp);

    return cfold_bin_arithm(bin_op);
}

/**
 * validate binary integer comparison operation
 */
static IrExpression *
validate_bin_icomp(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_icomp(bin_op));

    IrExpression *converted_left;
    IrExpression *converted_right;

    if (!types_usual_arithm_conv(ir_binary_operation_get_left(bin_op),
                                 ir_binary_operation_get_right(bin_op),
                                 &converted_left,
                                 &converted_right))
    {
        compile_error(compile_status,
                      IR_NODE(bin_op),
                      "illegal types in compare operation\n");
        return NULL;
    }

    ir_binary_operation_set_left(bin_op, converted_left);
    ir_binary_operation_set_right(bin_op, converted_right);

    return cfold_bin_icomp(bin_op);
}

/**
 * validate binary conditional operation
 */
static IrExpression *
validate_bin_conditional(compilation_status_t *compile_status,
                         sym_table_t *sym_table,
                         IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_conditional(bin_op));

    DtDataType *data_type;
    IrExpression *left;
    IrExpression *right;
   
    left = ir_binary_operation_get_left(bin_op);
    right = ir_binary_operation_get_right(bin_op);

    /*
     * check left operand data type
     */
    data_type = ir_expression_get_data_type(left);

    /* left operand can not be of void type */
    if (types_is_void(data_type))
    {
        compile_error(compile_status,
                      IR_NODE(bin_op),
                      "left operand can not be of void type\n");
        return NULL;
    }

    /* if left operand is not bool, cast to bool */
    if (!types_is_bool(data_type))
    {
        left = cfold_cast(ir_cast_new(types_get_bool_type(), left));
    }

    /*
     * check right operand data type
     */
    data_type = ir_expression_get_data_type(right);


    /* if right operand is not of void or bool type, cast to bool */
    if (!types_is_void(data_type) &&
        !types_is_bool(data_type))
    {
        right = cfold_cast(ir_cast_new(types_get_bool_type(), right));
    }

    ir_binary_operation_set_left(bin_op, left);
    ir_binary_operation_set_right(bin_op, right);
    
    return cfold_bin_conditional(bin_op);
}

static IrExpression *
validate_binary_op(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrBinaryOperation *bin_op)
{
    IrExpression *exp;

    /* validate left operand */
    exp = ir_binary_operation_get_left(bin_op);
    exp = validate_expression(compile_status, sym_table, exp);
    if (exp == NULL) {
        return NULL;
    }
    ir_binary_operation_set_left(bin_op, exp);

    /* validate right operand */
    exp = ir_binary_operation_get_right(bin_op);
    exp = validate_expression(compile_status, sym_table, exp);
    if (exp == NULL) {
        return NULL;
    }
    ir_binary_operation_set_right(bin_op, exp);

    /* do operation specific validation */
    if (ir_binary_operation_is_arithm(bin_op))
    {
        return validate_bin_arithm(compile_status, sym_table, bin_op);
    }
    else if (ir_binary_operation_is_icomp(bin_op))
    {
        return validate_bin_icomp(compile_status, sym_table, bin_op);
    }
    else if (ir_binary_operation_is_conditional(bin_op))
    {
        return validate_bin_conditional(compile_status, sym_table, bin_op);
    }

    /* unexpected binary operation type */
    assert(false);
}

static IrExpression *
validate_unary_op(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrUnaryOperation *operation)
{
    IrExpression *exp;

    exp = ir_unary_operation_get_operand(operation);
    exp = validate_expression(compile_status, sym_table, exp);

    switch (ir_unary_operation_get_operation(operation))
    {
        case ast_arithm_neg_op:
            exp = types_implicit_conv(types_get_int_type(), exp);
            if (exp == NULL)
            {
                compile_error(compile_status,
                              IR_NODE(ir_unary_operation_get_operand(operation)),
                              "can not convert to int type\n");
                return NULL;
            }
            break;
        case ast_bool_neg_op:
            exp = types_implicit_conv(types_get_bool_type(), exp);
            if (exp == NULL)
            {
                compile_error(compile_status,
                              IR_NODE(ir_unary_operation_get_operand(operation)),
                              "can not convert to bool type\n");
                return NULL;
            }
            break;
        default:
            /* unexpected unary operation */
            assert(false);
    }

    ir_unary_operation_set_operand(operation, exp);
    return IR_EXPRESSION(operation);
}

static IrExpression *
validate_array_cell(compilation_status_t *compile_status,
                    sym_table_t *sym_table,
                    IrArrayCell *cell)
{
    IrExpression *idx_exp;
    IrSymbol *array_symb;
    DtDataType *symb_type;

    /*
     * look-up the array in the symbol table
     */
    array_symb = sym_table_get_symbol(sym_table,
                                      ir_array_cell_get_name(cell),
                                      NULL);
    if (array_symb == NULL) 
    {
        compile_error(compile_status,
                      IR_NODE(cell),
                      "reference to unknow array symbol '%s'\n",
                      ir_array_cell_get_name(cell));
        return NULL;
    }
    else if (!IR_IS_VARIABLE(array_symb))
    {
        compile_error(compile_status,
                      IR_NODE(cell),
                      "unexpected reference to non variable\n");
        return NULL;
    }

    /* 
     * check that referenced symbol is an static array variable
     */
    symb_type = ir_variable_get_data_type(IR_VARIABLE(array_symb));
    if (!DT_IS_ARRAY_TYPE(symb_type))
    {
        compile_error(compile_status,
                      IR_NODE(cell),
                      "array element expression over non array\n");
        return NULL;
    }
    ir_lvalue_set_variable(IR_LVALUE(cell), IR_VARIABLE(array_symb));

    /*
     * validate array index expression
     */
    idx_exp = ir_array_cell_get_index(cell);
    idx_exp = validate_expression(compile_status,
                                  sym_table,
                                  idx_exp);
    if (idx_exp == NULL)
    {
        /* invalid index expression, bail out */
        return NULL;
    }

    /*
     * implicitly convert index expression to uint type
     */
    idx_exp = types_implicit_conv(types_get_uint_type(), idx_exp);
    if (idx_exp == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(ir_array_cell_get_index(cell)),
                      "illegal index expression type\n");
        return NULL;
    }

    ir_array_cell_set_index(cell, idx_exp);

    return IR_EXPRESSION(cell);
}

static IrExpression *
validate_sizeof_property(compilation_status_t *compile_status,
                         sym_table_t *sym_table,
                         IrProperty *prop)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_PROPERTY(prop));
    assert(ir_property_get_id(prop) == ir_prop_sizeof);

    DtDataType *exp_type;
    IrUintConstant *size_exp;

    exp_type = ir_expression_get_data_type(ir_property_get_expression(prop));

    size_exp = ir_uint_constant_new(dt_data_type_get_size(exp_type),
                                    ir_node_get_line_num(prop));

    return IR_EXPRESSION(size_exp);
}

static IrExpression *
validate_init_property(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrProperty *prop)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_PROPERTY(prop));
    assert(ir_property_get_id(prop) == ir_prop_init);

    DtDataType *exp_type;

    exp_type = ir_expression_get_data_type(ir_property_get_expression(prop));

    /* only .init property on basic type implemented so far */
    assert(DT_IS_BASIC_TYPE(exp_type));
    return types_get_default_initializer(DT_BASIC_TYPE(exp_type));
}

static IrExpression *
validate_length_property(compilation_status_t *compile_status,
                         sym_table_t *sym_table,
                         IrProperty *prop)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_PROPERTY(prop));
    assert(ir_property_get_id(prop) == ir_prop_length);

    DtDataType *exp_type;

    exp_type = ir_expression_get_data_type(ir_property_get_expression(prop));

    if (DT_IS_STATIC_ARRAY_TYPE(exp_type))
    {
        guint32 length;

        length = 
            dt_static_array_type_get_length(DT_STATIC_ARRAY_TYPE(exp_type));

        return IR_EXPRESSION(ir_uint_constant_new(length,
                                                  ir_node_get_line_num(prop)));
    }
    else if (DT_IS_ARRAY_TYPE(exp_type))
    {
        /* run-time expression */
        return IR_EXPRESSION(prop);
    }
    else
    {
        compile_error(compile_status,
                      IR_NODE(prop),
                      "no property 'length' for type '%s'\n",
                      dt_data_type_get_string(exp_type));
        return NULL;
    }
    /* we should not reach here */
    assert(false);
}

static IrExpression *
validate_property(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrProperty *prop)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_PROPERTY(prop));

    IrExpression *exp;

    exp = validate_expression(compile_status,
                              sym_table,
                              ir_property_get_expression(prop));

    if (exp == NULL) {
        /* invalid expression, bail-out */
        return NULL;
    }

    ir_property_set_expression(prop, exp);            

    switch (ir_property_get_id(prop))
    {
        case ir_prop_init:
            return
                validate_init_property(compile_status,
                                       sym_table,
                                       prop);
        case ir_prop_sizeof:
            return
                validate_sizeof_property(compile_status,
                                         sym_table,
                                         prop);
        case ir_prop_length:
            return
                validate_length_property(compile_status,
                                         sym_table,
                                         prop);
        default:
            /* unexpected property id */
            assert(false);
    }

    g_assert_not_reached();
}

static IrExpression *
validate_expression(compilation_status_t *compile_status,
                    sym_table_t *sym_table,
                    IrExpression *expression)
{
    if (IR_IS_BINARY_OPERATION(expression))
    {
        expression =
            validate_binary_op(compile_status,
                               sym_table,
                               IR_BINARY_OPERATION(expression));
    }
    else if (IR_IS_UNARY_OPERATION(expression))
    {
        expression =
            validate_unary_op(compile_status,
                              sym_table,
                              IR_UNARY_OPERATION(expression));
    }
    else if (IR_IS_FUNCTION_CALL(expression))
    {
        expression =
            validate_function_call(compile_status,
                                   sym_table,
                                   IR_FUNCTION_CALL(expression));
    }
    else if (IR_IS_ARRAY_CELL(expression))
    {
        expression =
            validate_array_cell(compile_status,
                                sym_table,
                                IR_ARRAY_CELL(expression));
    }
    else if (IR_IS_ARRAY_LITERAL(expression))
    {
        expression =
            validate_array_literal(compile_status,
                                   sym_table,
                                   IR_ARRAY_LITERAL(expression));
    }
    else if (IR_IS_SCALAR(expression))
    {
        expression =
            validate_scalar(compile_status,
                           sym_table,
                           IR_SCALAR(expression));
    }
    else if (IR_IS_ARRAY_SLICE(expression))
    {
       expression =
           validate_array_slice(compile_status,
                                sym_table,
                                IR_ARRAY_SLICE(expression));
    }
    else if (IR_IS_PROPERTY(expression))
    {
        expression =
            validate_property(compile_status,
                              sym_table,
                              IR_PROPERTY(expression));
    }

    return expression;
}

static void
validate_return(compilation_status_t *compile_status,
                sym_table_t *sym_table,
                IrReturn *ret)
{
    IrExpression *ret_exp;
    DtDataType *func_return_type;

    ret_exp = ir_return_get_return_value(ret);
    func_return_type = ir_function_get_return_type(compile_status->function);

    if (ret_exp != NULL)
    {
        IrExpression *exp;
        IrExpression *conv_exp;

        /* validate return expression */
        exp = validate_expression(compile_status, sym_table, ret_exp);
        if (exp == NULL)
        {
            /* invalid return expression, bail-out */
            return;
        }

        /* make implicit conversion */
        conv_exp = types_implicit_conv(func_return_type, exp);
        if (conv_exp == NULL)
        {
            compile_error(compile_status,
                          IR_NODE(exp),
                          "return expression have incompatible type\n");
            return;
        }
        ir_return_set_return_value(ret, conv_exp);
    }
    else
    {
        /* a return statment without an expression specified */
        if (!types_is_void(func_return_type))
        {
            compile_error(compile_status,
                          IR_NODE(ret),
                          "return expression of type %s expected\n",
                          dt_data_type_get_string(func_return_type));
            return;
        }
    }
}

static void
validate_assigment(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrAssigment *assigment)
{
    IrLvalue *lvalue;
    IrExpression *value;
    IrExpression *converted_value;
    DtDataType *target_type;

    /*
     * validate assigment lvalue
     */
    lvalue = ir_assigment_get_lvalue(assigment);
    lvalue = IR_LVALUE(validate_expression(compile_status,
                                           sym_table,
                                           IR_EXPRESSION(lvalue)));
    if (!IR_IS_LVALUE(lvalue))
    {
        return;
    }
    ir_assigment_set_lvalue(assigment, lvalue);

    /*
     * validate assigment right value
     */
    value = ir_assigment_get_value(assigment);
    value = validate_expression(compile_status, sym_table, value);
    if (value == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(ir_assigment_get_value(assigment)),
                      "invalid assigment expression\n");
        return;
    }

    /*
     * check that lvalue and assign expression have compatible data types
     */
    target_type = ir_expression_get_data_type(IR_EXPRESSION(lvalue));

    converted_value = types_implicit_conv(target_type, value);
    if (converted_value == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(assigment),
                      "incompatible types in assigment to '%s'\n",
                      ir_lvalue_get_name(IR_LVALUE(lvalue)));
        return;
    }

    ir_assigment_set_value(assigment, converted_value);
}

static void
validate_if_block(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrIfBlock *if_block)
{
    IrExpression *condition;
    IrCodeBlock *body;

    /* validate if condition expression */
    condition = ir_if_block_get_condition(if_block);
    condition = validate_expression(compile_status, sym_table, condition);

    /* insert implicit conversion to boolean type */
    condition = types_implicit_conv(types_get_bool_type(),
                                    condition);

    if (condition == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(ir_if_block_get_condition(if_block)),
                      "can not convert if condition to bool type\n");
        return;
    }
    ir_if_block_set_condition(if_block, condition);

    /* validate if body */
    body = ir_if_block_get_body(if_block);
    validate_code_block(compile_status, body);
}

static void
validate_if_else(compilation_status_t *compile_status,
                 sym_table_t *sym_table,
                 IrIfElse *if_else)
{
   GSList *i;
   IrCodeBlock *else_body;
   

   /* validate if-blocks */
   i = ir_if_else_get_if_else_blocks(if_else);
   for (; i != NULL; i = g_slist_next(i))
   {
       validate_if_block(compile_status, sym_table, IR_IF_BLOCK(i->data));
   }

   /* validate else-body */
   else_body = ir_if_else_get_else_body(if_else);
   if (else_body != NULL)
   {
       validate_code_block(compile_status, else_body);
   }
}

static void
validate_while(compilation_status_t *compile_status,
               sym_table_t *sym_table,
               IrWhile *while_statment)
{
    IrExpression *condition;
    IrCodeBlock *body;

    /* validate if condition expression */
    condition = ir_while_get_loop_condition(while_statment);
    condition = validate_expression(compile_status, sym_table, condition);
    if (condition == NULL)
    {
      /* condition expression was invalid, bail out */
      return;
    }

    /* insert implicit conversion to boolean type */
    condition = types_implicit_conv(types_get_bool_type(),
                                    condition);

    if (condition == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(ir_while_get_loop_condition(while_statment)),
                      "can not convert while loop condition to bool type\n");
        return;
    }
    ir_while_set_loop_condition(while_statment, condition);

    /* validate if body */
    body = ir_while_get_body(while_statment);
    validate_code_block(compile_status, body);
}

static void
validate_foreach(compilation_status_t *compile_status,
                 sym_table_t *sym_table,
                 IrForeach *foreach)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_FOREACH(foreach));

    IrArraySlice *aggregate;
    DtDataType *aggr_element_type;
    IrVariable *var;
    DtDataType *var_type;

    /*
     * Validate aggregate expression
     */
    aggregate =
        IR_ARRAY_SLICE(validate_array_slice(compile_status,
                                            sym_table,
                                            ir_foreach_get_aggregate(foreach)));
    if (aggregate == NULL)
    {
        /* invalid aggregate expression */
        return;
    }
    aggr_element_type =
        dt_array_type_get_data_type(
            DT_ARRAY_TYPE(
                ir_expression_get_data_type(IR_EXPRESSION(aggregate))));
    /* only foreach over aggregates over basic types is supported */
    assert(DT_IS_BASIC_TYPE(aggr_element_type));



    /*
     * check value variable's type
     */
    var = ir_foreach_get_value(foreach);
    var_type = ir_variable_get_data_type(var);
    if (DT_IS_AUTO_TYPE(var_type))
    {
        ir_variable_set_data_type(var, aggr_element_type);
    }
    else
    {
        /* only foreach over aggregates over basic types is supported */
        assert(DT_IS_BASIC_TYPE(var_type));
        if (dt_basic_type_get_data_type(DT_BASIC_TYPE(var_type)) !=
            dt_basic_type_get_data_type(DT_BASIC_TYPE(aggr_element_type)))
        {
            compile_error(compile_status,
                          IR_NODE(var),
                          "value variable type does not "
                          "match aggregate element type\n");
            return;
        }
    }

    /*
     * if present, check index variable's type
     */
    var = ir_foreach_get_index(foreach);
    if (var != NULL)
    {
        var_type = ir_variable_get_data_type(var);

        if (DT_IS_AUTO_TYPE(var_type))
        {
            ir_variable_set_data_type(var, types_get_uint_type());
        }
        else if (!types_is_int(var_type) &&
                 !types_is_uint(var_type))
        {
            compile_error(compile_status,
                          IR_NODE(var),
                          "invalid index variable '%s' type,"
                          " expected int or uint\n",
                          ir_variable_get_name(var));
        }
    }

    /*
     * validate foreach body code block
     */
    validate_code_block(compile_status, ir_foreach_get_body(foreach));  
}

static void
validate_statment(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrStatment *statment)
{
    if (IR_IS_FUNCTION_CALL(statment))
    {
         validate_function_call(compile_status,
                                sym_table,
                                IR_FUNCTION_CALL(statment));
    }
    else if (IR_IS_ASSIGMENT(statment))
    {
        validate_assigment(compile_status, sym_table, IR_ASSIGMENT(statment));
    }
    else if (IR_IS_RETURN(statment))
    {
        validate_return(compile_status, sym_table, IR_RETURN(statment));
    }
    else if (IR_IS_IF_ELSE(statment))
    {
        validate_if_else(compile_status, sym_table, IR_IF_ELSE(statment));
    }
    else if (IR_IS_WHILE(statment))
    {
        validate_while(compile_status, sym_table, IR_WHILE(statment));
    }
    else if (IR_IS_FOREACH(statment))
    {
        validate_foreach(compile_status, sym_table, IR_FOREACH(statment));
    }
    else if (IR_IS_EXPRESSION(statment))
    {
        compile_error(compile_status,
                      IR_NODE(statment),
                      "expression have no effect\n");
    }
}

static void
validate_code_block(compilation_status_t *compile_status,
                    IrCodeBlock *code_block)
{
    sym_table_t *sym_table;
    GSList *i;
    GList *locals;
    GList *p;

    sym_table = ir_code_block_get_symbols(code_block);

    /*
     * validate default initializer expressions of local variables
     * in this code block
     */
    locals = sym_table_get_all_symbols(sym_table);
    for (p = locals; p != NULL; p = g_list_next(p))
    {
        IrVariable *var;
        IrExpression *initializer;
        IrExpression *conv_initializer;
        DtDataType *var_type;

        /* skip if not a variable */
        if (!IR_IS_VARIABLE(p->data))
        {
            continue;
        }
        var = IR_VARIABLE(p->data);
        initializer = ir_variable_get_initializer(var);

        /* no initializer expression, skip */
        if (initializer == NULL)
        {
            continue;
        }

        initializer = 
            validate_expression(compile_status, sym_table, initializer);

        if (initializer == NULL)
        {
            /* 
             * initializer expression was invalid,
             * skip to next variable
             */
            continue;
        }

        var_type = ir_variable_get_data_type(var);
        conv_initializer = types_implicit_conv(var_type, initializer);
        if (conv_initializer == NULL)
        {
            compile_error(compile_status,
                          IR_NODE(initializer),
                          "illegal type in initializer assigment\n");
            continue;
        }
        ir_variable_set_initializer(var, conv_initializer);
    }
    g_list_free(locals);

    /*
     * validate statments in the code block
     */
    i = ir_code_block_get_statments(code_block);
    for (; i != NULL; i = g_slist_next(i))
    {
        if (IR_IS_CODE_BLOCK(i->data))
        {
            validate_code_block(compile_status, IR_CODE_BLOCK(i->data));
        }
        else if (IR_IS_STATMENT(i->data))
        {
            validate_statment(compile_status, sym_table, IR_STATMENT(i->data));
        }
        else
        {
            /* unexpected statment type */
            assert(false);
        }
    }
}

static void
validate_function_def(compilation_status_t *compile_status,
                      IrFunctionDef *func_def)
{
    assert(compile_status);
    assert(IR_IS_FUNCTION_DEF(func_def));

    IrCodeBlock *body;

    body = ir_function_def_get_body(func_def);

    /* validate function's body */
    compile_status->function = IR_FUNCTION(func_def);
    validate_code_block(compile_status, body);

    /*
     * For void function, add an implicit return statment if needed
     */
    if (types_is_void(ir_function_def_get_return_type(func_def)))
    {
        GSList *p;
        IrStatment *last_statment = NULL;

        /* look-up last statment in function body */
        p = g_slist_last(ir_code_block_get_statments(body));
        if (p != NULL)
        {
            last_statment = p->data;
        }

        /* if it's not a return, add an implicit return */
        if (!IR_IS_RETURN(last_statment))
        {
            ir_code_block_add_statment(body,
                                       IR_STATMENT(ir_return_new(NULL, 0)));
        }
    }
}

static bool
validate_lvalue(compilation_status_t *compile_status,
                sym_table_t *sym_table,
                IrLvalue *lvalue)
{
    IrSymbol *symb;

    /*
     * look-up the variable in the symbol table
     */
    symb = sym_table_get_symbol(sym_table,
                                ir_lvalue_get_name(lvalue),
                                NULL);
    if (symb == NULL) 
    {
        compile_error(compile_status,
                      IR_NODE(lvalue),
                      "reference to unknow symbol '%s'\n",
                      ir_lvalue_get_name(lvalue));
        return NULL;
    }
    else if (!IR_IS_VARIABLE(symb))
    {
        compile_error(compile_status,
                      IR_NODE(lvalue),
                      "can not assign value to '%s', not a variable\n",
                      ir_lvalue_get_name(lvalue));

        return NULL;
    }

    ir_lvalue_set_variable(lvalue, IR_VARIABLE(symb));

    return IR_EXPRESSION(lvalue);
}

static IrExpression *
validate_scalar(compilation_status_t *compile_status,
                sym_table_t *sym_table,
                IrScalar *scalar)
{
    if (!validate_lvalue(compile_status,
                         sym_table,
                         IR_LVALUE(scalar)))
    {
       return NULL;
    }

    return IR_EXPRESSION(scalar);
}



static IrExpression *
validate_array_slice(compilation_status_t *compile_status,
                     sym_table_t *sym_table,
                     IrArraySlice *array_slice)
{
    IrVariable *var;
    DtDataType *var_dt;
    IrExpression *exp;

    if (!validate_lvalue(compile_status,
                         sym_table,
                         IR_LVALUE(array_slice)))
    {
        return NULL;
    }

    /*
     * check that array slice expression symbol is of array type
     */
    var = ir_lvalue_get_variable(IR_LVALUE(array_slice));
    var_dt = ir_variable_get_data_type(var);

    if (!DT_IS_ARRAY_TYPE(var_dt))
    {
        compile_error(compile_status,
                      IR_NODE(array_slice),
                      "illegal array slice expression over "
                      "non-array variable\n");
        return NULL;
    }

    /*
     * validate start expression
     */
    exp = ir_array_slice_get_start(array_slice);
    if (exp == NULL)
    {
        exp = IR_EXPRESSION(ir_uint_constant_new(0, 0));
    }
    else
    {
        exp = validate_expression(compile_status, sym_table, exp);
        if (exp == NULL)
        {
            /* invalid start expression */
            return false;
        }
        exp = types_implicit_conv(types_get_uint_type(), exp);
        if (exp == NULL)
        {
            compile_error(compile_status,
                          IR_NODE(array_slice),
                          "cannot implicitly convert array slice start"
                          " expression to uint type\n");
            return NULL;
        }
    }
    ir_array_slice_set_start(array_slice, exp);

    /*
     * validate end expression
     */
    exp = ir_array_slice_get_end(array_slice);
    if (exp == NULL)
    {
        if (DT_IS_STATIC_ARRAY_TYPE(var_dt))
        {
          /* slice over static array */
          guint32 end_idx;

          end_idx =
              dt_static_array_type_get_length(DT_STATIC_ARRAY_TYPE(var_dt));
          exp = IR_EXPRESSION(ir_uint_constant_new(end_idx, 0));
        }
        else
        {
           /* slice over dynamic array */
           exp = IR_EXPRESSION(ir_property_new(IR_EXPRESSION(array_slice),
                                               IR_PROP_LENGTH, 0));
        }
    }
    else
    {
        exp = validate_expression(compile_status, sym_table, exp);
        if (exp == NULL)
        {
            /* invalid end expression */
            return false;
        }
        exp = types_implicit_conv(types_get_uint_type(), exp);
        if (exp == NULL)
        {
            compile_error(compile_status,
                          IR_NODE(array_slice),
                          "cannot implicitly convert array slice end"
                          " expression to uint type\n");
            return NULL;
        }
    }
    ir_array_slice_set_end(array_slice, exp);

    return IR_EXPRESSION(array_slice);
}

static IrExpression *
validate_array_literal(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrArrayLiteral *array_literal)
{
    GSList *i;
    GSList *validated_values = NULL;
    DtDataType *vals_trgt_type = NULL;

    i = ir_array_literal_get_values(array_literal);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrExpression *exp;

        exp = validate_expression(compile_status,
                                  sym_table,
                                  i->data);
        if (exp == NULL)
        {
           compile_error(compile_status,
                         IR_NODE(i->data),
                         "invalid array literal value expression\n");
           return NULL;
        }

        if (vals_trgt_type == NULL)
        {
            vals_trgt_type = ir_expression_get_data_type(exp);
        }
        else
        {
            /*
             * type cast all values in array literal expression to
             * the type of the first expression
             */
            exp = types_implicit_conv(vals_trgt_type, exp);
        }

        if (exp == NULL)
        {
            /* illegal implicit conversation */
            compile_error(compile_status,
                          IR_NODE(i->data),
                          "can not implicitly convert array literal value\n");
            return NULL;
        }

        validated_values = g_slist_prepend(validated_values, exp);
    }

    ir_array_literal_set_values(array_literal, 
                                g_slist_reverse(validated_values));

    if (ir_expression_is_constant(IR_EXPRESSION(array_literal)))
    {
        /*
         * a compile-time constant array literal expression, add it
         * to module's data section
         */
        ir_module_add_array_literal_data(compile_status->module,
                                         array_literal);
    }
    return IR_EXPRESSION(array_literal);
}

static void
validate_entry_point(compilation_status_t *compile_status,
                     IrModule *module)
{
    assert(compile_status);
    assert(IR_IS_MODULE(module));

    sym_table_t *sym_table;
    IrSymbol *main_symb;
    IrFunction *main_func;
    DtDataType *main_ret_type;

    sym_table = ir_module_get_symbols(module);
    main_symb = sym_table_get_symbol(sym_table, "main", NULL);

    if (!IR_IS_FUNCTION(main_symb))
    {
        /* no entry point function defined */
        return;
    }

    main_func = IR_FUNCTION(main_symb);
    if (ir_function_get_parameters(main_func) != NULL)
    {
        compile_error(compile_status,
                      IR_NODE(main_func),
                      "only void main() and int main() "
                      "entry points supported\n");
        return;
    }

    main_ret_type = ir_function_get_return_type(main_func);
    if (!types_is_void(main_ret_type) &&
        !types_is_int(main_ret_type))
    {
       compile_error(compile_status,
                     IR_NODE(main_func),
                     "function main() must return int or void\n");
    }
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
sem_analyze_validate(compilation_status_t *compile_status,
                     IrModule *module)
{
    assert(compile_status);
    assert(IR_IS_MODULE(module));

    GSList *i;

    compile_status->module = module;

    validate_entry_point(compile_status, module);

    i = ir_module_get_function_defs(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_FUNCTION_DEF(i->data));
        validate_function_def(compile_status, IR_FUNCTION_DEF(i->data));
    }
}

