#include <stdio.h>
#include <stdbool.h>

#include "sem_analyze.h"
#include "types.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_assigment.h"
#include "ast_basic_type.h"
#include "ast_static_array_type.h"
#include "ast_if_else.h"
#include "ast_array_slice_ref.h"
#include "ast_while.h"
#include "ast_foreach.h"
#include "ast_return.h"
#include "ast_int_constant.h"
#include "ast_bool_constant.h"
#include "ast_unary_operation.h"
#include "ast_binary_operation.h"
#include "ast_unary_operation.h"
#include "ast_function_call.h"
#include "ir_int_constant.h"
#include "ir_bool_constant.h"
#include "ir_if_else.h"
#include "ir_if_block.h"
#include "ir_variable.h"
#include "ir_function.h"
#include "ir_while.h"
#include "ir_foreach.h"
#include "ir_return.h"
#include "ir_cast.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_assigment.h"
#include "ir_function_call.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
sem_analyze_ast_code_block_to_ir(compilation_status_t *compile_status,
                                 AstCodeBlock *ast_code_block,
                                 IrCodeBlock *ir_code_block);

static IrStatment *
sem_analyze_ast_while_to_ir(compilation_status_t *compile_status,
                            sym_table_t *parent_symbols,
                            AstWhile *ast_while);

static IrStatment *
sem_analyze_ast_foreach_to_ir(compilation_status_t *compile_status,
                              sym_table_t *parent_symbols,
                              AstForeach *ast_foreach);

static IrStatment *
sem_analyze_ast_return_to_ir(compilation_status_t *compile_status,
                             sym_table_t *symbols,
                             AstReturn *ast_return);

static IrStatment *
sem_analyze_ast_assigment_to_ir(compilation_status_t *compile_status,
                                sym_table_t *symbols,
                                AstAssigment *ast_assigment);

static IrExpression *
sem_analyze_ast_expression_to_ir(compilation_status_t *compile_status,
                                 sym_table_t *symbols,
                                 AstExpression *ast_expression);

static IrExpression *
sem_analyze_arithmetic_binary_op(compilation_status_t *compile_status,
                                 ast_binary_op_type_t operation,
                                 IrExpression *left,
                                 IrExpression *right);

static IrExpression *
sem_analyze_ast_binary_op_to_ir(compilation_status_t *compile_status,
                                 sym_table_t *symbols,
                                 AstBinaryOperation *ast_operation);

static IrExpression *
sem_analyze_ast_func_call_to_ir(compilation_status_t *compile_status,
                                sym_table_t *symbols,
                                AstFunctionCall *func_call);

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

/**
 * convert AST if block to IR form.
 */
static IrIfBlock *
sem_analyze_ast_if_block_to_ir(compilation_status_t *compile_status,
                               IrCodeBlock *parent_block,
                               AstIfBlock *ast_if_block)
{
    sym_table_t *sym_table = ir_code_block_get_symbols(parent_block);
    AstExpression *ast_condition = ast_if_block_get_condition(ast_if_block);

    IrCodeBlock *body = 
        ir_code_block_new(ir_code_block_get_symbols(parent_block));

    IrExpression *condition =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         sym_table,
                                         ast_condition);

    sem_analyze_ast_code_block_to_ir(compile_status,
                                     ast_if_block_get_body(ast_if_block),
                                     body);

    return ir_if_block_new(condition, body);
}

/**
 * convert AST if_else statment to IR form.
 */
static IrStatment * 
sem_analyze_ast_if_else_to_ir(compilation_status_t *compile_status,
                              IrCodeBlock *parent_block,
                              AstIfElse *ast_if_else)
{
    IrIfElse *ifelse;
    GSList   *i;

    ifelse = ir_if_else_new();

    /* convert AST if-blocks to ir form */
    i = ast_if_else_get_if_else_blocks(ast_if_else);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrIfBlock *ifblock =
               sem_analyze_ast_if_block_to_ir(compile_status,
                                              parent_block,
                                              XDP_AST_IF_BLOCK(i->data));
        ir_if_else_add_if_else_block(ifelse, ifblock);
    }

    /* convert else-body code block to ir form, if else block is present */
    AstCodeBlock *ast_else_body = ast_if_else_get_else_block(ast_if_else);
    if (ast_else_body != NULL)
    {
        IrCodeBlock *else_body =
            ir_code_block_new(ir_code_block_get_symbols(parent_block));

        sem_analyze_ast_code_block_to_ir(compile_status,
                                         ast_else_body,
                                         else_body);

        ir_if_else_set_else_body(ifelse, else_body);
    }

    return IR_STATMENT(ifelse);
}

