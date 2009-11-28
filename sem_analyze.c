#include <stdio.h>
#include <stdbool.h>

#include "sem_analyze.h"
#include "sem_analyze_validate.h"
#include "types.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_assigment.h"
#include "ast_if_else.h"
#include "ast_variable_ref.h"
#include "ast_array_slice_ref.h"
#include "ast_while.h"
#include "ast_foreach.h"
#include "ast_return.h"
#include "ast_array_literal.h"
#include "ast_int_constant.h"
#include "ast_uint_constant.h"
#include "ast_bool_constant.h"
#include "ast_unary_operation.h"
#include "ast_binary_operation.h"
#include "ast_unary_operation.h"
#include "ast_function_call.h"
#include "ast_array_cell_ref.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_bool_constant.h"
#include "ir_array_slice.h"
#include "ir_array_literal.h"
#include "ir_array_cell.h"
#include "ir_scalar.h"
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

static IrCompileUnit *
sem_analyze_ast_compile_unit_to_ir(compilation_status_t *compile_status,
                                   AstCompileUnit *ast_compile_unit);

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

static IrFunctionDecl *
sem_analyze_ast_func_decl_to_ir(AstFunctionDecl *ast_func_decl);

static IrFunctionDef *
sem_analyze_ast_func_def_to_ir(compilation_status_t *compile_status,
                               AstFunctionDef *ast_func_def,
                               sym_table_t *global_sym_table);

static IrExpression *
sem_analyze_ast_expression_to_ir(compilation_status_t *compile_status,
                                 sym_table_t *symbols,
                                 AstExpression *ast_expression);

static IrExpression *
sem_analyze_ast_binary_op_to_ir(compilation_status_t *compile_status,
                                 sym_table_t *symbols,
                                 AstBinaryOperation *ast_operation);

static IrExpression *
sem_analyze_ast_func_call_to_ir(compilation_status_t *compile_status,
                                sym_table_t *symbols,
                                AstFunctionCall *func_call);

static IrExpression *
sem_analyze_ast_array_literal_to_ir(compilation_status_t *compile_status,
                                    sym_table_t *symbols,
                                    AstArrayLiteral *ast_arry_literal);

static void
sem_analyze_ast_var_def_to_ir(compilation_status_t *compile_status,
                              AstVariableDefinition *var_def,
                              sym_table_t *sym_table);

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
    AstExpression *ast_condition = ast_while_get_loop_condition(ast_while);
    IrCodeBlock *body = ir_code_block_new(parent_symbols);

    IrExpression *condition =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         parent_symbols,
                                         ast_condition);

    sem_analyze_ast_code_block_to_ir(compile_status,
                                     ast_while_get_body(ast_while),
                                     body);    

    return IR_STATMENT(ir_while_new(condition, body));
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
                          NULL,
                          ast_node_get_line_num(var));
    }

    var = ast_foreach_get_value(ast_foreach);
    ir_value =
        ir_variable_new(ast_variable_declaration_get_data_type(var),
                        ast_variable_declaration_get_name(var),
                        NULL,
                        ast_node_get_line_num(var));

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
    AstExpression *value;
    IrExpression *ir_target;
    IrExpression *ir_value;

    target = ast_assigment_get_target(ast_assigment);
    value = ast_assigment_get_value(ast_assigment);

    ir_target =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         symbols,
                                         XDP_AST_EXPRESSION(target));
    assert(IR_IS_LVALUE(ir_target));

    ir_value =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         symbols,
                                         value);

    return IR_STATMENT(ir_assigment_new(IR_LVALUE(ir_target),
                                        ir_value,
                                        ast_node_get_line_num(ast_assigment)));
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
    guint line_number;

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
    line_number = ast_node_get_line_num(ast_operation);
    switch (op) {
        case ast_plus_op:
        case ast_minus_op:
        case ast_mult_op:
        case ast_division_op:
        case ast_equal_op:
        case ast_not_equal_op:
        case ast_less_op:
        case ast_greater_op:
        case ast_less_or_eq_op:
        case ast_greater_or_eq_op:
        case ast_and_op:
        case ast_or_op:
            return IR_EXPRESSION(ir_binary_operation_new(op,
                                                         left,
                                                         right,
                                                         line_number));
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
    ast_unary_op_type_t op_type;

    op_type =
        ast_unary_operation_get_operation(ast_operation);

    operand =
        sem_analyze_ast_expression_to_ir(compile_status,
                                         symbols,
                                         ast_unary_operation_get_operand(ast_operation));

    return IR_EXPRESSION(ir_unary_operation_new(op_type, operand));
}

