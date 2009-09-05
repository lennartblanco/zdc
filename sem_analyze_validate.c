#include <stdbool.h>

#include "sem_analyze_validate.h"
#include "types.h"
#include "ast_basic_type.h"
#include "ir_function.h"
#include "ir_assigment.h"
#include "ir_function_call.h"
#include "ir_return.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_cast.h"
#include "ir_if_else.h"
#include "ir_while.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
sem_analyze_validate_function(compilation_status_t *compile_status,
                              IrFunction *func);

static IrExpression *
sem_analyze_validate_expression(compilation_status_t *compile_status,
                                sym_table_t *sym_table,
                                IrExpression *expression);

static void
sem_analyze_validate_code_block(compilation_status_t *compile_status,
                                IrCodeBlock *code_block);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
sem_analyze_validate(compilation_status_t *compile_status,
                     IrCompileUnit *compile_unit)
{
    assert(compile_status);
    assert(compile_unit);

    sym_table_t *sym_table;
    GList *symbols_list;
    GList *i;

    sym_table = ir_compile_unit_get_symbols(compile_unit);
    symbols_list = sym_table_get_all_symbols(sym_table);

    for (i = symbols_list; i != NULL; i = g_list_next(i))
    {
        if (IR_IS_FUNCTION(i->data))
        {
            sem_analyze_validate_function(compile_status, IR_FUNCTION(i->data));
        }
        else
        {
            /* unexpected symbol type */
            assert(false);
        }
    }
    g_list_free(symbols_list);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/


static void
sem_analyze_validate_function_call(compilation_status_t *compile_status,
                                   sym_table_t *sym_table,
                                   IrFunctionCall *func_call)
{
    IrSymbol *func_symb;
    char *func_name;
    AstDataType *func_return_type;
    GSList *formal_args;
    GSList *func_call_args;
    GSList *validated_args = NULL;
    GSList *i;


    func_name = ir_function_call_get_name(func_call);

    /* look-up function in the symbol table */
    func_symb = sym_table_get_symbol(sym_table, func_name);
    if (func_symb == NULL)
    {
        compile_error(compile_status,
                      "reference to unknow function '%s'\n",
                      func_name);
        return;
    }
    if (!IR_IS_FUNCTION(func_symb))
    {
        compile_error(compile_status,
                      "called object '%s' is not a function\n",
                      func_name);
        return; 
    }

   formal_args = ir_function_get_parameters(IR_FUNCTION(func_symb));
   func_call_args = ir_function_call_get_arguments(func_call);

   /*
    * check that function call have correct number of arguments
    */
   if (g_slist_length(formal_args) != g_slist_length(func_call_args))
   {
       compile_error(compile_status, 
                     "invalid call to function '%s', expected %d "
                     "arguments, got %d\n",
                     func_name,
                     g_slist_length(formal_args),
                     g_slist_length(func_call_args));
       return;
   }

   /*
    * validate function call arguments
    */
   for (i = func_call_args; i != NULL; i = g_slist_next(i))
   {
       IrExpression *exp;

       exp = sem_analyze_validate_expression(compile_status,
                                             sym_table,
                                             IR_EXPRESSION(i->data));
       validated_args = g_slist_prepend(validated_args, exp);
   }
   /* store validated call arguments */
   ir_function_call_set_arguments(func_call, g_slist_reverse(validated_args));

   /* store function call data type */
   func_return_type = ir_function_get_return_type(IR_FUNCTION(func_symb));
   ir_function_call_set_return_type(func_call, func_return_type);
}

/**
 * validate binary integer arithmetic operation
 */
static IrExpression *
sem_analyze_validate_bin_iarithm(compilation_status_t *compile_status,
                                 sym_table_t *sym_table,
                                 IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_iarithm(bin_op));

    IrExpression *exp;


    /*
     * integer promote left operand
     */
    exp = ir_binary_operation_get_left(bin_op);
    exp = types_integer_promotion(exp);
    if (exp == NULL)
    {
        compile_error(compile_status, "left operand of illegal type\n");
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
        compile_error(compile_status, "right operand of illegal type\n");
        return NULL;
    }
    ir_binary_operation_set_right(bin_op, exp);

    return IR_EXPRESSION(bin_op);
}