/**
 * Convert AST while statment to IR form.
 */
static IrStatment *
sem_analyze_ast_while_to_ir(compilation_status_t *compile_status,
                            sym_table_t *parent_symbols,
                            AstWhile *ast_while)
{
    IrCodeBlock *body = ir_code_block_new(parent_symbols);

    sem_analyze_ast_code_block_to_ir(compile_status,
                                     ast_while_get_body(ast_while),
                                     body);    

    return 
       IR_STATMENT(ir_while_new(ast_while_get_loop_condition(ast_while), body));
}

static IrStatment *
sem_analyze_ast_foreach_to_ir(compilation_status_t *compile_status,
                              sym_table_t *parent_symbols,
                              AstForeach *ast_foreach)
{
    IrForeach *foreach;
    IrVariable *ir_index = NULL;
    IrVariable *ir_value;
    AstVariableDeclaration *var;
    sym_table_t *loop_symbols;
    IrCodeBlock *body;

    var = ast_foreach_get_index(ast_foreach);
    if (var != NULL)
    {
        ir_index = 
          ir_variable_new(ast_variable_declaration_get_data_type(var),
                          ast_variable_declaration_get_name(var),
                          NULL);
    }

    var = ast_foreach_get_value(ast_foreach);
    ir_value =
        ir_variable_new(ast_variable_declaration_get_data_type(var),
                        ast_variable_declaration_get_name(var),
                        NULL);

    loop_symbols = sym_table_new(parent_symbols);

    if (ir_index != NULL)
    {
        sym_table_add_symbol(loop_symbols, IR_SYMBOL(ir_index));
    }

    sym_table_add_symbol(loop_symbols, IR_SYMBOL(ir_value));

    body = ir_code_block_new(loop_symbols);

    sem_analyze_ast_code_block_to_ir(compile_status,
                                     ast_foreach_get_body(ast_foreach),
                                     body);

    foreach = ir_foreach_new(ir_index, ir_value,
                             ast_foreach_get_aggregate(ast_foreach), body);

    return IR_STATMENT(foreach);
}

/**
 * Convert AST return statment to IR form.
 */
static IrStatment *
sem_analyze_ast_return_to_ir(compilation_status_t *compile_status,
                             sym_table_t *symbols,
                             AstReturn *ast_return)
{
  IrExpression *ir_return_value = NULL;
  AstExpression *ast_return_value;

  ast_return_value = ast_return_get_return_value(ast_return);
  if (ast_return_value != NULL)
  {
      ir_return_value =
          sem_analyze_ast_expression_to_ir(compile_status,
                                           symbols,
                                           ast_return_value); 
  }

  return IR_STATMENT(ir_return_new(ir_return_value));
}

static IrStatment *
sem_analyze_ast_assigment_to_ir(compilation_status_t *compile_status,
                                sym_table_t *symbols,
                                AstAssigment *ast_assigment)
{
    AstVariableRef *target;
    IrSymbol *target_sym;
    AstExpression *value;
    IrExpression *ir_value;
    AstDataType *target_type;

    target = ast_assigment_get_target(ast_assigment);
    value = ast_assigment_get_value(ast_assigment);

    target_sym =
        sym_table_get_symbol(symbols, ast_variable_ref_get_name(target));
    if (target_sym == NULL)
    {
        compile_error(compile_status,
                      "'%s' undeclared\n",
                      ast_variable_ref_get_name(target));
        return NULL;
    }
    if (!IR_IS_VARIABLE(target_sym))
    {
        compile_error(compile_status,
                      "can not assign value to '%s', not a variable\n",
                      ast_variable_ref_get_name(target));
        return NULL;
    }

    target_type = ir_expression_get_data_type(IR_EXPRESSION(target_sym));

    ir_value =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         symbols,
                                         value);

    ir_value = types_implicit_conv(target_type, ir_value);
    if (ir_value == NULL)
    {
        compile_error(compile_status,
                      "incompatible types in assigment to '%s'\n",
                      ast_variable_ref_get_name(target));
        return NULL;
    }

    return IR_STATMENT(ir_assigment_new(IR_VARIABLE(target_sym), ir_value));
}

