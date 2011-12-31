#include <stdbool.h>

#include "sem_analyze_validate.h"
#include "dt_name.h"
#include "dt_pointer.h"
#include "types.h"
#include "dt_auto.h"
#include "dt_void.h"
#include "dt_static_array.h"
#include "dt_pointer.h"
#include "dt_enum.h"
#include "ir_function.h"
#include "ir_assignment.h"
#include "ir_function_call.h"
#include "ir_method_call.h"
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
#include "ir_for.h"
#include "ir_foreach.h"
#include "ir_foreach_range.h"
#include "ir_break.h"
#include "ir_array_cell.h"
#include "ir_property.h"
#include "ir_enum_member.h"
#include "ir_dot.h"
#include "ir_ptr_dref.h"
#include "ir_address_of.h"
#include "ir_ident.h"
#include "ir_var_value.h"
#include "ir_var_ref.h"
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
              DtEnum *enum_def);

/**
 * Look-up the user defined data type in current module. If no data type
 * with provided name is found, a compile error is printed.
 *
 * @param sym_table the lexical scope where to look for the type
 * @param user_type the placeholder object for user defined type
 *
 * @return the data type found or NULL if failed to look-up data type
 */
static DtDataType *
resolve_user_type(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  DtName *user_type);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrExpression *
validate_ident(compilation_status_t *compile_status,
               sym_table_t *sym_table,
               IrIdent *ident)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_IDENT(ident));

    IrSymbol *symb;
    GError *err = NULL;

    symb = sym_table_get_symbol(sym_table, ir_ident_get_name(ident), &err);
    if (symb == NULL)
    {
        switch (err->code)
        {
            case SYM_TABLE_SYMBOL_NOT_FOUND_ERROR:
                compile_error(compile_status,
                              ident,
                              "reference to unknown symbol '%s'\n",
                              ir_ident_get_name(ident));
                break;
            case SYM_TABLE_MULTIPLE_SYMBOLS_FOUND_ERROR:
                compile_error(compile_status,
                              ident,
                              "ambiguous reference '%s', matches: %s\n",
                              ir_ident_get_name(ident),  err->message);
                break;
            default:
                assert(false); /* unexpected error code */
        }
        return NULL;
    }

    if (IR_IS_FUNCTION(symb))
    {
        compile_error(compile_status,
                      ident,
                      "invalid call to function '%s',"
                      " expected arguments\n",
                      ir_ident_get_name(ident));
        return NULL;
    }
    else if (IR_IS_VARIABLE(symb))
    {
        IrVariable *var = ir_variable(symb);
        if (ir_variable_is_ref(var))
        {
            return
                ir_expression(ir_var_ref_new(ir_variable(var),
                                             ir_node_get_line_num(ident)));
        }
        else
        {
            return
                ir_expression(ir_var_value_new(ir_variable(var),
                                               ir_node_get_line_num(ident)));
        }
    }

    return ir_expression(symb);
}

static IrCall *
validate_call(compilation_status_t *compile_status,
              sym_table_t *sym_table,
              IrCall *call)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_CALL(call));

    IrFunction *func = ir_call_get_function(call);
    GSList *params = ir_function_get_parameters(func);
    GSList *args = ir_call_get_arguments(call);

    /*
     * check that function call have correct number of arguments
     */
    if (g_slist_length(params) != g_slist_length(args))
    {
        compile_error(compile_status,
                      call,
                      "invalid call to function '%s()', expected %d "
                      "arguments, got %d\n",
                      ir_symbol_get_name(ir_symbol(func)),
                      g_slist_length(params),
                      g_slist_length(args));
        return NULL;
    }

    GSList *validated_args = NULL;
    GSList *i;
    GSList *j;
    guint counter;

    /*
     * validate function call arguments
     */
    for (i = args, j = params, counter = 1;
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
                                      ir_expression(i->data));
        if (arg_exp == NULL)
        {
            /* invalid argument expression, bail out */
            return NULL;
        }

        /* check if the type is compatible */
        formal_arg_type = ir_variable_get_data_type(j->data);

        exp = types_implicit_conv(formal_arg_type, arg_exp);
        if (exp == NULL)
        {
            compile_error(compile_status,
                          IR_NODE(arg_exp),
                          "in function call to '%s()', argument %d of invalid type\n",
                          ir_symbol_get_name(ir_symbol(func)),
                          counter);
            compile_error(compile_status,
                          IR_NODE(arg_exp),
                          "argument's type is '%s' got '%s'\n",
                          dt_data_type_get_string(formal_arg_type),
                          dt_data_type_get_string(ir_expression_get_data_type(arg_exp)));
            return NULL;
        }

        if (ir_variable_is_ref(j->data))
        {
            if (!ir_expression_is_lvalue(exp))
            {
                compile_error(compile_status,
                              exp,
                              "argument %d to function '%s()' "
                              "must be an lvalue\n",
                              counter,
                              ir_symbol_get_name(ir_symbol(func)));
                return NULL;
            }
            exp =
                ir_expression(
                    ir_address_of_new(exp, ir_node_get_line_num(exp)));
        }

        /* valid expression */
        validated_args = g_slist_prepend(validated_args, exp);
    }

    /* store validated call arguments */
    ir_call_set_arguments(call, g_slist_reverse(validated_args));

    return call;
}