/**
 * validate binary integer comparison operation
 */
static IrExpression *
sem_analyze_validate_bin_icomp(compilation_status_t *compile_status,
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
        compile_error(compile_status, "illegal types in compare operation\n");
        return NULL;
    }

    ir_binary_operation_set_left(bin_op, converted_left);
    ir_binary_operation_set_right(bin_op, converted_right);

    return IR_EXPRESSION(bin_op);
}

/**
 * validate binary conditional operation
 */
static IrExpression *
sem_analyze_validate_bin_conditional(compilation_status_t *compile_status,
                                     sym_table_t *sym_table,
                                     IrBinaryOperation *bin_op)
{
    assert(ir_binary_operation_is_conditional(bin_op));

    AstDataType *data_type;
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
        compile_error(compile_status, "left operand can not be of void type\n");
        return NULL;
    }

    /* if left operand is not bool, cast to bool */
    if (!types_is_bool(data_type))
    {
        left =
            IR_EXPRESSION(
              ir_cast_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                          left));
    }

    /*
     * check right operand data type
     */
    data_type = ir_expression_get_data_type(right);


    /* if right operand is not of void or bool type, cast to bool */
    if (!types_is_void(data_type) &&
        !types_is_bool(data_type))
    {
        right =
            IR_EXPRESSION(
              ir_cast_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                          right));
    }

    ir_binary_operation_set_left(bin_op, left);
    ir_binary_operation_set_right(bin_op, right);


    
    return IR_EXPRESSION(bin_op);
}

static IrExpression *
sem_analyze_validate_binary_op(compilation_status_t *compile_status,
                               sym_table_t *sym_table,
                               IrBinaryOperation *bin_op)
{
    IrExpression *exp;

    /* validate left operand */
    exp = ir_binary_operation_get_left(bin_op);
    exp = sem_analyze_validate_expression(compile_status,
                                          sym_table,
                                          exp);
    ir_binary_operation_set_left(bin_op, exp);

    /* validate right operand */
    exp = ir_binary_operation_get_right(bin_op);
    exp = sem_analyze_validate_expression(compile_status,
                                          sym_table,
                                          exp);
    ir_binary_operation_set_right(bin_op, exp);

    if (ir_binary_operation_is_iarithm(bin_op))
    {
        return 
          sem_analyze_validate_bin_iarithm(compile_status,
                                           sym_table,
                                           bin_op);
    }
    else if (ir_binary_operation_is_icomp(bin_op))
    {
        return
          sem_analyze_validate_bin_icomp(compile_status,
                                         sym_table,
                                         bin_op);
    }
    else if (ir_binary_operation_is_conditional(bin_op))
    {
        return
          sem_analyze_validate_bin_conditional(compile_status,
                                               sym_table,
                                               bin_op);
    }

    /* unexpected binary operation type */
    assert(false);
}