/**
 * Build IR representation of an arithmetic binary operation. Adding type
 * conversion operations if left and right operands are of different types.
 *
 * The arithmetic operations are +, -, * and /.
 */
static IrExpression *
sem_analyze_arithmetic_binary_op(compilation_status_t *compile_status,
                                 ast_binary_op_type_t operation,
                                 IrExpression *left,
                                 IrExpression *right)
{
    assert(operation == ast_plus_op  ||
           operation == ast_minus_op ||
           operation == ast_mult_op  ||
           operation == ast_division_op);

    left = types_integer_promotion(left);
    if (left == NULL)
    {
        goto err_exit;
    }

    right = types_integer_promotion(right);
    if (right == NULL) 
    {
        goto err_exit;
    }

    return IR_EXPRESSION(ir_binary_operation_new(operation, left, right));

err_exit:
    compile_error(compile_status, "operand of illegal type\n");
    return NULL;
}

static IrExpression *
sem_analyze_compare_binary_op(compilation_status_t *compile_status,
                              ast_binary_op_type_t operation,
                              IrExpression *left,
                              IrExpression *right)
{
    assert(operation == ast_equal_op      ||
           operation == ast_not_equal_op  ||
           operation == ast_less_op       ||
           operation == ast_greater_op    ||
           operation == ast_less_or_eq_op ||
           operation == ast_greater_or_eq_op);

    IrExpression *converted_left;
    IrExpression *converted_right;

    if (!types_usual_arithm_conv(left,
                                 right,
                                 &converted_left,
                                 &converted_right))
    {
        compile_error(compile_status, "illegal types in compare operation\n");
    }

    return IR_EXPRESSION(ir_binary_operation_new(operation,
                                                 converted_left,
                                                 converted_right));
}

static IrExpression *
sem_analyze_conditional_binary_op(compilation_status_t *compile_status,
                                  ast_binary_op_type_t operation,
                                  IrExpression *left,
                                  IrExpression *right)
{
    assert(operation == ast_and_op || operation == ast_or_op);

    AstDataType *data_type;
    IrExpression *converted_left = left;
    IrExpression *converted_right = right;
   
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
        converted_left =
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
        converted_right =
            IR_EXPRESSION(
              ir_cast_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                          right));
    }
    
    return IR_EXPRESSION(ir_binary_operation_new(operation,
                                                 converted_left,
                                                 converted_right));

}

/**
 * Convert AST binary operation to IR form.
 */
static IrExpression *
sem_analyze_ast_binary_op_to_ir(compilation_status_t *compile_status,
                                 sym_table_t *symbols,
                                 AstBinaryOperation *ast_operation)
{
    IrExpression *left;
    IrExpression *right;
    ast_binary_op_type_t op;

    left =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         symbols,
                                         ast_binary_operation_get_left(ast_operation));
    right =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         symbols,
                                         ast_binary_operation_get_right(ast_operation));

    if (left == NULL || right == NULL)
    {
        return NULL;
    }

    op = ast_binary_operation_get_operation(ast_operation);
    switch (op) {
        case ast_plus_op:
        case ast_minus_op:
        case ast_mult_op:
        case ast_division_op:
            return sem_analyze_arithmetic_binary_op(compile_status, 
                                                    op, left, right);
        case ast_equal_op:
        case ast_not_equal_op:
        case ast_less_op:
        case ast_greater_op:
        case ast_less_or_eq_op:
        case ast_greater_or_eq_op:
            return sem_analyze_compare_binary_op(compile_status,
                                                 op, left, right);
        case ast_and_op:
        case ast_or_op:
            return sem_analyze_conditional_binary_op(compile_status,
                                                     op, left, right);
        default:
            /* unexpected binary operation */
            assert(false);
    }

    /* we should not get here */
    assert(false);
}