static IrExpression *
sem_analyze_ast_func_call_to_ir(compilation_status_t *compile_status,
                                sym_table_t *symbols,
                                AstFunctionCall *func_call)
{
    char *func_name;
    GSList *i;
    GSList *ir_call_args = NULL;

    func_name = ast_function_call_get_name(func_call);

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
    ir_call_args = g_slist_reverse(ir_call_args);

    return IR_EXPRESSION(ir_function_call_new(func_name,
                                              ir_call_args,
                                              ast_node_get_line_num(func_call)));
}

static IrExpression *
sem_analyze_ast_array_literal_to_ir(compilation_status_t *compile_status,
                                    sym_table_t *symbols,
                                    AstArrayLiteral *ast_arry_literal)
{
    IrArrayLiteral *ir_arry_literal;
    GSList *i;

    ir_arry_literal =
        ir_array_literal_new(ast_node_get_line_num(ast_arry_literal));

    i = ast_array_literal_get_values(ast_arry_literal);
    for (; i != NULL; i = g_slist_next(i))
    {
        IrExpression *exp;

        exp =
          sem_analyze_ast_expression_to_ir(compile_status,
                                           symbols,
                                           i->data);
        ir_array_literal_add_value(ir_arry_literal, exp);
    }

    return IR_EXPRESSION(ir_arry_literal);
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
    else if (XDP_IS_AST_UINT_CONSTANT(ast_expression))
    {
        guint32 val;

        val = ast_uint_constant_get_value(XDP_AST_UINT_CONSTANT(ast_expression));
        return IR_EXPRESSION(ir_uint_constant_new(val));
    }
    else if (XDP_IS_AST_BOOL_CONSTANT(ast_expression))
    {
        gboolean val;

        val = 
            ast_bool_constant_get_value(XDP_AST_BOOL_CONSTANT(ast_expression));
        return IR_EXPRESSION(ir_bool_constant_new(val));
    }
    else if (XDP_IS_AST_ARRAY_SLICE_REF(ast_expression))
    {
        AstArraySliceRef *array_slice;
        char *name;
        AstExpression *ast_start_idx;
        IrExpression *ir_start_idx = NULL;
        AstExpression *ast_end_idx;
        IrExpression *ir_end_idx = NULL;
        guint line_numer = ast_node_get_line_num(ast_expression);

        array_slice = XDP_AST_ARRAY_SLICE_REF(ast_expression);
        name = ast_array_slice_ref_get_name(array_slice);

        /* convert start expression, if any, to IR-form */
        ast_start_idx = ast_array_slice_ref_get_start(array_slice);
        if (ast_start_idx != NULL)
        {
            ir_start_idx =
                sem_analyze_ast_expression_to_ir(compile_status,
                                                 symbols,
                                                 ast_start_idx);
        }

        /* convert end expression, if any, to IR-form */
        ast_end_idx = ast_array_slice_ref_get_end(array_slice);
        if (ast_end_idx != NULL)
        {
            ir_end_idx =
                sem_analyze_ast_expression_to_ir(compile_status,
                                                 symbols,
                                                 ast_end_idx);
        }

        return IR_EXPRESSION(ir_array_slice_new(name,
                                                ir_start_idx,
                                                ir_end_idx,
                                                line_numer));
    }
    else if (AST_IS_VARIABLE_REF(ast_expression))
    {
        AstVariableRef *var_ref;
        IrExpression *ir_expression;
        char *var_name;

        var_ref = AST_VARIABLE_REF(ast_expression);
        var_name = ast_variable_ref_get_name(var_ref);

        if (XDP_IS_AST_ARRAY_CELL_REF(var_ref))
        {
            AstArrayCellRef *array_cell_ref;
            AstExpression *ast_index_exp;
            IrExpression *ir_index_exp;

            /*
             * convert array index expression to IR form
             */
            array_cell_ref = XDP_AST_ARRAY_CELL_REF(var_ref);
            ast_index_exp = ast_array_cell_ref_get_index(array_cell_ref);
            ir_index_exp = 
                sem_analyze_ast_expression_to_ir(compile_status,
                                                 symbols,
                                                 ast_index_exp);
            /* create IR array cell ref */
            ir_expression =
                IR_EXPRESSION(ir_array_cell_new(var_name,
                                                ir_index_exp,
                                                ast_node_get_line_num(var_ref)));
                                              
        }
        else if (AST_IS_VARIABLE_REF(var_ref))
        {
            ir_expression =
                IR_EXPRESSION(ir_scalar_new(var_name,
                                            ast_node_get_line_num(var_ref)));
        }

        return ir_expression;
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
    else if (XDP_IS_AST_ARRAY_LITERAL(ast_expression))
    {
        AstArrayLiteral *arry_literal;

        arry_literal = XDP_AST_ARRAY_LITERAL(ast_expression);
        return sem_analyze_ast_array_literal_to_ir(compile_status,
                                                   symbols,
                                                   arry_literal);
    }

    /* unexpected expression type */
    printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(ast_expression)));
    g_assert_not_reached();
    return NULL;
}


