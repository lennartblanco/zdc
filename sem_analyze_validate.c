#include <stdbool.h>

#include "sem_analyze_validate.h"
#include "dt_name.h"
#include "dt_pointer.h"
#include "types.h"
#include "dt_auto.h"
#include "dt_static_array.h"
#include "dt_pointer.h"
#include "ir_function.h"
#include "ir_assignment.h"
#include "ir_function_call.h"
#include "ir_return.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_conditional.h"
#include "ir_array_slice.h"
#include "ir_array_literal.h"
#include "ir_basic_constant.h"
#include "ir_cast.h"
#include "ir_if_else.h"
#include "ir_while.h"
#include "ir_foreach.h"
#include "ir_break.h"
#include "ir_array_cell.h"
#include "ir_property.h"
#include "ir_enum_member.h"
#include "ir_dot.h"
#include "ir_ptr_dref.h"
#include "ir_ident.h"
#include "ir_to_iml.h"
#include "const_fold.h"
#include "errors.h"
#include "iml_constant.h"

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

static void
validate_return(compilation_status_t *compile_status,
                sym_table_t *sym_table,
                IrReturn *ret);

static void
validate_assignment(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrAssignment *assignment);

static void
validate_if_block(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrIfBlock *if_block,
                  iml_operation_t *end_label);

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

static void
validate_enum(compilation_status_t *compile_status,
              IrEnum *enum_def);

/**
 * Look-up the user defined data type in current module. If no data type
 * with provided name is found, a compile error is printed.
 *
 * @param compile_status the lexical scope where to look for the type
 * @param user_type the placeholder object for user defined type
 *
 * @return the data type found or NULL if failed to look-up data type
 */
static DtDataType *
resolve_user_type(compilation_status_t *compile_status,
                  DtName *user_type);

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
 * validate pointer arithmetic binary expression
 */