static IrExpression *
validate_function_call(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrFunctionCall *func_call)
{
    IrSymbol *func_symb;
    char *func_name;
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

    /* store called function object */
    ir_call_set_function(IR_CALL(func_call), ir_function(func_symb));

    if (validate_call(compile_status,
                           sym_table,
                           IR_CALL(func_call)) == NULL)
    {
        /* invalid call arguments, bail out */
        return NULL;
    }

    return ir_expression(func_call);
}

/**
 * Check if type is a struct or pointer to struct type, e.g. if struct '.'
 * operations can be used on an expression of specified type.
 *
 * @return NULL if not a struct type,
 *         otherwise the refered struct type
 */
static DtStruct *
get_struct_type(DtDataType *type)
{
    if (DT_IS_STRUCT(type))
    {
        return DT_STRUCT(type);
    }
    else if (DT_IS_POINTER(type))
    {
        return get_struct_type(dt_pointer_get_base_type(DT_POINTER(type)));
    }
    return NULL;
}

static IrExpression *
validate_method_call(compilation_status_t *compile_status,
                     sym_table_t *sym_table,
                     IrMethodCall *method_call)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_METHOD_CALL(method_call));

    IrExpression *exp;
    DtStruct *struct_type;

    /*
     * validate this expression
     */
    exp = validate_expression(compile_status,
                              sym_table,
                              ir_method_call_get_this_exp(method_call));
    if (exp == NULL)
    {
        /* invalid this expression, bail out */
        return NULL;
    }

    /* check that this expression is of struct type */
    struct_type = get_struct_type(ir_expression_get_data_type(exp));
    if (struct_type == NULL)
    {
        compile_error(compile_status,
                      exp,
                      "method call on non-struct type '%s'\n",
                      dt_data_type_get_string(
                          ir_expression_get_data_type(exp)));
        return NULL;
    }

    ir_method_call_set_this_exp(method_call, exp);

    /* look-up called method */
    IrFunctionDef *method =
        dt_struct_get_method(struct_type,
                             ir_method_call_get_name(method_call));
    if (method == NULL)
    {
        compile_error(compile_status,
                      method_call,
                      "struct '%s' have no method '%s'\n",
                      ir_symbol_get_name(ir_symbol(struct_type)),
                      ir_method_call_get_name(method_call));
        return NULL;
    }
    ir_call_set_function(IR_CALL(method_call), ir_function(method));

    if (validate_call(compile_status, sym_table, IR_CALL(method_call)) == NULL)
    {
        /* invalid call arguments, bail out */
        return NULL;
    }

    return ir_expression(method_call);
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

    binary_op_type_t op = ir_binary_operation_get_operation(bin_op);
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

    if (op == op_plus)
    {
        right =
            ir_expression(
                ir_binary_operation_new(op_mult,
                                        right,
                                        ir_expression(base_type_size),
                                        0));
        right =
            validate_bin_arithm(compile_status,
                                sym_table,
                                ir_binary_operation(right));
    }
    else
    {
        assert(op == op_minus);

        if (DT_IS_POINTER(right_type))
        {
            left =
                ir_expression(
                    ir_binary_operation_new(op_minus, left, right, 0));
            right = ir_expression(base_type_size);
            bin_op = ir_binary_operation_new(op_division, left, right, 0);
        }
        else
        {
            right =
                ir_expression(
                    ir_binary_operation_new(op_mult,
                                            right,
                                            ir_expression(base_type_size),
                                            0));
            right =
                validate_bin_arithm(compile_status,
                                    sym_table,
                                    ir_binary_operation(right));
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
    if (DT_IS_VOID(data_type))
    {
        compile_error(compile_status,
                      IR_NODE(bin_op),
                      "left operand can not be of void type\n");
        return NULL;
    }

    /* if left operand is not bool, cast to bool */
    if (!dt_basic_is_bool(data_type))
    {
        left = cfold_cast(ir_cast_new(types_get_bool_type(), left, 0));
    }

    /*
     * check right operand data type
     */
    data_type = ir_expression_get_data_type(right);


    /* if right operand is not of void or bool type, cast to bool */
    if (!DT_IS_VOID(data_type) &&
        !dt_basic_is_bool(data_type))
    {
        right = cfold_cast(ir_cast_new(types_get_bool_type(), right, 0));
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
        switch (dt_basic_get_data_type(dt_basic(exp_type)))
        {
            case char_type:
            case byte_type:
            case ubyte_type:
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
        goto invalid_type;
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
        case op_arithm_neg:
            exp = types_implicit_conv(types_get_int_type(), exp);
            if (exp == NULL)
            {
                compile_error(compile_status,
                              IR_NODE(ir_unary_operation_get_operand(operation)),
                              "can not convert to int type\n");
                return NULL;
            }
            break;
        case op_bool_neg:
            exp = types_implicit_conv(types_get_bool_type(), exp);
            if (exp == NULL)
            {
                compile_error(compile_status,
                              IR_NODE(ir_unary_operation_get_operand(operation)),
                              "can not convert to bool type\n");
                return NULL;
            }
            break;
        case op_pre_inc:
        case op_pre_dec:
        case op_post_inc:
        case op_post_dec:
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
    return ir_expression(operation);
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
    symb_type = ir_expression_get_data_type(ir_expression(array_symb));
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

    return ir_expression(cell);
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

    IrExpression *exp;
    DtDataType *exp_type;
    IrBasicConstant *size_exp;

    exp = ir_property_get_expression(prop);
    if (DT_IS_DATA_TYPE(exp))
    {
        /* sizeof property of a data type */
        exp_type = DT_DATA_TYPE(exp);
    }
    else
    {
        exp_type = ir_expression_get_data_type(exp);
    }

    size_exp = ir_basic_constant_new_uint(dt_data_type_get_size(exp_type),
                                          ir_node_get_line_num(prop));

    return ir_expression(size_exp);
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
      exp = ir_enum_member_get_value(ir_enum_member(exp));
    }

    if (ir_is_array_literal(exp))
    {
        guint32 length;

        length = ir_array_literal_get_length(ir_array_literal(exp));

        return
            ir_expression(
                ir_basic_constant_new_int(length,
                                          ir_node_get_line_num(prop)));
    }
    else if (DT_IS_STATIC_ARRAY_TYPE(exp_type))
    {
        guint32 length;

        length = 
            dt_static_array_get_length(DT_STATIC_ARRAY(exp_type));

        return
            ir_expression(
                ir_basic_constant_new_int(length,
                                          ir_node_get_line_num(prop)));
    }
    else if (DT_IS_ARRAY(exp_type))
    {
        /* run-time expression */
        return ir_expression(prop);
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
    return ir_expression(prop);
}

static IrExpression *
validate_dot_var_value(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrExpression *left,
                       IrIdent *right)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_VAR_VALUE(left));
    assert(IR_IS_IDENT(right));

    DtStruct *dt_struct = get_struct_type(ir_expression_get_data_type(left));
    if (dt_struct != NULL)
    {
        IrStructMember *mbr = dt_struct_get_member(dt_struct, right);
        if (mbr != NULL)
        {
            ir_struct_member_set_base(mbr, left);
            return ir_expression(mbr);
        }
    }

    return validate_dot_property(compile_status,
                                 sym_table,
                                 left,
                                 ir_expression(right));
}

static IrExpression *
validate_dot(compilation_status_t *compile_status,
             sym_table_t *sym_table,
             IrDot *dot)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_DOT(dot));

    IrExpression *left;
    IrExpression *right = ir_dot_get_right(dot);
    IrExpression *res;

    left = validate_expression(compile_status,
                               sym_table,
                               ir_dot_get_left(dot));
    assert(IR_IS_IDENT(right));
    if (left == NULL)
    {
        /* invalid left expression, bail out */
        return NULL;
    }

    if (dt_is_enum(left))
    {
        IrEnumMember *mbr;

        mbr = dt_enum_get_member(dt_enum(left),
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
            res = ir_expression(mbr);
        }
    }
    else if (IR_IS_VAR_VALUE(left))
    {
        res = validate_dot_var_value(compile_status,
                                     sym_table,
                                     left,
                                     IR_IDENT(right));
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

    return ir_expression(ptr_dref);
}