/**
 * Convert AST unary operation to IR form.
 */
static IrExpression *
sem_analyze_ast_unary_op_to_ir(compilation_status_t *compile_status,
                               sym_table_t *symbols,
                               AstUnaryOperation *ast_operation)
{
    IrExpression *operand;
    AstDataType *operand_type;
    basic_data_type_t operand_data_type;
    ast_unary_op_type_t op_type;

    operand =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         symbols,
                                         ast_unary_operation_get_operand(ast_operation));


    operand_type = ir_expression_get_data_type(operand);
    if (!(XDP_IS_AST_BASIC_TYPE(operand_type))) {
        compile_error(compile_status, "not a basic type\n");
        return NULL;
    }

    operand_data_type =
        ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(operand_type));

    op_type = ast_unary_operation_get_operation(ast_operation);
    switch (op_type)
    {
        case ast_arithm_neg_op:
            switch (operand_data_type)
            {
                case int_type:
                    /* nop */
                    break;
                case bool_type:
                    operand =
                      IR_EXPRESSION(
                        ir_cast_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                                    operand));
                    break;
                default:
                    /* unexpected operand data type */
                    assert(false);
            }
            break;
        case ast_bool_neg_op:
            switch (operand_data_type)
            {
                case bool_type:
                    /* nop */
                    break;
                case int_type:
                    operand =
                      IR_EXPRESSION(
                        ir_cast_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                                    operand));
                    break;
                default:
                    /* unexpected operand data type */
                    assert(false);
            }
            break;
        default:
            /* unexpected unary operation */
            assert(false);
    }

    return IR_EXPRESSION(ir_unary_operation_new(op_type, operand));
}

static IrExpression *
sem_analyze_ast_func_call_to_ir(compilation_status_t *compile_status,
                                sym_table_t *symbols,
                                AstFunctionCall *func_call)
{
    IrSymbol *func_symb;
    AstDataType *func_return_type;
    char *func_name;
    GSList *i;
    GSList *ir_call_args = NULL;

    func_name = ast_function_call_get_name(func_call);

    /* look-up function in the symbol table */
    func_symb = sym_table_get_symbol(symbols, func_name);
    if (func_symb == NULL)
    {
        compile_error(compile_status,
                      "reference to unknow function '%s'\n",
                      func_name);
        return NULL;
    }
    if (!IR_IS_FUNCTION(func_symb))
    {
        compile_error(compile_status,
                      "called object '%s' is not a function\n",
                      func_name);
        return NULL;  
    }

    func_return_type = ir_function_get_return_type(IR_FUNCTION(func_symb));

    /* convert call argument expressions to IR form */
    i = ast_function_call_get_arguments(func_call);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrExpression *arg;

        arg = 
            sem_analyze_ast_expression_to_ir(compile_status,
                                             symbols,
                                             XDP_AST_EXPRESSION(i->data));
        ir_call_args = g_slist_prepend(ir_call_args, arg);
    }

    return IR_EXPRESSION(ir_function_call_new(func_return_type,
                                              func_name,
                                              ir_call_args));
}

/**
 * Convert AST expression to IR form.
 */