static IrExpression *
validate_pointer_arithm(compilation_status_t *compile_status,
                        sym_table_t *sym_table,
                        IrBinaryOperation *bin_op)
{
    IrExpression *left;
    IrExpression *right;
    DtDataType *left_type;
    DtDataType *right_type;

    ast_binary_op_type_t op = ir_binary_operation_get_operation(bin_op);
    left = ir_binary_operation_get_left(bin_op);
    right = ir_binary_operation_get_right(bin_op);

    if (!types_pointer_arithm_conv(left, op, right, &left, &right))
    {
        compile_error(compile_status,
                      bin_op,
                      "illegal types for pointer arithmetic operation\n");
        return NULL;
    }

    left_type = ir_expression_get_data_type(left);
    right_type = ir_expression_get_data_type(right);

    /* now we know that left operand is of pointer type */
    IrBasicConstant *base_type_size =
        ir_basic_constant_new_uint(
            dt_pointer_get_base_type_size(DT_POINTER(left_type)),
            0);

    if (op == ast_plus_op)
    {
        right =
            IR_EXPRESSION(
                ir_binary_operation_new(ast_mult_op,
                                        right,
                                        IR_EXPRESSION(base_type_size),
                                        0));
        right =
            validate_bin_arithm(compile_status,
                                sym_table,
                                IR_BINARY_OPERATION(right));
    }
    else
    {
        assert(op == ast_minus_op);

        if (DT_IS_POINTER(right_type))
        {
            left =
                IR_EXPRESSION(
                    ir_binary_operation_new(ast_minus_op, left, right, 0));
            right = IR_EXPRESSION(base_type_size);
            bin_op = ir_binary_operation_new(ast_division_op, left, right, 0);
        }
        else
        {
            right =
                IR_EXPRESSION(
                    ir_binary_operation_new(ast_mult_op,
                                            right,
                                            IR_EXPRESSION(base_type_size),
                                            0));
            right =
                validate_bin_arithm(compile_status,
                                    sym_table,
                                    IR_BINARY_OPERATION(right));
        }
    }

    ir_binary_operation_set_left(bin_op, left);
    ir_binary_operation_set_right(bin_op, right);

    return cfold_bin_arithm(bin_op);
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

    IrExpression *left;
    IrExpression *right;

    left = ir_binary_operation_get_left(bin_op);
    right = ir_binary_operation_get_right(bin_op);

    if (DT_IS_POINTER(ir_expression_get_data_type(left)) ||
        DT_IS_POINTER(ir_expression_get_data_type(right)))
    {
        return validate_pointer_arithm(compile_status,
                                       sym_table,
                                       bin_op);
    }

    if (!types_usual_arithm_conv(left, right, &left, &right))
    {
        compile_error(compile_status,
                      bin_op,
                      "illegal types in arithmetic operation\n");
        return NULL;
    }

    ir_binary_operation_set_left(bin_op, left);
    ir_binary_operation_set_right(bin_op, right);

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

    IrExpression *left;
    IrExpression *right;
    IrExpression *converted_left;
    IrExpression *converted_right;
    DtDataType *left_type;
    DtDataType *right_type;

    left = ir_binary_operation_get_left(bin_op);
    right = ir_binary_operation_get_right(bin_op);

    left_type = ir_expression_get_data_type(left);
    right_type = ir_expression_get_data_type(right);

    if (DT_IS_POINTER(left_type) || DT_IS_POINTER(right_type))
    {
        /* when pointers are compared, data types must match */
        if (!dt_data_type_is_same(left_type, right_type))
        {
            compile_error(compile_status,
                          bin_op,
                          "invalid pointers comparison\n");
            return NULL;
        }
    }
    else
    {
        if (!types_usual_arithm_conv(left,
                                     right,
                                     &converted_left,
                                     &converted_right))
        {
            compile_error(compile_status,
                          bin_op,
                          "illegal types in compare operation\n");
            return NULL;
        }

        ir_binary_operation_set_left(bin_op, converted_left);
        ir_binary_operation_set_right(bin_op, converted_right);
    }

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
validate_ind_dec_ops(compilation_status_t *compile_status,
                     sym_table_t *sym_table,
                     IrExpression *exp)
{
    if (!ir_expression_is_lvalue(exp))
    {
        compile_error(compile_status,
                      exp,
                      "must be an lvalue\n");
        goto invalid_exp;
    }

    DtDataType *exp_type = ir_expression_get_data_type(exp);

    if (dt_is_basic(exp_type))
    {
        switch (dt_basic_get_data_type(DT_BASIC(exp_type)))
        {
            case char_type:
            case int_type:
            case uint_type:
                goto valid_exp;
            default:
                goto invalid_type;
        }
    }
    else if (DT_IS_POINTER(exp_type))
    {
        goto valid_exp;
    }
    else
    {
        /* any non-basic type convertible to int is valid */
        if (types_implicit_conv(types_get_int_type(), exp) == NULL)
        {
            goto invalid_type;
        }
    }

valid_exp:
    return exp;

invalid_type:
    compile_error(compile_status,
                  exp,
                  "operation not allowed on '%s' type\n",
                  dt_data_type_get_string(exp_type));
invalid_exp:
    return NULL;
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
        case ast_pre_inc_op:
        case ast_pre_dec_op:
        case ast_post_inc_op:
        case ast_post_dec_op:
            exp = validate_ind_dec_ops(compile_status, sym_table, exp);
            if (exp == NULL)
            {
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
    IrVariable *array_symb;
    DtDataType *symb_type;

    array_symb = ir_array_cell_get_symbol(cell);
    /*
     * check that referenced symbol is an array variable
     */
    symb_type = ir_expression_get_data_type(IR_EXPRESSION(array_symb));
    if (!DT_IS_ARRAY(symb_type))
    {
        compile_error(compile_status,
                      IR_NODE(cell),
                      "'%s' must be of array type, not %s\n",
                      ir_variable_get_name(array_symb),
                      dt_data_type_get_string(symb_type));
        return NULL;
    }

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
    IrBasicConstant *size_exp;

    exp_type = ir_expression_get_data_type(ir_property_get_expression(prop));

    size_exp = ir_basic_constant_new_uint(dt_data_type_get_size(exp_type),
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

    return dt_data_type_get_init(exp_type);
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

    IrExpression *exp;
    DtDataType *exp_type;

    exp = ir_property_get_expression(prop);
    exp_type = ir_expression_get_data_type(exp);

    if (IR_IS_ENUM_MEMBER(exp))
    {
      exp = ir_enum_member_get_value(IR_ENUM_MEMBER(exp));
    }

    if (ir_is_array_literal(exp))
    {
        guint32 length;

        length = ir_array_literal_get_length(IR_ARRAY_LITERAL(exp));

        return
            IR_EXPRESSION(
                ir_basic_constant_new_int(length,
                                          ir_node_get_line_num(prop)));
    }
    else if (DT_IS_STATIC_ARRAY_TYPE(exp_type))
    {
        guint32 length;

        length = 
            dt_static_array_get_length(DT_STATIC_ARRAY(exp_type));

        return
            IR_EXPRESSION(
                ir_basic_constant_new_int(length,
                                          ir_node_get_line_num(prop)));
    }
    else if (DT_IS_ARRAY(exp_type))
    {
        /* run-time expression */
        return IR_EXPRESSION(prop);
    }
    else
    {
        compile_error(compile_status,
                      prop,
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
validate_dot_property(compilation_status_t *compile_status,
                      sym_table_t *sym_table,
                      IrExpression *left,
                      IrExpression *right)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_EXPRESSION(left));
    /* only identifiers supported as right operand to '.' operation */
    assert(IR_IS_IDENT(right));

    IrProperty *prop =
        ir_property_new(left,
                        ir_ident_get_name(IR_IDENT(right)),
                        ir_node_get_line_num(IR_NODE(right)));

    if (prop == NULL)
    {
        compile_error(compile_status,
                      right,
                      "unknown property '%s'\n",
                      ir_ident_get_name(IR_IDENT(right)));
        return NULL;
    }
    return IR_EXPRESSION(prop);
}

static IrExpression *
validate_dot_variable(compilation_status_t *compile_status,
                      sym_table_t *sym_table,
                      IrExpression *left,
                      IrExpression *right)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_VARIABLE(left));
    /* only identifiers supported as right operand to '.' operation */
    assert(IR_IS_IDENT(right));

    DtDataType *type = ir_expression_get_data_type(left);
    if (DT_IS_POINTER(type) &&
        DT_IS_STRUCT(dt_pointer_get_base_type(DT_POINTER(type))))
    {
        DtStruct *dt_struct;
        IrStructMember *mbr;

        dt_struct = DT_STRUCT(dt_pointer_get_base_type(DT_POINTER(type)));
        mbr = dt_struct_get_member(dt_struct,
                                   ir_ident_get_name(IR_IDENT(right)));
        if (mbr != NULL)
        {
            ir_struct_member_set_base(mbr, left);
            return IR_EXPRESSION(mbr);
        }
    }

    return validate_dot_property(compile_status, sym_table, left, right);
}

static IrExpression *
validate_dot(compilation_status_t *compile_status,
             sym_table_t *sym_table,
             IrDot *dot)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_DOT(dot));

    IrExpression *left = ir_dot_get_left(dot);
    IrExpression *right = ir_dot_get_right(dot);
    IrExpression *res;

    /* only identifiers supported as right operand to '.' operation */
    assert(IR_IS_IDENT(right));

    if (IR_IS_ENUM(left))
    {
        IrEnumMember *mbr;

        mbr = ir_enum_get_member(IR_ENUM(left),
                                 ir_ident_get_name(IR_IDENT(right)));
        if (mbr == NULL)
        {
            res = validate_dot_property(compile_status,
                                        sym_table,
                                        left,
                                        right);
        }
        else
        {
            res = IR_EXPRESSION(mbr);
        }
    }
    else if (IR_IS_VARIABLE(left))
    {
        res = validate_dot_variable(compile_status, sym_table, left, right);
    }
    else
    {
        res = validate_dot_property(compile_status, sym_table, left, right);
    }

    if (res != NULL)
    {
        res = validate_expression(compile_status, sym_table, res);
    }
    return res;
}