static IrExpression *
validate_address_of(compilation_status_t *compile_status,
                    sym_table_t *sym_table,
                    IrAddressOf *address_of)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_ADDRESS_OF(address_of));

    IrExpression *exp = ir_address_of_get_expression(address_of);

    /* validate operand expression */
    exp = validate_expression(compile_status, sym_table, exp);
    if (exp == NULL)
    {
        /* invalid expression dereferenced */
        return NULL;
    }
    ir_address_of_set_expression(address_of, exp);

    /* check that operand is an lvalue */
    if (!ir_expression_is_lvalue(exp))
    {
        compile_error(compile_status,
                      exp,
                      "& expects a lvalue operand\n");
        return NULL;
    }

    return ir_expression(address_of);
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
        true_exp = ir_expression(ir_cast_new(common, true_exp, 0));
    }

    if (!dt_data_type_is_same(common, false_type))
    {
        false_exp = ir_expression(ir_cast_new(common, false_exp, 0));
    }


    ir_conditional_set_expressions(cond, eval, true_exp, false_exp);

    return cfold_conditional(cond);
}

static IrExpression *
validate_cast(compilation_status_t *compile_status,
              sym_table_t *sym_table,
              IrCast *cast)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_CAST(cast));

    IrExpression *val = validate_expression(compile_status,
                                            sym_table,
                                            ir_cast_get_value(cast));
    if (val == NULL)
    {
        /* invalid expression to cast, bail out */
        return NULL;
    }

    ir_cast_set_value(cast, val);
    return ir_expression(cast);
}

/**
 * Validate the expression. The expression may be transformed during
 * validation, for example implicit cast are converted to explicit.
 *
 * @return the validated expression or NULL if the expression is invalid
 */