static IrExpression *
sem_analyze_validate_unary_op(compilation_status_t *compile_status,
                              sym_table_t *sym_table,
                              IrUnaryOperation *operation)
{
    IrExpression *exp;

    exp = ir_unary_operation_get_operand(operation);
    exp = sem_analyze_validate_expression(compile_status,
                                          sym_table,
                                          exp);

    switch (ir_unary_operation_get_operation(operation))
    {
        case ast_arithm_neg_op:
            exp = types_implicit_conv(types_get_int_type(), exp);
            if (exp == NULL)
            {
                compile_error(compile_status,
                              "can not convert to int type");
                return NULL;
            }
            break;
        case ast_bool_neg_op:
            exp = types_implicit_conv(types_get_bool_type(), exp);
            if (exp == NULL)
            {
                compile_error(compile_status,
                              "can not convert to bool type");
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
sem_analyze_validate_expression(compilation_status_t *compile_status,
                                sym_table_t *sym_table,
                                IrExpression *expression)
{
    if (IR_IS_BINARY_OPERATION(expression))
    {
        expression = 
            sem_analyze_validate_binary_op(compile_status,
                                           sym_table,
                                           IR_BINARY_OPERATION(expression));
    }
    else if (IR_IS_UNARY_OPERATION(expression))
    {
        expression =
            sem_analyze_validate_unary_op(compile_status,
                                          sym_table,
                                          IR_UNARY_OPERATION(expression));
    }
    else if (IR_IS_FUNCTION_CALL(expression))
    {
        sem_analyze_validate_function_call(compile_status,
                                           sym_table,
                                           IR_FUNCTION_CALL(expression));
    }

    return expression;
}

static void
sem_analyze_validate_return(compilation_status_t *compile_status,
                            sym_table_t *sym_table,
                            IrReturn *ret)
{
    IrExpression *ret_exp;

    ret_exp = ir_return_get_return_value(ret);

    if (ret_exp != NULL)
    {
        IrExpression *exp;
        exp = 
           sem_analyze_validate_expression(compile_status, sym_table, ret_exp);
        ir_return_set_return_value(ret, exp);
    }
}

static void
sem_analyze_validate_assigment(compilation_status_t *compile_status,
                               sym_table_t *sym_table,
                               IrAssigment *assigment)
{
    AstVariableRef *target;
    IrExpression *value;
    IrSymbol *target_sym;
    AstDataType *target_type;

    target = ir_assigment_get_target_ref(assigment);

    target_sym =
        sym_table_get_symbol(sym_table, ast_variable_ref_get_name(target));
    if (target_sym == NULL)
    {
        compile_error(compile_status,
                      "'%s' undeclared\n",
                      ast_variable_ref_get_name(target));
        return;
    }
    if (!IR_IS_VARIABLE(target_sym))
    {
        compile_error(compile_status,
                      "can not assign value to '%s', not a variable\n",
                      ast_variable_ref_get_name(target));
        return;
    }

    value = ir_assigment_get_value(assigment);
    value = sem_analyze_validate_expression(compile_status, sym_table, value);


    target_type = ir_expression_get_data_type(IR_EXPRESSION(target_sym));

    value = types_implicit_conv(target_type, value);
    if (value == NULL)
    {
        compile_error(compile_status,
                      "incompatible types in assigment to '%s'\n",
                      ast_variable_ref_get_name(target));
        return;
    }

    ir_assigment_set_target(assigment, IR_VARIABLE(target_sym));
    ir_assigment_set_value(assigment, value);
}

static void
sem_analyze_validate_if_block(compilation_status_t *compile_status,
                             sym_table_t *sym_table,
                             IrIfBlock *if_block)
{
    IrExpression *condition;
    IrCodeBlock *body;

    /* validate if condition expression */
    condition = ir_if_block_get_condition(if_block);
    condition = sem_analyze_validate_expression(compile_status,
                                                sym_table,
                                                condition);
    /* insert implicit conversion to boolean type */
    condition = types_implicit_conv(types_get_bool_type(),
                                    condition);

    if (condition == NULL)
    {
        compile_error(compile_status,
                      "can not convert if condition to bool type\n");
        return;
    }
    ir_if_block_set_condition(if_block, condition);

    /* validate if body */
    body = ir_if_block_get_body(if_block);
    sem_analyze_validate_code_block(compile_status,
                                    body);
}

static void
sem_analyze_validate_if_else(compilation_status_t *compile_status,
                             sym_table_t *sym_table,
                             IrIfElse *if_else)
{
   GSList *i;
   IrCodeBlock *else_body;
   

   /* validate if-blocks */
   i = ir_if_else_get_if_else_blocks(if_else);
   for (; i != NULL; i = g_slist_next(i))
   {
       sem_analyze_validate_if_block(compile_status,
                                     sym_table,
                                     IR_IF_BLOCK(i->data));
   }

   /* validate else-body */
   else_body = ir_if_else_get_else_body(if_else);
   if (else_body != NULL)
   {
       sem_analyze_validate_code_block(compile_status,
                                       else_body);
   }
}

static void
sem_analyze_validate_while(compilation_status_t *compile_status,
                           sym_table_t *sym_table,
                           IrWhile *while_statment)
{
    IrExpression *condition;
    IrCodeBlock *body;

    /* validate if condition expression */
    condition = ir_while_get_loop_condition(while_statment);
    condition = sem_analyze_validate_expression(compile_status,
                                                sym_table,
                                                condition);

    /* insert implicit conversion to boolean type */
    condition = types_implicit_conv(types_get_bool_type(),
                                    condition);

    if (condition == NULL)
    {
        compile_error(compile_status,
                      "can not convert while loop condition to bool type\n");
        return;
    }
    ir_while_set_loop_condition(while_statment, condition);

    /* validate if body */
    body = ir_while_get_body(while_statment);
    sem_analyze_validate_code_block(compile_status, body);
}


static void
sem_analyze_validate_statment(compilation_status_t *compile_status,
                              sym_table_t *sym_table,
                              IrStatment *statment)
{
    if (IR_IS_FUNCTION_CALL(statment))
    {
         sem_analyze_validate_function_call(compile_status,
                                            sym_table,
                                            IR_FUNCTION_CALL(statment));
    }
    else if (IR_IS_ASSIGMENT(statment))
    {
        sem_analyze_validate_assigment(compile_status,
                                       sym_table,
                                       IR_ASSIGMENT(statment));
    }
    else if (IR_IS_RETURN(statment))
    {
        sem_analyze_validate_return(compile_status,
                                    sym_table,
                                    IR_RETURN(statment));
    }
    else if (IR_IS_IF_ELSE(statment))
    {
        sem_analyze_validate_if_else(compile_status,
                                     sym_table,
                                     IR_IF_ELSE(statment));
    }
    else if (IR_IS_WHILE(statment))
    {
        sem_analyze_validate_while(compile_status,
                                   sym_table,
                                   IR_WHILE(statment));
    }
    else if (IR_IS_EXPRESSION(statment))
    {
        compile_error(compile_status, "expression have no effect\n");
    }
}

static void
sem_analyze_validate_code_block(compilation_status_t *compile_status,
                                IrCodeBlock *code_block)
{
    sym_table_t *sym_table;
    GSList *i;

    sym_table = ir_code_block_get_symbols(code_block);

    i = ir_code_block_get_statments(code_block);
    for (; i != NULL; i = g_slist_next(i))
    {
        if (IR_IS_CODE_BLOCK(i->data))
        {
            sem_analyze_validate_code_block(compile_status,
                                            IR_CODE_BLOCK(i->data));
        }
        else if (IR_IS_STATMENT(i->data))
        {
            sem_analyze_validate_statment(compile_status,
                                          sym_table,
                                          IR_STATMENT(i->data));
        }
        else
        {
            /* unexpected statment type */
            assert(false);
        }
    }
}

static void
sem_analyze_validate_function(compilation_status_t *compile_status,
                              IrFunction *func)
{
    IrCodeBlock *body;

    body = ir_function_get_body(func);

    /* validate function's body */
    sem_analyze_validate_code_block(compile_status, body);

    /*
     * For void function, add an implicit return statment if needed
     */
    if (types_is_void(ir_function_get_return_type(func)))
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
            ir_code_block_add_statment(body, ir_return_new(NULL));
        }
    }
}