static IrExpression *
validate_ptr_dref(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrPtrDref *ptr_dref)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_PTR_DREF(ptr_dref));

    IrExpression *exp = ir_ptr_dref_get_expression(ptr_dref);

    /* validate pointer expression */
    exp = validate_expression(compile_status, sym_table, exp);
    if (exp == NULL)
    {
        /* invalid expression dereferenced */
        return NULL;
    }
    ir_ptr_dref_set_expression(ptr_dref, exp);

    /* check that dereferenced expression is of pointer type */
    if (!DT_IS_POINTER(ir_expression_get_data_type(exp)))
    {
        compile_error(compile_status,
                      ptr_dref,
                      "can't dereference non-pointer expression\n");
        return NULL;
    }

    return IR_EXPRESSION(ptr_dref);
}

static IrExpression *
validate_conditional(compilation_status_t *compile_status,
                     sym_table_t *sym_table,
                     IrConditional *cond)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_CONDITIONAL(cond));

    IrExpression *eval;
    IrExpression *true_exp;
    IrExpression *false_exp;

    eval = validate_expression(compile_status,
                               sym_table,
                               ir_conditional_get_cond(cond));
    true_exp = validate_expression(compile_status,
                                   sym_table,
                                   ir_conditional_get_true(cond));
    false_exp = validate_expression(compile_status,
                                    sym_table,
                                    ir_conditional_get_false(cond));

    if (eval == NULL || true_exp == NULL || false_exp == NULL)
    {
        /* invalid subexpressions, bail out */
        return NULL;
    }

    /* check that eval condition can be converted to bool type */
    IrExpression *temp = eval;
    eval = types_implicit_conv(types_get_bool_type(), eval);
    if (eval == NULL)
    {
        compile_error(compile_status,
                      temp,
                      "can not convert '%s' to bool type\n",
                      dt_data_type_get_string(
                          ir_expression_get_data_type(temp)));
        return NULL;
    }

    /*
     * figure out the data type for this conditional expression
     */
    DtDataType *true_type = ir_expression_get_data_type(true_exp);
    DtDataType *false_type = ir_expression_get_data_type(false_exp);
    DtDataType *common = types_find_common(true_type, false_type);

    if (common == NULL)
    {
        compile_error(compile_status,
                      cond,
                      "incompatible types for conditional '%s' and '%s'\n",
                      dt_data_type_get_string(true_type),
                      dt_data_type_get_string(false_type));
        return NULL;
    }

    /*
     * insert casts if needed
     */
    if (!dt_data_type_is_same(common, true_type))
    {
        true_exp = IR_EXPRESSION(ir_cast_new(common, true_exp));
    }

    if (!dt_data_type_is_same(common, false_type))
    {
        false_exp = IR_EXPRESSION(ir_cast_new(common, false_exp));
    }


    ir_conditional_set_expressions(cond, eval, true_exp, false_exp);

    return cfold_conditional(cond);
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
    else if (ir_is_array_literal(expression))
    {
        expression =
            validate_array_literal(compile_status,
                                   sym_table,
                                   IR_ARRAY_LITERAL(expression));
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
    else if (IR_IS_DOT(expression))
    {
        expression =
            validate_dot(compile_status, sym_table, IR_DOT(expression));
    }
    else if (IR_IS_PTR_DREF(expression))
    {
        expression =
            validate_ptr_dref(compile_status,
                              sym_table,
                              IR_PTR_DREF(expression));
    }
    else if (IR_IS_CONDITIONAL(expression))
    {
        expression =
            validate_conditional(compile_status,
                                 sym_table,
                                 IR_CONDITIONAL(expression));
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
    func_return_type =
        ir_function_get_return_type(IR_FUNCTION(compile_status->function));

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
                          ret,
                          "return expression have incompatible type\n");
            return;
        }

        /*
         * valid return statement, add iml operations
         */
        ImlOperand *ret_val = iml_add_expression_eval(compile_status->function,
                                                      conv_exp,
                                                      NULL);
        ir_function_def_add_operation(compile_status->function,
                                  iml_operation_new(iml_return, ret_val));
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

        /* valid return from void function, add iml-operation */
        ir_function_def_add_operation(compile_status->function,
                                  iml_operation_new(iml_return, NULL));
    }
}