static IrExpression *
validate_expression(compilation_status_t *compile_status,
                    sym_table_t *sym_table,
                    IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    if (IR_IS_IDENT(expression))
    {
        expression =
            validate_ident(compile_status, sym_table, IR_IDENT(expression));
    }
    else if (IR_IS_BINARY_OPERATION(expression))
    {
        expression =
            validate_binary_op(compile_status,
                               sym_table,
                               ir_binary_operation(expression));
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
                                   ir_function_call(expression));
    }
    else if (IR_IS_METHOD_CALL(expression))
    {
        expression = validate_method_call(compile_status,
                                          sym_table,
                                          IR_METHOD_CALL(expression));
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
                                   ir_array_literal(expression));
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
    else if (IR_IS_ADDRESS_OF(expression))
    {
        expression =
            validate_address_of(compile_status,
                                sym_table,
                                ir_address_of(expression));
    }
    else if (IR_IS_CONDITIONAL(expression))
    {
        expression =
            validate_conditional(compile_status,
                                 sym_table,
                                 IR_CONDITIONAL(expression));
    }
    else if (IR_IS_CAST(expression))
    {
        expression = validate_cast(compile_status,
                                   sym_table,
                                   ir_cast(expression));
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
        ir_function_get_return_type(ir_function(compile_status->function));

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

        if (compile_status->errors_count == 0)
        {
            /*
             * valid return statement, add iml operations
             */
            ImlOperand *ret_val =
                iml_add_expression_eval(compile_status->iml_func,
                                        conv_exp,
                                        NULL,
                                        false);
            iml_function_add_operation(compile_status->iml_func,
                                       iml_operation_new(iml_return, ret_val));
        }
    }
    else
    {
        /* a return statement without an expression specified */
        if (!DT_IS_VOID(func_return_type))
        {
            compile_error(compile_status,
                          IR_NODE(ret),
                          "return expression of type %s expected\n",
                          dt_data_type_get_string(func_return_type));
            return;
        }

        /* valid return from void function, add iml-operation */
        iml_function_add_operation(compile_status->iml_func,
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
    target_type = ir_expression_get_data_type(ir_expression(lvalue));
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
            dt_array_get_element_type(dt_array(target_type));
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
    if (compile_status->errors_count == 0)
    {
        iml_add_assignment(compile_status->iml_func, lvalue, converted_value);
    }
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
    condition_eval_res = iml_add_expression_eval(compile_status->iml_func,
                                                 condition,
                                                 NULL,
                                                 false);

    /*
     * add conditional jump operation if
     * the condition expression evaluates for false
     */
    skip_label =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    iml_function_add_operation(
            compile_status->iml_func,
            iml_operation_new(iml_jmpneq,
                              condition_eval_res,
                              iml_constant_new_8b(1),
                              iml_operation_get_operand(skip_label, 1)));

    /* mark condition result operand as unused */
    iml_function_unused_oper(compile_status->iml_func, condition_eval_res);

    /* validate if body */
    body = ir_if_block_get_body(if_block);
    validate_code_block(compile_status, body);

    /* add jump operation past the rest of if-else bodies if needed */
    if (end_label != NULL)
    {
        iml_function_add_operation(
                compile_status->iml_func,
                iml_operation_new(iml_jmp,
                                  iml_operation_get_operand(end_label, 1)));
    }

    /* insert the skip label */
    iml_function_add_operation(compile_status->iml_func, skip_label);
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
       iml_function_add_operation(compile_status->iml_func, end_label);
   }
}

static void
validate_while(compilation_status_t *compile_status,
               sym_table_t *sym_table,
               IrWhile *while_statment)
{
    IrExpression *condition;
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

    /* generate iml for the loop head */
    iml_add_while_head(compile_status->iml_func,
                       condition,
                       loop_start,
                       loop_end);

    /*
     * validate while body, and generate iml operations
     */
    IrLoop *prev_loop = compile_status->loop;
    compile_status->loop = IR_LOOP(while_statment);

    validate_code_block(compile_status, ir_while_get_body(while_statment));

    compile_status->loop = prev_loop;

    /* generate iml for the loop tail */
    iml_add_while_tail(compile_status->iml_func, loop_start, loop_end);
}

static void
validate_for(compilation_status_t *compile_status,
             sym_table_t *sym_table,
             IrFor *for_loop)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_FOR(for_loop));

    /* validate and generate iml for init code */
    IrCodeBlock *init = ir_for_get_init(for_loop);
    validate_code_block(compile_status, init);

    /* generate iml labels for start and end of the loop operations */
    iml_operation_t *loop_start =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    iml_operation_t *loop_end =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    ir_loop_set_exit_label(IR_LOOP(for_loop), loop_end);


    IrExpression *test = ir_for_get_test(for_loop);
    if (test != NULL)
    {
        test = validate_expression(compile_status,
                                   ir_code_block_get_symbols(init),
                                   test);
        if (test == NULL)
        {
            /* invalid test expression, bail out */
            return;
        }

        /* insert implicit conversion to boolean type */
        test = types_implicit_conv(types_get_bool_type(), test);
        if (test == NULL)
        {
            compile_error(compile_status,
                          ir_for_get_test(for_loop),
                          "can not convert for condition to bool type\n");
            return;
        }
    }

    /* generate for-lopp head's iml */
    if (compile_status->errors_count == 0)
    {
        iml_add_for_head(compile_status->iml_func, test, loop_start, loop_end);
    }


    IrExpression *step = ir_for_get_step(for_loop);
    if (step != NULL)
    {
        step = validate_expression(compile_status,
                                   ir_code_block_get_symbols(init),
                                   step);
        if (step == NULL)
        {
            /* invalid step expression, bail out */
            return;
        }
    }

    /*
     * validate while body, and generate iml operations
     */
    IrLoop *prev_loop = compile_status->loop;
    compile_status->loop = IR_LOOP(for_loop);
    validate_code_block(compile_status, ir_for_get_body(for_loop));
    compile_status->loop = prev_loop;

    if (compile_status->errors_count == 0)
    {
        iml_add_for_tail(compile_status->iml_func,
                         step, loop_start, loop_end);
    }
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

    ir_foreach_set_aggregate(foreach, aggregate);

    aggr_element_type = dt_array_get_element_type(dt_array(aggr_type));
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
        if (dt_basic_get_data_type(dt_basic(var_type)) !=
            dt_basic_get_data_type(dt_basic(aggr_element_type)))
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
        else if (!dt_basic_is_int(var_type) &&
                 !dt_basic_is_uint(var_type))
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
    iml_operation_t *loop_label =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    iml_add_foreach_head(compile_status->iml_func,
                         foreach,
                         &index,
                         &length,
                         loop_label,
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
    iml_add_foreach_tail(compile_status->iml_func,
                         index,
                         length,
                         loop_label);
    /* insert loop exit label */
    iml_function_add_operation(compile_status->iml_func, exit_label);
}