static IrExpression *
sem_analyze_ast_expression_to_ir(compilation_status_t *compile_status,
                                 sym_table_t *symbols,
                                 AstExpression *ast_expression)
{
    assert(compile_status);
    assert(symbols);
    assert(ast_expression);

    if (XDP_IS_AST_INT_CONSTANT(ast_expression))
    {
        gint32 val;

        val = ast_int_constant_get_value(XDP_AST_INT_CONSTANT(ast_expression));
        return IR_EXPRESSION(ir_int_constant_new(val));
    }
    else if (XDP_IS_AST_BOOL_CONSTANT(ast_expression))
    {
        gboolean val;

        val = 
            ast_bool_constant_get_value(XDP_AST_BOOL_CONSTANT(ast_expression));
        return IR_EXPRESSION(ir_bool_constant_new(val));
    }
    else if (XDP_IS_AST_VARIABLE_REF(ast_expression))
    {
        AstVariableRef *var_ref;
        IrSymbol *var_symb;

        /* look-up the variable in the symbol table */
        var_ref = XDP_AST_VARIABLE_REF(ast_expression);
        var_symb = sym_table_get_symbol(symbols,
                                        ast_variable_ref_get_name(var_ref));
        if (var_symb == NULL) 
        {
            compile_error(compile_status, 
                          "reference to unknow symbol '%s'\n",
                          ast_variable_ref_get_name(var_ref));
            return NULL;

        }
        else if (!IR_IS_VARIABLE(var_symb))
        {
            compile_error(compile_status,
                          "return expression symbol must be a variable\n");
            return NULL;
        }
        return IR_EXPRESSION(var_symb);
    }
    else if (XDP_IS_AST_UNARY_OPERATION(ast_expression))
    {
        AstUnaryOperation *op;

        op = XDP_AST_UNARY_OPERATION(ast_expression);

        return sem_analyze_ast_unary_op_to_ir(compile_status, 
                                              symbols, op);
    }
    else if (XDP_IS_AST_BINARY_OPERATION(ast_expression))
    {
        AstBinaryOperation *bin_op;

        bin_op = XDP_AST_BINARY_OPERATION(ast_expression);

        return sem_analyze_ast_binary_op_to_ir(compile_status, 
                                               symbols, bin_op);
    }
    else if (XDP_IS_AST_FUNCTION_CALL(ast_expression))
    {
        AstFunctionCall *func_call;

        func_call = XDP_AST_FUNCTION_CALL(ast_expression);
        return sem_analyze_ast_func_call_to_ir(compile_status,
                                               symbols,
                                               func_call);
    }

    /* unexpected expression type */
    printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(ast_expression)));
    g_assert_not_reached();
    return NULL;
}
/**
 * convert AST code block representation to IR form.
 */