static void
validate_assignment(compilation_status_t *compile_status,
                   sym_table_t *sym_table,
                   IrAssignment *assignment)
{
    IrExpression *lvalue;
    IrExpression *value;
    IrExpression *converted_value;
    DtDataType *target_type;

    /*
     * validate assignment lvalue
     */
    lvalue = ir_assignment_get_lvalue(assignment);
    lvalue = validate_expression(compile_status, sym_table, lvalue);
    if (lvalue == NULL)
    {
        /* invalid lvalue expression, bail-out */
        return;
    }
    if (!ir_expression_is_lvalue(lvalue))
    {
        compile_error(compile_status,
                      assignment,
                      "invalid lvalue in assignment\n");
        return;
    }
    ir_assignment_set_lvalue(assignment, lvalue);

    /*
     * validate assignment right value
     */
    value = ir_assignment_get_value(assignment);
    value = validate_expression(compile_status, sym_table, value);
    if (value == NULL)
    {
        compile_error(compile_status,
                      ir_assignment_get_value(assignment),
                      "invalid assignment expression\n");
        return;
    }

    /*
     * check that lvalue and assign expression have compatible data types
     */
    target_type = ir_expression_get_data_type(IR_EXPRESSION(lvalue));
    if (dt_data_type_is_immutalbe(target_type))
    {
        compile_error(compile_status,
                      IR_NODE(assignment),
                      "can not modify immutable lvalue\n");
        return;
    }

    converted_value = types_implicit_conv(target_type, value);
    if (converted_value == NULL && DT_IS_ARRAY(target_type))
    {
        /*
         * Handle the special case of array assignments from a scalar,
         * check if scalar can be implicitly converted to array's
         * element data type
         */
        DtDataType *array_element_type;

        array_element_type =
            dt_array_get_element_type(DT_ARRAY(target_type));
        converted_value = types_implicit_conv(array_element_type, value);
    }
    if (converted_value == NULL)
    {
        compile_error(
            compile_status,
            IR_NODE(assignment),
            "can't assign expression of type %s to lvalue of %s type\n",
            dt_data_type_get_string(ir_expression_get_data_type(value)),
            dt_data_type_get_string(target_type));

        return;
    }

    /* valid assignment, add iml operations */
    iml_add_assignment(compile_status->function, lvalue, converted_value);
}

/**
 * @param end_label if not NULL, inserts a jump operation as the
 *                  last statment of the if-blocks body
 */
static void
validate_if_block(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrIfBlock *if_block,
                  iml_operation_t *end_label)
{
    IrExpression *condition;
    IrCodeBlock *body;
    ImlOperand *condition_eval_res;
    iml_operation_t *skip_label;

    /* validate if condition expression */
    condition = ir_if_block_get_condition(if_block);
    condition = validate_expression(compile_status, sym_table, condition);
    if (condition == NULL)
    {
        /* invalid condition expression */
        return;
    }

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

    /* generate iml operation for validation of condition expression */
    condition_eval_res = iml_add_expression_eval(compile_status->function,
                                                 condition,
                                                 NULL);

    /*
     * add conditional jump operation if
     * the condition expression evaluates for false
     */
    skip_label =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    ir_function_def_add_operation(
            compile_status->function,
            iml_operation_new(iml_jmpneq,
                              condition_eval_res,
                              iml_constant_new_8b(1),
                              iml_operation_get_operand(skip_label, 1)));

    /* mark condition result operand as unused */
    iml_func_frame_unused_oper(
            ir_function_def_get_frame(compile_status->function),
            condition_eval_res);

    /* validate if body */
    body = ir_if_block_get_body(if_block);
    validate_code_block(compile_status, body);

    /* add jump operation past the rest of if-else bodies if needed */
    if (end_label != NULL)
    {
        ir_function_def_add_operation(
                compile_status->function,
                iml_operation_new(iml_jmp,
                                  iml_operation_get_operand(end_label, 1)));
    }

    /* insert the skip label */
    ir_function_def_add_operation(compile_status->function, skip_label);
}

static void
validate_if_else(compilation_status_t *compile_status,
                 sym_table_t *sym_table,
                 IrIfElse *if_else)
{
   GSList *i;
   IrCodeBlock *else_body;
   iml_operation_t *end_label = NULL;

   i = ir_if_else_get_if_else_blocks(if_else);
   else_body = ir_if_else_get_else_body(if_else);

   /* if there is any 'else-if' or 'else' statments, generate an end label */
   if (g_slist_next(i) != NULL || else_body != NULL)
   {
     end_label =
         iml_operation_new(iml_label,
                           ir_module_gen_label(compile_status->module));
   }

   /* validate if-blocks */
   for (; i != NULL; i = g_slist_next(i))
   {
       validate_if_block(compile_status,
                         sym_table,
                         IR_IF_BLOCK(i->data),
                         end_label);
   }

   /* validate else-body */
   if (else_body != NULL)
   {
       validate_code_block(compile_status, else_body);
   }

   /* insert end label if needed */
   if (end_label != NULL)
   {
       ir_function_def_add_operation(compile_status->function, end_label);
   }
}