static void
validate_foreach_range(compilation_status_t *compile_status,
                       sym_table_t *sym_table,
                       IrForeachRange *foreach)
{
    assert(compile_status);
    assert(sym_table);
    assert(IR_IS_FOREACH_RANGE(foreach));

    /*
     * validate lower and upper expressions
     */
    IrExpression *lower_exp =
        validate_expression(compile_status,
                            sym_table,
                            ir_foreach_range_get_lower_exp(foreach));

    IrExpression *upper_exp =
        validate_expression(compile_status,
                            sym_table,
                            ir_foreach_range_get_upper_exp(foreach));

    if (lower_exp == NULL || upper_exp == NULL)
    {
        /* invalid upper of lower expression, bail out */
        return;
    }

    /*
     * find lower and upper expressions common type
     */
    DtDataType *lower_type = ir_expression_get_data_type(lower_exp);
    DtDataType *upper_type = ir_expression_get_data_type(upper_exp);
    DtDataType *common_type = types_find_common(lower_type, upper_type);

    if (common_type == NULL)
    {
        /* failed to find common type, bail out with compiler error */
        assert(false); /* not implemented */
    }
    else if (DT_IS_VOID(common_type))
    {
        compile_error(compile_status,
                      lower_exp,
                      "can't use void expression in foreach\n");
        return;
    }

    /* cast lower expression to common type if needed */
    if (!dt_data_type_is_same(common_type, lower_type))
    {
        lower_exp = ir_expression(ir_cast_new(common_type, lower_exp, 0));
    }

    /* cast upper expression to common type if needed */
    if (!dt_data_type_is_same(common_type, upper_type))
    {
        upper_exp = ir_expression(ir_cast_new(common_type, upper_exp, 0));
    }

    /* set or check index variable's data type */
    IrVarValue *idx_var_val = ir_foreach_range_get_index(foreach);
    IrVariable *idx_var = ir_var_value_get_var(idx_var_val);
    DtDataType *idx_type = ir_variable_get_data_type(idx_var);
    if (DT_IS_AUTO(idx_type))
    {
        ir_variable_set_data_type(idx_var, common_type);
    }
    else if (!dt_data_type_is_same(common_type, idx_type))
    {
        /* check if common_type and index_type are compatible */
        assert(false); /* not implemented */
    }

    /* allocate a slot in function frame for loop's index variable */
    add_to_func_frame(compile_status->iml_func, idx_var, false);

    /* generate iml labels for start and end of the loop operations */
    iml_operation_t *loop_start =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    iml_operation_t *loop_end =
        iml_operation_new(iml_label,
                          ir_module_gen_label(compile_status->module));
    ir_loop_set_exit_label(IR_LOOP(foreach), loop_end);

    /* synthesize looping test expression */
    IrExpression *loop_test_exp =
        ir_expression(ir_binary_operation_new(op_less,
                                              ir_expression(idx_var_val),
                                              upper_exp,
                                              0));
    loop_test_exp =
        validate_expression(compile_status,  sym_table, loop_test_exp);
    assert(loop_test_exp != NULL);


    /* generate loop's head iml code */
    ImlOperand *head_temp_op =
        iml_add_foreach_range_head(compile_status->iml_func,
                                   idx_var_val,
                                   lower_exp,
                                   loop_test_exp,
                                   loop_start,
                                   loop_end);

    /*
     * validate and insert iml operations for foreach body
     */

    /* set current loop as this foreach-loop */
    IrLoop *prev_loop = compile_status->loop;
    compile_status->loop = IR_LOOP(foreach);

    validate_code_block(compile_status, ir_foreach_range_get_body(foreach));

    /* restore previous loop statment */
    compile_status->loop = prev_loop;

    /* synthesize loop index variable increment expression */
    IrExpression *index_inc_exp =
            ir_expression(ir_unary_operation_new(op_pre_inc,
                                                 ir_expression(idx_var_val),
                                                 0));
    index_inc_exp =
            validate_expression(compile_status,  sym_table, index_inc_exp);
    assert(index_inc_exp != NULL);

    /* generate iml for loop's tail */
    iml_add_foreach_range_tail(compile_status->iml_func,
                               index_inc_exp,
                               head_temp_op,
                               loop_start,
                               loop_end);
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
    iml_function_add_operation(
            compile_status->iml_func,
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
                                    ir_function_call(statment)) == NULL)
         {
             /* invalid function call, bail out */
             return;
         }
         iml_add_call_eval(compile_status->iml_func, IR_CALL(statment), NULL);
    }
    else if (IR_IS_ASSIGNMENT(statment))
    {
        validate_assignment(compile_status,
                            sym_table,
                            ir_assignment(statment));
    }
    else if (IR_IS_RETURN(statment))
    {
        validate_return(compile_status, sym_table, ir_return(statment));
    }
    else if (IR_IS_IF_ELSE(statment))
    {
        validate_if_else(compile_status, sym_table, IR_IF_ELSE(statment));
    }
    else if (IR_IS_WHILE(statment))
    {
        validate_while(compile_status, sym_table, IR_WHILE(statment));
    }
    else if (IR_IS_FOR(statment))
    {
        validate_for(compile_status, sym_table, IR_FOR(statment));
    }
    else if (IR_IS_FOREACH(statment))
    {
        validate_foreach(compile_status, sym_table, ir_foreach(statment));
    }
    else if (IR_IS_FOREACH_RANGE(statment))
    {
        validate_foreach_range(compile_status,
                               sym_table,
                               IR_FOREACH_RANGE(statment));
    }
    else if (IR_IS_BREAK(statment))
    {
        validate_break(compile_status, sym_table, IR_BREAK(statment));
    }
    else if (IR_IS_EXPRESSION(statment))
    {
        IrExpression *exp =
            validate_expression(compile_status,
                                sym_table,
                                ir_expression(statment));
        if (exp == NULL)
        {
            /* invalid expression, bail out */
            return;
        }

        if (!ir_expression_has_effect(exp))
        {
            compile_error(compile_status,
                          IR_NODE(statment),
                          "expression have no effect\n");
            return;
        }
        iml_add_expression_eval(compile_status->iml_func, exp, NULL, true);
    }
    else
    {
        /* unexpected statment */
        assert(false);
    }
}