static void
sem_analyze_ast_var_def_to_ir(compilation_status_t *compile_status,
                              AstVariableDefinition *var_def,
                              sym_table_t *sym_table)
{
    /*
     * put the variable definition into the local symbol table
     */

    DtDataType *var_data_type = ast_variable_definition_get_data_type(var_def);
    AstExpression *ast_initializer =
        ast_variable_definition_get_initializer(var_def);
    IrExpression *initializer = NULL;

    if (ast_initializer != NULL)
    {
        initializer =
            sem_analyze_ast_expression_to_ir(compile_status,
                                             sym_table,
                                             ast_initializer);
    }

    IrVariable *sym =
        ir_variable_new(var_data_type,
                        ast_variable_definition_get_name(var_def),
                        initializer,
                        ast_node_get_line_num(var_def));

    if (sym_table_add_symbol(sym_table, IR_SYMBOL(sym)) != 0)
    {
        compile_error(compile_status, 
                      IR_NODE(sym),
                      "redeclaration of symbol '%s'\n",
                      ir_symbol_get_name(IR_SYMBOL(sym)));
    }
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
        if (AST_IS_VARIABLE_DEFINITION(stmt))
        {
            AstVariableDefinition *var_def = AST_VARIABLE_DEFINITION(stmt);

            sem_analyze_ast_var_def_to_ir(compile_status,
                                          var_def,
                                          symbols);

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
        else if (AST_IS_WHILE(stmt))
        {
            ir_stmt =
                sem_analyze_ast_while_to_ir(compile_status,
                                            symbols,
                                            AST_WHILE(stmt));
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
        else if (XDP_IS_AST_EXPRESSION(stmt))
        {
            ir_stmt =
                IR_STATMENT(
                  sem_analyze_ast_expression_to_ir(compile_status,
                                                   symbols,
                                                   XDP_AST_EXPRESSION(stmt)));
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

static IrFunctionDecl *
sem_analyze_ast_func_decl_to_ir(AstFunctionDecl *ast_func_decl)
{
    assert(XDP_IS_AST_FUNCTION_DECL(ast_func_decl));

    IrFunctionDecl *func_decl;
    char *linkage_type_name;
    ir_linkage_type_t linkage_type;


    linkage_type_name = ast_function_decl_get_linkage(ast_func_decl);

    if (linkage_type_name == NULL || g_str_equal("D", linkage_type_name))
    {
        /* default linkage type is 'D' */
        linkage_type = ir_d_linkage;
    }
    else if (g_str_equal("C", linkage_type_name))
    {
        linkage_type = ir_c_linkage;
    } else {
        /* unexpected linkage type string */
        assert(false);
    }

    func_decl = 
        ir_function_decl_new(ast_function_decl_get_return_type(ast_func_decl),
                             ast_function_decl_get_name(ast_func_decl),
                             ast_function_decl_get_parameters(ast_func_decl),
                             linkage_type,
                             ast_node_get_line_num(ast_func_decl));

    return func_decl;
}

/**
 * convert AST representation of a function definition to IR form.
 */
static IrFunctionDef *
sem_analyze_ast_func_def_to_ir(compilation_status_t *compile_status,
                               AstFunctionDef *ast_func_def,
                               sym_table_t *global_sym_table)
{
    IrFunctionDef *ir_func;
    GSList *parameters = NULL;
    GSList *i;

    /* convert function parameters to IR form */
    i = ast_function_def_get_parameters(ast_func_def);
    for (; i != NULL; i = g_slist_next(i))
    {
        AstVariableDeclaration *ast_var;
        IrVariable *ir_var;

        ast_var = AST_VARIABLE_DECLARATION(i->data);

        ir_var = 
            ir_variable_new(ast_variable_declaration_get_data_type(ast_var),
                            ast_variable_declaration_get_name(ast_var),
                            NULL,
                            ast_node_get_line_num(ast_var));
        parameters = g_slist_prepend(parameters, ir_var);
    }
    parameters = g_slist_reverse(parameters);

    ir_func = 
        ir_function_def_new(ast_function_def_get_return_type(ast_func_def),
                            ast_function_def_get_name(ast_func_def),
                            parameters,
                            global_sym_table,
                            ast_node_get_line_num(ast_func_def));


    /* convert function body to ir format */
    sem_analyze_ast_code_block_to_ir(compile_status,
                                     ast_function_def_get_body(ast_func_def),
                                     ir_function_def_get_body(ir_func));

    return ir_func;
}

static IrCompileUnit *
sem_analyze_ast_compile_unit_to_ir(compilation_status_t *compile_status,
                                   AstCompileUnit *ast_compile_unit)
{
    IrCompileUnit *comp_unit;
    GSList *i;
    sym_table_t *global_sym_table;

    comp_unit = ir_compile_unit_new();

    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    /*
     * store all function declarations in module's symbol table
     */
    i = ast_compile_unit_get_function_decls(ast_compile_unit);
    for (;i != NULL; i = i->next)
    {
        IrFunctionDecl *ir_func_decl;

        ir_func_decl =
            sem_analyze_ast_func_decl_to_ir(XDP_AST_FUNCTION_DECL(i->data));
        if (!ir_compile_unit_add_function_decl(comp_unit, ir_func_decl))
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_decl),
                          "redeclaration of function '%s'\n",
                          ir_function_get_name(IR_FUNCTION(ir_func_decl)));
        }
    }
    
    /*
     * convert all function definitions to IR form and store them
     * in module's symbol table
     */
    i = ast_compile_unit_get_function_defs(ast_compile_unit);
    for (;i != NULL; i = i->next)
    {
        IrFunctionDef *ir_func_def;

        ir_func_def =
            sem_analyze_ast_func_def_to_ir(compile_status,
                                           XDP_AST_FUNCTION_DEF(i->data),
                                           global_sym_table);
        if (!ir_compile_unit_add_function_def(comp_unit, ir_func_def))
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_def),
                          "redifinition of function '%s'\n",
                          ir_function_get_name(IR_FUNCTION(ir_func_def)));
        }
    }

    return comp_unit;
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrCompileUnit *
semantic_analyze(const char *source_file, AstCompileUnit *ast_compile_unit)
{
    IrCompileUnit *comp_unit;

    compilation_status_t comp_stat;

    /* set-up compilation status struct */
    comp_stat.source_file = source_file;
    comp_stat.errors_count = 0;

    comp_unit =
        sem_analyze_ast_compile_unit_to_ir(&comp_stat, ast_compile_unit);

    /* if there were errors while converting to IR, return failure result */
    if (comp_stat.errors_count > 0)
    {
        /* @todo: clean-up comp_unit ? */
        return NULL;
    }

    sem_analyze_validate(&comp_stat, comp_unit);

    /* if there were errors during analysis, return failure result */
    if (comp_stat.errors_count > 0)
    {
        /* @todo: clean-up comp_unit ? */
        return NULL;
    }
    return comp_unit;
}