static void
validate_while(compilation_status_t *compile_status,
               sym_table_t *sym_table,
               IrWhile *while_statment)
{
    IrExpression *condition;
    ImlOperand *condition_eval_res;
    iml_operation_t *loop_start;
    iml_operation_t *loop_end;

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

    /* generate iml labels for start and end of the loop operations */
    loop_start =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    loop_end =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    ir_loop_set_exit_label(IR_LOOP(while_statment), loop_end);

    /* label the start of loop */
    ir_function_def_add_operation(compile_status->function, loop_start);

    /* figure out the jump operation to issue after condition evaluation */
    iml_operation_t *jump_op;
    if (ir_expression_is_constant(condition))
    {
        /*
         * a constant expression,
         * this is either an eternal loop or the loop which is always skipped
         */
        assert(IR_IS_BASIC_CONSTANT(condition));
        if (ir_basic_constant_get_bool(IR_BASIC_CONSTANT(condition)))
        {
            /* eternal loop, no jump instruction needed */
            jump_op = NULL;
        }
        else
        {
            /* just skip the loop body */
            jump_op =
                iml_operation_new(iml_jmp,
                                  iml_operation_get_operand(loop_end, 1));
        }
    }
    else
    {
        /* insert iml operation for validation of loop condition */
        condition_eval_res = iml_add_expression_eval(compile_status->function,
                                                     condition,
                                                     NULL);
        /* jump past the loop body if condition evaluates to false */
        jump_op =
            iml_operation_new(iml_jmpneq,
                              condition_eval_res,
                              iml_constant_new_8b(1),
                              iml_operation_get_operand(loop_end, 1));
    }

    /* add jump operation if needed */
    if (jump_op != NULL)
    {
        ir_function_def_add_operation(compile_status->function, jump_op);
    }

    /*
     * validate while body, and generate iml operations
     */
    IrLoop *prev_loop = compile_status->loop;
    compile_status->loop = IR_LOOP(while_statment);

    validate_code_block(compile_status, ir_while_get_body(while_statment));

    compile_status->loop = prev_loop;

    /* jump to loop start */
    ir_function_def_add_operation(
            compile_status->function,
            iml_operation_new(iml_jmp,
                              iml_operation_get_operand(loop_start, 1)));

    /* and loop end label */
    ir_function_def_add_operation(compile_status->function, loop_end);
}

static void
validate_foreach(compilation_status_t *compile_status,
                 sym_table_t *sym_table,
                 IrForeach *foreach)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_FOREACH(foreach));

    IrExpression *aggregate;
    DtDataType *aggr_type;
    DtDataType *aggr_element_type;
    IrVariable *var;
    DtDataType *var_type;
    ImlVariable *index;
    ImlVariable *length;
    iml_operation_t *loop_label;

    /*
     * Validate aggregate expression
     */
    aggregate = validate_expression(compile_status,
                                    sym_table,
                                    ir_foreach_get_aggregate(foreach));
    if (aggregate == NULL)
    {
        /* invalid aggregate expression */
        return;
    }
    aggr_type = ir_expression_get_data_type(aggregate);
    if (!DT_IS_ARRAY(aggr_type))
    {
        compile_error(compile_status,
                      foreach,
                      "'%s' is not an aggregate type\n",
                      dt_data_type_get_string(aggr_type));
        return;
    }
    aggr_element_type = dt_array_get_element_type(DT_ARRAY(aggr_type));
    /* only foreach over aggregates over basic types is supported */
    assert(dt_is_basic(aggr_element_type));

    /*
     * check value variable's type
     */
    var = ir_foreach_get_value(foreach);
    var_type = ir_variable_get_data_type(var);
    if (DT_IS_AUTO(var_type))
    {
        ir_variable_set_data_type(var, aggr_element_type);
    }
    else
    {
        /* only foreach over aggregates over basic types is supported */
        assert(dt_is_basic(var_type));
        if (dt_basic_get_data_type(DT_BASIC(var_type)) !=
            dt_basic_get_data_type(DT_BASIC(aggr_element_type)))
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

        if (DT_IS_AUTO(var_type))
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
            return;
        }
    }

    /* generate and store loop exit label */
    iml_operation_t *exit_label =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    ir_loop_set_exit_label(IR_LOOP(foreach), exit_label);

    /* insert iml operations for foreach head */
    iml_add_foreach_head(compile_status->function,
                         foreach,
                         &index,
                         &length,
                         &loop_label,
                         exit_label);

    /*
     * validate and insert iml operations for foreach body
     */

    /* set current loop as this foreach-loop */
    IrLoop *prev_loop = compile_status->loop;
    compile_status->loop = IR_LOOP(foreach);

    validate_code_block(compile_status, ir_foreach_get_body(foreach));

    /* restore previous loop statment */
    compile_status->loop = prev_loop;

    /* insert iml operations for foreach tail */
    iml_add_foreach_tail(compile_status->function,
                         index,
                         length,
                         loop_label);
    /* insert loop exit label */
    ir_function_def_add_operation(compile_status->function, exit_label);
}

static void
validate_break(compilation_status_t *compile_status,
               sym_table_t *sym_table,
               IrBreak *break_stmt)
{
    if (compile_status->loop == NULL)
    {
        compile_error(compile_status,
                      break_stmt,
                      "break is not inside a loop\n");
        return;
    }

    iml_operation_t *break_label =
            ir_loop_get_exit_label(compile_status->loop);
    ir_function_def_add_operation(
            compile_status->function,
            iml_operation_new(iml_jmp,
                              iml_operation_get_operand(break_label, 1)));
}