static DtDataType *
resolve_user_type(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  DtName *user_type)
{
    GError *error = NULL;
    IrSymbol *type;

    type = sym_table_get_symbol(sym_table,
                                dt_name_get_name(user_type),
                                &error);
    if (type == NULL)
    {
        switch (error->code)
        {
            case SYM_TABLE_SYMBOL_NOT_FOUND_ERROR:
                /* failed to look-up the data type */
                compile_error(compile_status,
                              user_type,
                              "unknown data type '%s'\n",
                              dt_name_get_name(user_type));
                break;
            case SYM_TABLE_MULTIPLE_SYMBOLS_FOUND_ERROR:
                /* ambiguous type name */
                compile_error(compile_status,
                              user_type,
                              "ambiguous type name '%s', matches: %s\n",
                              dt_name_get_name(user_type),
                              error->message);
                break;
            default:
                assert(false); /* unexpected error code */

        }
        return NULL;
    }
    else if (!DT_IS_DATA_TYPE(type))
    {
        compile_error(compile_status,
                      user_type,
                      "'%s' is not a type\n",
                      dt_name_get_name(user_type));
        return NULL;
    }
    else if (DT_IS_ALIAS(type))
    {
        return dt_alias_get_aliased_type(DT_ALIAS(type));
    }

    return DT_DATA_TYPE(type);
}

static DtDataType *
validate_type(compilation_status_t *compile_status,
              sym_table_t *sym_table,
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
        validated_type = resolve_user_type(compile_status,
                                           sym_table,
                                           DT_NAME(type));
    }
    else if (DT_IS_POINTER(type))
    {
        DtDataType *base_type;
        DtPointer *ptr_type = DT_POINTER(type);

        base_type = validate_type(compile_status,
                                  sym_table,
                                  dt_pointer_get_base_type(ptr_type));
        if (base_type == NULL)
        {
            return NULL;
        }

        dt_pointer_set_base_type(ptr_type, base_type);
    }
    else if (DT_IS_STATIC_ARRAY_TYPE(type))
    {
        if (dt_data_type_get_size(type) > DT_STATIC_ARRAY_MAX_SIZE)
        {
            compile_error(compile_status,
                          type,
                          "static array length %d exceeds limit\n",
                          dt_static_array_get_length(DT_STATIC_ARRAY(type)));
            return NULL;
        }
    }

    return validated_type;
}


static IrVariable *
validate_variable(compilation_status_t *compile_status,
                  sym_table_t *sym_table,
                  IrVariable *variable)
{
    assert(compile_status);
    assert(IR_IS_VARIABLE(variable));

    DtDataType *type;

    /*
     * validate variable's type
     */
    type = validate_type(compile_status,
                         sym_table,
                         ir_variable_get_data_type(variable));
    if (type == NULL)
    {
        /* invalid type */
        return NULL;
    }
    ir_variable_set_data_type(variable, type);

    /*
     * validate variable's init expression
     */
    IrExpression *initializer = ir_variable_get_initializer(variable);
    if (initializer != NULL)
    {
        assert(sym_table);
        initializer =
            validate_expression(compile_status, sym_table, initializer);

        if (initializer == NULL)
        {
            /*
             * initializer expression is invalid
             */
            return NULL;
        }

        IrExpression *conv_initializer =
            types_implicit_conv(type, initializer);
        if (conv_initializer == NULL && DT_IS_ARRAY(type))
        {
            /*
             * Handle the special case of array initialization from a scalar,
             * check if scalar can be implicitly converted to array's
             * element data type
             */
            DtDataType *array_element_type;

            array_element_type = dt_array_get_element_type(dt_array(type));
            conv_initializer =
                types_implicit_conv(array_element_type, initializer);
        }
        if (conv_initializer == NULL)
        {
            compile_error(compile_status,
                          variable,
                          "illegal type in initializer assignment\n");
            return NULL;
        }
        ir_variable_set_initializer(variable, conv_initializer);
    }
    else
    {
        /* default init expression */
        ir_variable_set_initializer(variable,
                                    dt_data_type_get_init(type));
    }

    return variable;
}