static void
sem_analyze_ast_code_block_to_ir(compilation_status_t *compile_status,
                                 AstCodeBlock *ast_code_block,
                                 IrCodeBlock *ir_code_block)
{
    GSList      *i;
    sym_table_t *symbols;

    symbols = ir_code_block_get_symbols(ir_code_block);

    /*
     * iterate over statments in the AST code block
     * and convert them to IR form
     */
    i = ast_code_block_get_statments(ast_code_block);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrStatment *ir_stmt = NULL;
        AstStatment *stmt = XDP_AST_STATMENT(i->data);

        /* variable declaration found */
        if (XDP_IS_AST_VARIABLE_DEFINITION(stmt))
        {
            /*
             * put the variable definition into the local symbol table
             */
            AstVariableDefinition *var_def =
                XDP_AST_VARIABLE_DEFINITION(stmt);

            AstDataType *var_data_type =
                ast_variable_definition_get_data_type(var_def);

            AstExpression *ast_initializer =
                ast_variable_definition_get_initializer(var_def);

            IrExpression *initializer = NULL;

            if (ast_initializer != NULL)
            {
                initializer =
                    sem_analyze_ast_expression_to_ir(compile_status,
                                                     symbols,
                                                     ast_initializer);
                initializer = types_implicit_conv(var_data_type, initializer);
                if (initializer == NULL)
                {
                    compile_error(compile_status,
                                  "illegal types in initializer assigment\n");
                    return;
                }
            }

            IrVariable *sym = 
                ir_variable_new(var_data_type,
                                ast_variable_definition_get_name(var_def),
                                initializer);

            if (sym_table_add_symbol(symbols, IR_SYMBOL(sym)) != 0)
            {
                compile_error(compile_status, 
                              "redeclaration of symbol '%s'\n",
                              ir_symbol_get_name(IR_SYMBOL(sym)));
                return;
            }
            /** @todo delete the stmt node here */
            /* no IR statment to add, jump to next ast statment */
            continue;
        }
        /* sub-code block found */
        else if (XDP_IS_AST_CODE_BLOCK(stmt))
        {
            IrCodeBlock *sub_block =
                ir_code_block_new(symbols);

            sem_analyze_ast_code_block_to_ir(compile_status,
                                             XDP_AST_CODE_BLOCK(stmt),
                                             sub_block);
            ir_stmt = IR_STATMENT(sub_block);
        }
        else if (XDP_IS_AST_IF_ELSE(stmt))
        {
            ir_stmt =
                sem_analyze_ast_if_else_to_ir(compile_status,
                                              ir_code_block,
                                              XDP_AST_IF_ELSE(stmt));
        }
        else if (XDP_IS_AST_WHILE(stmt))
        {
            ir_stmt =
                sem_analyze_ast_while_to_ir(compile_status,
                                            symbols,
                                            XDP_AST_WHILE(stmt));
        }
        else if (XDP_IS_AST_FOREACH(stmt))
        {
            ir_stmt =
                sem_analyze_ast_foreach_to_ir(compile_status,
                                              symbols,
                                              XDP_AST_FOREACH(stmt));
        }
        else if (XDP_IS_AST_RETURN(stmt))
        {
            ir_stmt =
                sem_analyze_ast_return_to_ir(compile_status,
                                             symbols,
                                             XDP_AST_RETURN(stmt));
        }
        else if (XDP_IS_AST_FUNCTION_CALL(stmt))
        {
            ir_stmt =
                IR_STATMENT(
                  sem_analyze_ast_func_call_to_ir(compile_status,
                                                  symbols,
                                                  XDP_AST_FUNCTION_CALL(stmt)));
        }
        else if (XDP_IS_AST_ASSIGMENT(stmt))
        {
            ir_stmt =
                sem_analyze_ast_assigment_to_ir(compile_status,
                                                symbols,
                                                XDP_AST_ASSIGMENT(stmt));
        }
        else
        {
            /* unexpected statment type */
            assert(false);
        }

        if (ir_stmt == NULL)
        {
            /* semantic error found, bail out */
            return;
        }
        ir_code_block_add_statment(ir_code_block, ir_stmt);
        /** @todo delete the stmt node here */
    }
}

/**
 * convert AST representation of a function to IR form.
 */
static IrFunction *
sem_analyze_ast_func_to_ir(compilation_status_t *compile_status,
                           AstFunction *ast_func,
                           sym_table_t *global_sym_table)
{
    IrFunction *ir_func;

    ir_func = 
        ir_function_new(ast_function_get_return_type(ast_func),
                        ast_function_get_name(ast_func),
                        ast_function_get_parameters(ast_func),
                        global_sym_table);


    /* convert function body to ir format */
    sem_analyze_ast_code_block_to_ir(compile_status,
                                     ast_function_get_body(ast_func),
                                     ir_function_get_body(ir_func));

    return ir_func;
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrCompileUnit *
semantic_analyze(const char *source_file, AstCompileUnit *ast_compile_unit)
{
    IrCompileUnit        *comp_unit;
    GSList               *ptr;
    IrFunction           *ir_func;
    sym_table_t          *global_sym_table;
    compilation_status_t comp_stat;

    /* set-up compilation status struct */
    comp_stat.source_file = source_file;
    comp_stat.errors_count = 0;

    comp_unit = ir_compile_unit_new();
    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    ptr = ast_compile_unit_get_functions(ast_compile_unit);
    for (;ptr != NULL; ptr = ptr->next)
    {
        ir_func = sem_analyze_ast_func_to_ir(&comp_stat,
                                             XDP_AST_FUNCTION(ptr->data),
                                             global_sym_table);
        ir_compile_unit_add_function(comp_unit, ir_func);
    }

    /* if there were errors during analysis, return failure result */
    if (comp_stat.errors_count > 0)
    {
        /* @todo: clean-up comp_unit ? */
        return NULL;
    }
    return comp_unit;
}