static void
validate_statment(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrStatment *statment)
{
    if (IR_IS_FUNCTION_CALL(statment))
    {
         if (validate_function_call(compile_status,
                                    sym_table,
                                    IR_FUNCTION_CALL(statment)) == NULL)
         {
             /* invalid function call, bail out */
             return;
         }
         iml_add_func_call_eval(compile_status->function,
                                IR_FUNCTION_CALL(statment),
                                NULL);
    }
    else if (IR_IS_ASSIGNMENT(statment))
    {
        validate_assignment(compile_status,
                            sym_table,
                            IR_ASSIGNMENT(statment));
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
    else if (IR_IS_BREAK(statment))
    {
        validate_break(compile_status, sym_table, IR_BREAK(statment));
    }
    else if (IR_IS_EXPRESSION(statment))
    {
        compile_error(compile_status,
                      IR_NODE(statment),
                      "expression have no effect\n");
    }
}

static DtDataType *
resolve_user_type(compilation_status_t *compile_status,
                  DtName *user_type)
{
    DtDataType *type;

    type = ir_module_get_user_type(compile_status->module, user_type);
    if (type == NULL)
    {
        /* failed to look-up the data type */
        compile_error(compile_status,
                      user_type,
                      "unknown data type '%s'\n",
                      dt_name_get_name(user_type));
    }

    return type;
}

static DtDataType *
validate_type(compilation_status_t *compile_status,
              DtDataType *type)
{
    assert(compile_status);
    assert(DT_IS_DATA_TYPE(type));

    DtDataType *validated_type = type;

    if (DT_IS_NAME(type))
    {
        /*
         * user defined type,
         * look-up the data type object with the specified name
         */
        validated_type = resolve_user_type(compile_status, DT_NAME(type));
    }
    else if (DT_IS_POINTER(type))
    {
        DtDataType *base_type;
        DtPointer *ptr_type = DT_POINTER(type);

        base_type = validate_type(compile_status,
                                  dt_pointer_get_base_type(ptr_type));
        if (base_type == NULL)
        {
            return NULL;
        }

        dt_pointer_set_base_type(ptr_type, base_type);
    }

    return validated_type;
}


static IrVariable *
validate_variable(compilation_status_t *compile_status,
                  IrVariable *variable)
{
    assert(compile_status);
    assert(IR_IS_VARIABLE(variable));

    DtDataType *type;

    type = validate_type(compile_status,
                         ir_variable_get_data_type(variable));
    if (type == NULL)
    {
        /* invalid type */
        return NULL;
    }
    ir_variable_set_data_type(variable, type);

    return variable;
}

static void
validate_code_block(compilation_status_t *compile_status,
                    IrCodeBlock *code_block)
{
    sym_table_t *sym_table;
    GSList *i;
    GSList *local_vars;

    sym_table = ir_code_block_get_symbols(code_block);

    /*
     * validate default initializer expressions of local variables
     * in this code block
     */
    local_vars = ir_code_block_get_local_vars(code_block);

    for (i = local_vars; i != NULL; i = g_slist_next(i))
    {
        IrVariable *var;
        IrExpression *initializer;
        IrExpression *conv_initializer;
        DtDataType *var_type;

        var = validate_variable(compile_status, IR_VARIABLE(i->data));
        if (var == NULL)
        {
            /* invalid variable definition */
            continue;
        }

        initializer = ir_variable_get_initializer(var);
        var_type = ir_variable_get_data_type(var);

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

        conv_initializer = types_implicit_conv(var_type, initializer);
        if (conv_initializer == NULL && DT_IS_ARRAY(var_type))
        {
            /*
             * Handle the special case of array initialization from a scalar,
             * check if scalar can be implicitly converted to array's 
             * element data type
             */
            DtDataType *array_element_type;
 
            array_element_type =
                dt_array_get_element_type(DT_ARRAY(var_type));
            conv_initializer =
                types_implicit_conv(array_element_type, initializer);
        }
        if (conv_initializer == NULL)
        {
            compile_error(compile_status,
                          var,
                          "illegal type in initializer assignment\n");
            continue;
        }
        ir_variable_set_initializer(var, conv_initializer);
        ir_module_add_const_data(compile_status->module, conv_initializer);

        /*
         * now we know that variable and it's initializer expression are valid
         * add the variable to function's frame and it's initializer to
         * expression to operations list
         */
    }

    /*
     * if no errors are detected so far,
     * add local variables for function frame
     */
    if (compile_status->errors_count == 0)
    {
        for (i = local_vars; i != NULL; i = g_slist_next(i))
        {
            IrVariable *variable = IR_VARIABLE(i->data);
            add_to_func_frame(compile_status->function,
                              variable,
                              false);

            /* generate iml code for default initialization of the variable */
            IrExpression *init_exp = ir_variable_get_initializer(variable);

            if (init_exp == NULL)
            {
                init_exp =
                    dt_data_type_get_init(ir_variable_get_data_type(variable));
            }

            iml_add_assignment(compile_status->function,
                               IR_EXPRESSION(variable),
                               init_exp);
        }
    }

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
validate_function_decl(compilation_status_t *compile_status,
                       IrFunctionDecl *func_decl)
{
    assert(compile_status);
    assert(IR_IS_FUNCTION_DECL(func_decl));

    DtDataType *type;

    /* validate return type */
    type = validate_type(compile_status,
                         ir_function_get_return_type(IR_FUNCTION(func_decl)));
    if (type != NULL)
    {
        ir_function_set_return_type(IR_FUNCTION(func_decl),
                                    type);
    }

    /* validate parameters */
    GSList *i = ir_function_get_parameters(IR_FUNCTION(func_decl));
    for (; i != NULL; i = g_slist_next(i))
    {
        validate_variable(compile_status, IR_VARIABLE(i->data));
    }
}

static void
validate_function_def(compilation_status_t *compile_status,
                      IrFunctionDef *func_def)
{
    assert(compile_status);
    assert(IR_IS_FUNCTION_DEF(func_def));

    IrCodeBlock *body;
    DtDataType *type;

    compile_status->function = func_def;

    /*
     * validate parameter definitions and add parameters to function frame
     */
    GSList *i = ir_function_def_get_parameters(func_def);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrVariable *var;

        var = validate_variable(compile_status, IR_VARIABLE(i->data));
        if (var == NULL)
        {
            /* invalid parameter */
            continue;
        }
        /* Add to function frame */
        add_to_func_frame(func_def, var, true);
    }

    /* resolve possible user types in function return type */
    type = ir_function_def_get_return_type(func_def);
    type = validate_type(compile_status,
                         ir_function_def_get_return_type(func_def));
    if (type != NULL)
    {
        ir_function_def_set_return_type(func_def, type);
    }

    /* validate function's body */
    body = ir_function_def_get_body(func_def);
    validate_code_block(compile_status, body);

    /*
     * For void function, add an implicit return statement if needed
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
            ir_function_def_add_operation(compile_status->function,
                                      iml_operation_new(iml_return, NULL));

        }
    }
}

static IrExpression *
validate_array_slice(compilation_status_t *compile_status,
                     sym_table_t *sym_table,
                     IrArraySlice *array_slice)
{
    DtDataType *array_type;
    IrExpression *exp;

    /* validate array expression */
    exp = ir_array_slice_get_array(array_slice);
    exp = validate_expression(compile_status, sym_table, exp);
    if (exp == NULL) {
        /* invalid array expression, bail out */
        return NULL;
    }

    /* make sure array expression is of array type */
    array_type = ir_expression_get_data_type(exp);
    if (!DT_IS_ARRAY(array_type))
    {
        compile_error(compile_status,
                      array_slice,
                      "expected array type for slice expression, got %s\n",
                      dt_data_type_get_string(array_type));
        return NULL;
    }
    ir_array_slice_set_array(array_slice, exp);


    /*
     * validate start expression
     */
    exp = ir_array_slice_get_start(array_slice);
    if (exp == NULL)
    {
        exp = IR_EXPRESSION(ir_basic_constant_new_uint(0, 0));
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
        if (DT_IS_STATIC_ARRAY_TYPE(array_type))
        {
          /* slice over static array */
          guint32 end_idx;

          end_idx =
              dt_static_array_get_length(DT_STATIC_ARRAY(array_type));
          exp = IR_EXPRESSION(ir_basic_constant_new_uint(end_idx, 0));
        }
        else
        {
           /* slice over dynamic array */
           exp = IR_EXPRESSION(
                   ir_property_new(ir_array_slice_get_array(array_slice),
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
    ir_module_add_const_data(compile_status->module,
                             IR_EXPRESSION(array_literal));

    return IR_EXPRESSION(array_literal);
}

static void
validate_enum(compilation_status_t *compile_status,
              IrEnum *enum_def)
{
    assert(IR_IS_ENUM(enum_def));

    GSList *members;
    GSList *i;
    sym_table_t *sym_table;
    DtDataType *base_type;
    IrExpression *first_member_init;
    IrExpression *prev_member_value;
    bool enum_member_errs = false;
    static IrExpression *one = NULL;

    if (one == NULL)
    {
        one = IR_EXPRESSION(ir_basic_constant_new_int(1, 0));
    }

    sym_table = ir_module_get_symbols(compile_status->module);
    members = ir_enum_get_members(enum_def);
    first_member_init = ir_enum_member_get_value(members->data);

    /*
     * validate enum members initialization expressions
     */
    for (i = members; i != NULL; i = g_slist_next(i))
    {
        IrExpression *var;
        IrExpression *tmp;

        var = ir_enum_member_get_value(i->data);
        if (var == NULL)
        {
            continue;
        }

        tmp = validate_expression(compile_status,
                                  sym_table,
                                  var);
        if (tmp == NULL)
        {
            compile_error(compile_status,
                          var,
                          "invalid enum member initialization expression\n");
            enum_member_errs = true;
            continue;
        }
        ir_enum_member_set_value(i->data, tmp);
    }

    if (enum_member_errs)
    {
        /*
         * errors detected in enum member value expression,
         * can not continue validation
         */
        return;
    }

    /*
     * figure out enum's base type
     */
    base_type = ir_enum_get_base_type(enum_def);
    if (base_type == NULL)
    {
        /*
         * get first member's initialization expression
         */
        if (first_member_init != NULL)
        {
            /* enum's base type is derived from first members type */
            base_type = ir_expression_get_data_type(first_member_init);
        }
        else
        {
            /* use default int base type */
            base_type = types_get_int_type();
        }
        ir_enum_set_base_type(enum_def, base_type);
    }

    /*
     * figure out enum members values
     */
    if (first_member_init == NULL)
    {
        prev_member_value =
            cfold_cast(
                ir_cast_new(base_type,
                            IR_EXPRESSION(ir_basic_constant_new_int(0, 0))));
        ir_enum_member_set_value(members->data, prev_member_value);
    }

    for (i = members; i != NULL; i = g_slist_next(i))
    {
        IrEnumMember *member = IR_ENUM_MEMBER(i->data);
        IrExpression *value = ir_enum_member_get_value(member);

        if (value == NULL)
        {
            /* calculate next member value by increasing previous with 1 */
            value =
                validate_binary_op(
                    compile_status,
                    sym_table,
                        ir_binary_operation_new(
                            ast_plus_op,
                            prev_member_value,
                            one,
                        0));
            value = cfold_cast(ir_cast_new(base_type, value));

            ir_enum_member_set_value(member, value);
        }
        prev_member_value = value;
    }
}

static void
validate_struct(compilation_status_t *compile_status,
                IrStruct *ir_struct)
{
    assert(compile_status);
    assert(IR_IS_STRUCT(ir_struct));

    DtStruct *struct_type;
    GSList *i;

    struct_type = ir_struct_get_data_type(ir_struct);
    for (i = ir_struct_get_members(ir_struct); i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_VARIABLE(i->data));
        IrVariable *var = IR_VARIABLE(i->data);
        DtDataType *type = ir_variable_get_data_type(var);

        type = validate_type(compile_status, type);
        if (type == NULL)
        {
            /* invalid type, skip to next member */
            continue;
        }

        ir_variable_set_data_type(var, type);
        dt_struct_add_member(struct_type, type, ir_variable_get_name(var));
    }
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

static void
assign_registers(IrFunctionDef *func, arch_backend_t *backend)
{
    iml_func_frame_t *frame = ir_function_def_get_frame(func);
    GSList *scratch_regs;
    GSList *preserved_regs;
    GSList *regs; /* all available registers */
    GSList *i;
    GSList *vars;
    GSList *used_regs = NULL;

    backend->get_registers(&scratch_regs, &preserved_regs);

    /* only use preserved register for now */
    regs = preserved_regs;

    /* assign registers to 32b variables */
    vars = iml_func_frame_get_locals(frame, iml_32b);
    for (i = vars; i != NULL && regs != NULL; i = g_slist_next(i))
    {
        const char *reg = regs->data;

        /* remove register from the list of available */
        regs = g_slist_remove(regs, reg);

        /* assign the register to the variable */
        iml_variable_set_register(i->data, reg);
        used_regs = g_slist_prepend(used_regs, (gpointer)reg);
    }

    /* assign registers to 16b variables */
    vars = iml_func_frame_get_locals(frame, iml_16b);
    for (i = vars; i != NULL && regs != NULL; i = g_slist_next(i))
    {
        const char *reg = regs->data;

        /* remove register from the list of available */
        regs = g_slist_remove(regs, reg);

        /* assign the register to the variable */
        iml_variable_set_register(i->data, reg);
        used_regs = g_slist_prepend(used_regs, (gpointer)reg);
    }

    /* assign registers to 8b variables */
    vars = iml_func_frame_get_locals(frame, iml_8b);
    for (i = vars; i != NULL && regs != NULL; i = g_slist_next(i))
    {
        const char *reg = regs->data;

        /* remove register from the list of available */
        regs = g_slist_remove(regs, reg);

        /* assign the register to the variable */
        iml_variable_set_register(i->data, reg);
        used_regs = g_slist_prepend(used_regs, (gpointer)reg);
    }

    /* assign registers to pointer variables */
    vars = iml_func_frame_get_locals(frame, iml_ptr);
    for (i = vars; i != NULL && regs != NULL; i = g_slist_next(i))
    {
        const char *reg = regs->data;

        /* remove register from the list of available */
        regs = g_slist_remove(regs, reg);

        /* assign the register to the variable */
        iml_variable_set_register(i->data, reg);
        used_regs = g_slist_prepend(used_regs, (gpointer)reg);
    }

    /* store the list of used preserved registers */
    iml_func_frame_set_used_regs(frame, used_regs);

    /*
     * call the backend hook for
     * assigning locations to this frames variables
     */
    backend->assign_var_locations(frame,
                                  ir_function_get_linkage(IR_FUNCTION(func)));
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

    /* validate enum definitions */
    i = ir_module_get_enums(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_ENUM(i->data));
        validate_enum(compile_status,
                      IR_ENUM(i->data));
    }

    /* validate struct definitions */
    i = ir_module_get_structs(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_STRUCT(i->data));
        validate_struct(compile_status, IR_STRUCT(i->data));
    }

    /* validate function declarations */
    i = ir_module_get_function_decls(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        validate_function_decl(compile_status, IR_FUNCTION_DECL(i->data));
    }

    /* validate function definitions */
    i = ir_module_get_function_defs(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrFunctionDef *func_def = IR_FUNCTION_DEF(i->data);

        validate_function_def(compile_status, func_def);
        if (compile_status->errors_count == 0) {
            assign_registers(func_def, compile_status->backend);
        }
    }
}