static void
validate_code_block(compilation_status_t *compile_status,
                    IrCodeBlock *code_block)
{
    sym_table_t *sym_table;
    GSList *i;
    GSList *local_vars;
    bool add_vars_to_frame = true;

    sym_table = ir_code_block_get_symbols(code_block);

    /*
     * validate default initializer expressions of local variables
     * in this code block
     */
    local_vars = ir_code_block_get_local_vars(code_block);

    for (i = local_vars; i != NULL; i = g_slist_next(i))
    {
        if (validate_variable(compile_status,
                              sym_table,
                              ir_variable(i->data)) == NULL)
        {
            /* invalid variable definition */
            add_vars_to_frame = false;
            continue;
        }
    }

    /*
     * if no errors are detected so far,
     * add local variables for function frame
     */
    if (add_vars_to_frame)
    {
        for (i = local_vars; i != NULL; i = g_slist_next(i))
        {
            IrVariable *variable = ir_variable(i->data);
            add_to_func_frame(compile_status->iml_func,
                              variable,
                              false);

            /* generate iml code for default initialization of the variable */
            IrVarValue *var_val =
                ir_var_value_new(variable, ir_node_get_line_num(variable));
            IrExpression *init_exp = ir_variable_get_initializer(variable);
            ir_module_add_const_data(compile_status->module, init_exp);
            iml_add_assignment(compile_status->iml_func,
                               ir_expression(var_val),
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
            validate_code_block(compile_status, ir_code_block(i->data));
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
    sym_table_t *symbols =
        ir_module_get_symbols(
            ir_symbol_get_parent_module(ir_symbol(func_decl)));
    type = validate_type(compile_status,
                         symbols,
                         ir_function_get_return_type(ir_function(func_decl)));
    if (type != NULL)
    {
        ir_function_set_return_type(ir_function(func_decl),
                                    type);
    }

    /* validate parameters */
    GSList *i = ir_function_get_parameters(ir_function(func_decl));
    for (; i != NULL; i = g_slist_next(i))
    {
        validate_variable(compile_status, symbols, ir_variable(i->data));
    }
}

static void
assign_registers(iml_function_t *func, arch_backend_t *backend)
{
    GSList *scratch_regs;
    GSList *preserved_regs;
    GSList *regs; /* all available registers */
    GSList *i;
    GSList *vars;
    GSList *used_regs = NULL;

    backend->get_registers(&scratch_regs, &preserved_regs);

    /* only use preserved register for now */
    regs = preserved_regs;

    /* assign registers to variables */
    vars = iml_function_get_locals(func);
    for (i = vars; i != NULL && regs != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = iml_variable(i->data);

        if (iml_variable_is_mem_pinned(var) ||
            iml_variable_get_data_type(var) == iml_blob)
        {
            /* don't assign registers to memory pinned and blob variables */
            continue;
        }

        const char *reg = regs->data;

        /* remove register from the list of available */
        regs = g_slist_remove(regs, reg);

        /* assign the register to the variable */
        iml_variable_set_register(var, reg);
        used_regs = g_slist_prepend(used_regs, (gpointer)reg);
    }

    /* store the list of used preserved registers */
    iml_function_set_used_regs(func, used_regs);

    /*
     * call the backend hook for
     * assigning locations to this frames variables
     */
    backend->assign_var_locations(func);
}

static void
validate_function_def(compilation_status_t *compile_status,
                      IrFunctionDef *func_def)
{
    assert(compile_status);
    assert(IR_IS_FUNCTION_DEF(func_def));

    IrCodeBlock *body;
    DtDataType *type;
    IrFunction *func = ir_function(func_def);

    compile_status->function = func_def;
    compile_status->iml_func =
        iml_function_new(compile_status->module,
                           ir_function_get_linkage(func));

    sym_table_t *symbols =
        ir_module_get_symbols(
            ir_symbol_get_parent_module(ir_symbol(func_def)));

    /*
     * validate parameter definitions and add parameters to function frame
     */
    GSList *i = ir_function_def_get_parameters(func_def);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrVariable *var;

        var = validate_variable(compile_status, symbols, ir_variable(i->data));
        if (var == NULL)
        {
            /* invalid parameter */
            continue;
        }
        /* Add to function frame */
        add_to_func_frame(compile_status->iml_func, var, true);
    }

    /* resolve possible user types in function return type */
    type = ir_function_def_get_return_type(func_def);
    type = validate_type(compile_status,
                         symbols,
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
    if (DT_IS_VOID(ir_function_def_get_return_type(func_def)))
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
            iml_function_add_operation(compile_status->iml_func,
                                       iml_operation_new(iml_return, NULL));
        }
    }

    if (compile_status->errors_count == 0)
    {
        iml_function_set_name(compile_status->iml_func,
                              ir_function_get_mangled_name(func));

        /* do registers assignment */
        assign_registers(compile_status->iml_func,
                         compile_status->backend);

        /* store generated IML function in module object */
        ir_module_add_function(compile_status->module,
                               compile_status->iml_func);
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
        exp = ir_expression(ir_basic_constant_new_uint(0, 0));
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
          exp = ir_expression(ir_basic_constant_new_uint(end_idx, 0));
        }
        else
        {
           /* slice over dynamic array */
           exp = ir_expression(
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

    return ir_expression(array_slice);
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
                             ir_expression(array_literal));

    return ir_expression(array_literal);
}

static void
validate_enum(compilation_status_t *compile_status,
              DtEnum *enum_def)
{
    assert(dt_is_enum(enum_def));

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
        one = ir_expression(ir_basic_constant_new_int(1, 0));
    }

    sym_table = ir_module_get_symbols(compile_status->module);
    members = dt_enum_get_members(enum_def);
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
    base_type = dt_enum_get_base_type(enum_def);
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
    }
    else if (DT_IS_NAME(base_type))
    {
        /* resolved base user type */
        base_type = resolve_user_type(compile_status,
                                      sym_table,
                                      DT_NAME(base_type));
        if (base_type == NULL)
        {
            /* invalid user type, bail out */
            return;
        }
    }
    dt_enum_set_base_type(enum_def, base_type);

    /*
     * figure out enum members values
     */
    if (first_member_init == NULL)
    {
        prev_member_value =
            cfold_cast(
                ir_cast_new(base_type,
                            ir_expression(ir_basic_constant_new_int(0, 0)),
                            0));
        ir_enum_member_set_value(members->data, prev_member_value);
    }

    for (i = members; i != NULL; i = g_slist_next(i))
    {
        IrEnumMember *member = ir_enum_member(i->data);
        IrExpression *value = ir_enum_member_get_value(member);

        if (value == NULL)
        {
            /* calculate next member value by increasing previous with 1 */
            value =
                validate_binary_op(
                    compile_status,
                    sym_table,
                        ir_binary_operation_new(
                            op_plus,
                            prev_member_value,
                            one,
                        0));
            value = cfold_cast(ir_cast_new(base_type, value, 0));

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
    sym_table_t *sym_table = ir_module_get_symbols(compile_status->module);

    /* validate struct members */
    struct_type = ir_struct_get_data_type(ir_struct);
    for (i = ir_struct_get_members(ir_struct); i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_VARIABLE(i->data));

        IrVariable *var = validate_variable(compile_status,
                                            sym_table,
                                            ir_variable(i->data));
        if (var == NULL)
        {
            /* invalid member, skip to next member */
            continue;
        }

        dt_struct_add_member(struct_type, var);
    }

    /* validate struct methods */
    for (i = ir_struct_get_methods(ir_struct); i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_FUNCTION_DEF(i->data));

        IrFunctionDef *method = ir_function_def(i->data);

        ir_function_def_set_this_type(method, DT_DATA_TYPE(struct_type));
        validate_function_def(compile_status, method);
        dt_struct_add_method(struct_type, method);
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

    main_func = ir_function(main_symb);
    if (ir_function_get_parameters(main_func) != NULL)
    {
        compile_error(compile_status,
                      IR_NODE(main_func),
                      "only void main() and int main() "
                      "entry points supported\n");
        return;
    }

    main_ret_type = ir_function_get_return_type(main_func);
    if (!DT_IS_VOID(main_ret_type) &&
        !dt_basic_is_int(main_ret_type))
    {
       compile_error(compile_status,
                     IR_NODE(main_func),
                     "function main() must return int or void\n");
    }
}

static void
validate_user_types(compilation_status_t *compile_status, IrModule *module)
{
    assert(compile_status);
    assert(IR_IS_MODULE(module));

    /* validate enum definitions */
    GSList *i;
    for (i = ir_module_get_enums(module); i != NULL; i = g_slist_next(i))
    {
        assert(dt_is_enum(i->data));
        validate_enum(compile_status, dt_enum(i->data));
    }

    /* validate struct definitions */
    for (i = ir_module_get_structs(module); i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_STRUCT(i->data));
        validate_struct(compile_status, IR_STRUCT(i->data));
    }
}

static void
validate_func_decls(compilation_status_t *compile_status,
                    IrModule *module)
{
    GSList *i;

    i = ir_module_get_function_decls(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        validate_function_decl(compile_status, IR_FUNCTION_DECL(i->data));
    }
}

static void
validate_func_defs(compilation_status_t *compile_status,
                   IrModule *module)
{
    GSList *i;

    i = ir_module_get_function_defs(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        validate_function_def(compile_status, i->data);
    }
}

static void
validate_imports(compilation_status_t *compile_status, IrModule *module)
{
    assert(compile_status);
    assert(IR_IS_MODULE(module));

    GSList *i;

    for (i = ir_module_get_imports(module); i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_MODULE(i->data));
        compile_status->module = i->data;
        validate_user_types(compile_status, i->data);
        validate_func_decls(compile_status, i->data);
        validate_func_defs(compile_status, i->data);
    }

    compile_status->module = module;
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

    compile_status->module = module;

    validate_imports(compile_status, module);
    validate_entry_point(compile_status, module);
    validate_user_types(compile_status, module);
    validate_func_decls(compile_status, module);
    validate_func_defs(compile_status, module);
}
