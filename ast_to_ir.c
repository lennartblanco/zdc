#include "ast_to_ir.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_if_else.h"
#include "ast_while.h"
#include "ast_foreach.h"
#include "ast_return.h"
#include "ast_function_call.h"
#include "ast_assigment.h"
#include "ast_binary_operation.h"
#include "ast_array_literal.h"
#include "ast_array_cell_ref.h"
#include "ast_bool_constant.h"
#include "ast_int_constant.h"
#include "ast_uint_constant.h"
#include "ast_property.h"
#include "ir_array.h"
#include "ir_function_call.h"
#include "ir_if_else.h"
#include "ir_if_block.h"
#include "ir_variable.h"
#include "ir_while.h"
#include "ir_foreach.h"
#include "ir_return.h"
#include "ir_assigment.h"
#include "ir_array_slice.h"
#include "ir_array_literal.h"
#include "ir_array_cell.h"
#include "ir_scalar.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_bool_constant.h"
#include "ir_property.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static IrFunctionDecl *
func_decl_to_ir(AstFunctionDecl *ast_func_decl);

/**
 * convert AST representation of a function definition to IR form.
 *
 * @param convert convert_body if true, the function body will be converted
 *                             if false, the body will not be converted, only
 *                             function signature
 */
static IrFunctionDef *
func_def_to_ir(compilation_status_t *compile_status,
               AstFunctionDef *ast_func_def,
               IrModule *parent_module,
               bool convert_body);

/**
 * convert AST code block representation to IR form.
 */
static void
code_block_to_ir(compilation_status_t *compile_status,
                 AstCodeBlock *ast_code_block,
                 IrCodeBlock *ir_code_block);

static IrStatment *
while_to_ir(compilation_status_t *compile_status,
            sym_table_t *parent_symbols,
            AstWhile *ast_while);

static IrStatment *
foreach_to_ir(compilation_status_t *compile_status,
              sym_table_t *parent_symbols,
              AstForeach *ast_foreach);

static IrStatment *
return_to_ir(compilation_status_t *compile_status,
             sym_table_t *symbols,
             AstReturn *ast_return);

static IrStatment *
assigment_to_ir(compilation_status_t *compile_status,
                sym_table_t *symbols,
                AstAssigment *ast_assigment);

static IrExpression *
expression_to_ir(compilation_status_t *compile_status,
                 sym_table_t *symbols,
                 AstExpression *ast_expression);

static IrExpression *
binary_op_to_ir(compilation_status_t *compile_status,
                sym_table_t *symbols,
                AstBinaryOperation *ast_operation);

static IrExpression *
func_call_to_ir(compilation_status_t *compile_status,
                sym_table_t *symbols,
                AstFunctionCall *func_call);

static IrExpression *
array_literal_to_ir(compilation_status_t *compile_status,
                    sym_table_t *symbols,
                    AstArrayLiteral *ast_arry_literal);

static IrExpression *
array_slice_to_ir(compilation_status_t *compile_status,
                  sym_table_t *symbols,
                  AstArraySliceRef *ast_arry_slice);

static IrExpression *
array_cell_ref_to_ir(compilation_status_t *compile_status,
                     sym_table_t *symbols,
                     AstArrayCellRef *array_cell_ref);

static IrExpression *
property_to_ir(compilation_status_t *compile_status,
               sym_table_t *symbols,
               AstProperty *ast_property);

static void
var_def_to_ir(compilation_status_t *compile_status,
              AstVariableDefinition *var_def,
              sym_table_t *sym_table);

/**
 * convert AST if_else statment to IR form.
 */
static IrStatment * 
if_else_to_ir(compilation_status_t *compile_status,
              IrCodeBlock *parent_block,
              AstIfElse *ast_if_else);

/**
 * convert function parameters to IR form
 *
 * @param ast_func_params function parameters in ast-form to convert,
 *                        as a list of AstVariableDeclaration objects
 * @return function parameters in ir-form, as a list of IrVariable objects
 */
static GSList *
func_params_to_ir(GSList *ast_func_params);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/


static void
import_module(compilation_status_t *compile_status,
              sym_table_t *sym_table,
              AstModule *ast_module)
{
    sym_table_t *imports;
    GSList *i;
    IrModule *module;

    module = ir_module_new(ast_module_get_package(ast_module));
    imports = ir_module_get_symbols(module);

    /* add imported function declarations to sym table */
    i = ast_module_get_function_decls(ast_module);
    for (;i != NULL; i = i->next)
    {
        IrFunctionDecl *ir_func_decl;

        ir_func_decl = func_decl_to_ir(AST_FUNCTION_DECL(i->data));
        if (sym_table_add_symbol(imports, IR_SYMBOL(ir_func_decl)) != 0)
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_decl),
                          "redeclaration of function '%s'\n",
                          ir_function_get_name(IR_FUNCTION(ir_func_decl)));
        }
    }

    /* add imported function definitions to sym table */
    i = ast_module_get_function_defs(ast_module);
    for (;i != NULL; i = i->next)
    {
        IrFunctionDef *ir_func_def;

        ir_func_def = func_def_to_ir(compile_status,
                                     AST_FUNCTION_DEF(i->data),
                                     module,
                                     false);
        if (sym_table_add_symbol(imports, IR_SYMBOL(ir_func_def)) != 0)
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_def),
                          "redifinition of function '%s'\n",
                          ir_function_get_name(IR_FUNCTION(ir_func_def)));
        }
    }

    sym_table_add_import(sym_table, imports);
}

IrModule *
sem_analyze_ast_module_to_ir(compilation_status_t *compile_status,
                             AstModule *ast_module)
{
    IrModule *module;
    GSList *i;
    sym_table_t *module_sym_table;

    module = ir_module_new(ast_module_get_package(ast_module));

    /*
     * store all function declarations in module's symbol table
     */
    i = ast_module_get_function_decls(ast_module);
    for (;i != NULL; i = i->next)
    {
        IrFunctionDecl *ir_func_decl;

        ir_func_decl = func_decl_to_ir(AST_FUNCTION_DECL(i->data));
        if (!ir_module_add_function_decl(module, ir_func_decl))
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
    i = ast_module_get_function_defs(ast_module);
    for (;i != NULL; i = i->next)
    {
        IrFunctionDef *ir_func_def;

        ir_func_def = func_def_to_ir(compile_status,
                                     AST_FUNCTION_DEF(i->data),
                                     module,
                                     true);
        if (!ir_module_add_function_def(module, ir_func_def))
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_def),
                          "redifinition of function '%s'\n",
                          ir_function_get_name(IR_FUNCTION(ir_func_def)));
        }
    }

    /*
     * Handle imports
     */
    i = ast_module_get_imports(ast_module);
    module_sym_table = ir_module_get_symbols(module);

    for (; i != NULL; i = g_slist_next(i))
    {
        import_module(compile_status,
                      module_sym_table,
                      ast_import_get_module(i->data));
    }

    return module;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static GSList *
func_params_to_ir(GSList *ast_func_params)
{
    GSList *parameters = NULL;
    GSList *i;

    for (i = ast_func_params; i != NULL; i = g_slist_next(i))
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

    return g_slist_reverse(parameters);
}

static IrFunctionDecl *
func_decl_to_ir(AstFunctionDecl *ast_func_decl)
{
    assert(AST_IS_FUNCTION_DECL(ast_func_decl));

    IrFunctionDecl *func_decl;
    char *linkage_type_name;
    ir_linkage_type_t linkage_type;
    GSList *parameters;


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

    parameters = 
        func_params_to_ir(ast_function_decl_get_parameters(ast_func_decl));

    func_decl = 
        ir_function_decl_new(ast_function_decl_get_return_type(ast_func_decl),
                             ast_function_decl_get_name(ast_func_decl),
                             parameters,
                             linkage_type,
                             ast_node_get_line_num(ast_func_decl));

    return func_decl;
}

static IrFunctionDef *
func_def_to_ir(compilation_status_t *compile_status,
               AstFunctionDef *ast_func_def,
               IrModule *parent_module,
               bool convert_body)
{
    IrFunctionDef *ir_func;
    GSList *parameters;
    parameters = 
        func_params_to_ir(ast_function_def_get_parameters(ast_func_def));

    ir_func = 
        ir_function_def_new(ast_function_def_get_return_type(ast_func_def),
                            ast_function_def_get_name(ast_func_def),
                            parameters,
                            parent_module,
                            ast_node_get_line_num(ast_func_def));

    if (convert_body)
    {
        /* convert function body to ir format */
        code_block_to_ir(compile_status,
                         ast_function_def_get_body(ast_func_def),
                         ir_function_def_get_body(ir_func));
    }

    return ir_func;
}

static void
code_block_to_ir(compilation_status_t *compile_status,
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
        AstStatment *stmt = AST_STATMENT(i->data);

        /* variable declaration found */
        if (AST_IS_VARIABLE_DEFINITION(stmt))
        {
            AstVariableDefinition *var_def = AST_VARIABLE_DEFINITION(stmt);

            var_def_to_ir(compile_status, var_def, symbols);

            /** @todo delete the stmt node here */
            /* no IR statment to add, jump to next ast statment */
            continue;
        }
        /* sub-code block found */
        else if (AST_IS_CODE_BLOCK(stmt))
        {
            IrCodeBlock *sub_block = ir_code_block_new(symbols);

            code_block_to_ir(compile_status,
                             AST_CODE_BLOCK(stmt),
                             sub_block);
            ir_stmt = IR_STATMENT(sub_block);
        }
        else if (AST_IS_IF_ELSE(stmt))
        {
            ir_stmt =
               if_else_to_ir(compile_status, ir_code_block, AST_IF_ELSE(stmt));
        }
        else if (AST_IS_WHILE(stmt))
        {
            ir_stmt = while_to_ir(compile_status,
                                  symbols,
                                  AST_WHILE(stmt));
        }
        else if (AST_IS_FOREACH(stmt))
        {
            ir_stmt = foreach_to_ir(compile_status,
                                    symbols,
                                    AST_FOREACH(stmt));
        }
        else if (AST_IS_RETURN(stmt))
        {
            ir_stmt = return_to_ir(compile_status,
                                   symbols,
                                   AST_RETURN(stmt));
        }
        else if (AST_IS_FUNCTION_CALL(stmt))
        {
            ir_stmt =
                IR_STATMENT(func_call_to_ir(compile_status,
                                            symbols,
                                            AST_FUNCTION_CALL(stmt)));
        }
        else if (AST_IS_ASSIGMENT(stmt))
        {
            ir_stmt = assigment_to_ir(compile_status,
                                      symbols,
                                      AST_ASSIGMENT(stmt));
        }
        else if (AST_IS_EXPRESSION(stmt))
        {
            ir_stmt =
                IR_STATMENT(expression_to_ir(compile_status,
                                             symbols,
                                             AST_EXPRESSION(stmt)));
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
        expression_to_ir(compile_status, sym_table, ast_condition);

    code_block_to_ir(compile_status,
                     ast_if_block_get_body(ast_if_block),
                     body);

    return ir_if_block_new(condition, body);
}

static IrStatment * 
if_else_to_ir(compilation_status_t *compile_status,
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
                                              AST_IF_BLOCK(i->data));
        ir_if_else_add_if_else_block(ifelse, ifblock);
    }

    /* convert else-body code block to ir form, if else block is present */
    AstCodeBlock *ast_else_body = ast_if_else_get_else_block(ast_if_else);
    if (ast_else_body != NULL)
    {
        IrCodeBlock *else_body =
            ir_code_block_new(ir_code_block_get_symbols(parent_block));

        code_block_to_ir(compile_status, ast_else_body, else_body);
        ir_if_else_set_else_body(ifelse, else_body);
    }

    return IR_STATMENT(ifelse);
}

/**
 * Convert AST while statment to IR form.
 */
static IrStatment *
while_to_ir(compilation_status_t *compile_status,
            sym_table_t *parent_symbols,
            AstWhile *ast_while)
{
    AstExpression *ast_condition = ast_while_get_loop_condition(ast_while);
    IrCodeBlock *body = ir_code_block_new(parent_symbols);

    IrExpression *condition =
        expression_to_ir(compile_status, parent_symbols, ast_condition);

    code_block_to_ir(compile_status,  ast_while_get_body(ast_while), body);    

    return IR_STATMENT(ir_while_new(condition, body));
}

static IrStatment *
foreach_to_ir(compilation_status_t *compile_status,
              sym_table_t *parent_symbols,
              AstForeach *ast_foreach)
{
    IrForeach *foreach;
    IrVariable *ir_index = NULL;
    IrVariable *ir_value;
    IrArraySlice *ir_aggregate;
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
    code_block_to_ir(compile_status, ast_foreach_get_body(ast_foreach), body);

    ir_aggregate =
        IR_ARRAY_SLICE(
            array_slice_to_ir(compile_status,
                              parent_symbols,
                              ast_foreach_get_aggregate(ast_foreach)));

    foreach = ir_foreach_new(ir_index, ir_value, ir_aggregate, body);

    return IR_STATMENT(foreach);
}

/**
 * Convert AST return statment to IR form.
 */
static IrStatment *
return_to_ir(compilation_status_t *compile_status,
             sym_table_t *symbols,
             AstReturn *ast_return)
{
  IrExpression *ir_return_value = NULL;
  AstExpression *ast_return_value;

  ast_return_value = ast_return_get_return_value(ast_return);
  if (ast_return_value != NULL)
  {
      ir_return_value =
          expression_to_ir(compile_status, symbols, ast_return_value); 
  }

  return IR_STATMENT(ir_return_new(ir_return_value));
}

static IrStatment *
assigment_to_ir(compilation_status_t *compile_status,
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
        expression_to_ir(compile_status, symbols, AST_EXPRESSION(target));
    assert(IR_IS_LVALUE(ir_target));

    ir_value =
        expression_to_ir(compile_status, symbols, value);

    return IR_STATMENT(ir_assigment_new(IR_LVALUE(ir_target),
                                        ir_value,
                                        ast_node_get_line_num(ast_assigment)));
}

/**
 * Convert AST binary operation to IR form.
 */
static IrExpression *
binary_op_to_ir(compilation_status_t *compile_status,
                sym_table_t *symbols,
                AstBinaryOperation *ast_operation)
{
    IrExpression *left;
    IrExpression *right;
    ast_binary_op_type_t op;
    guint line_number;

    left = expression_to_ir(compile_status,
                            symbols,
                            ast_binary_operation_get_left(ast_operation));
    right = expression_to_ir(compile_status,
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
ast_unary_op_to_ir(compilation_status_t *compile_status,
                   sym_table_t *symbols,
                   AstUnaryOperation *ast_operation)
{
    IrExpression *operand;
    ast_unary_op_type_t op_type;

    op_type =
        ast_unary_operation_get_operation(ast_operation);

    operand =
        expression_to_ir(compile_status,
                         symbols,
                         ast_unary_operation_get_operand(ast_operation));

    return IR_EXPRESSION(ir_unary_operation_new(op_type, operand));
}

static IrExpression *
func_call_to_ir(compilation_status_t *compile_status,
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
            expression_to_ir(compile_status, symbols, AST_EXPRESSION(i->data));
        ir_call_args = g_slist_prepend(ir_call_args, arg);
    }
    ir_call_args = g_slist_reverse(ir_call_args);

    return IR_EXPRESSION(ir_function_call_new(func_name,
                                              ir_call_args,
                                              ast_node_get_line_num(func_call)));
}

static IrExpression *
array_literal_to_ir(compilation_status_t *compile_status,
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

        exp = expression_to_ir(compile_status, symbols, i->data);
        ir_array_literal_add_value(ir_arry_literal, exp);
    }

    return IR_EXPRESSION(ir_arry_literal);
}

static IrExpression *
array_slice_to_ir(compilation_status_t *compile_status,
                  sym_table_t *symbols,
                  AstArraySliceRef *ast_arry_slice)
{
    char *name;
    AstExpression *ast_start_idx;
    IrExpression *ir_start_idx = NULL;
    AstExpression *ast_end_idx;
    IrExpression *ir_end_idx = NULL;
    guint line_number;

    name = ast_array_slice_ref_get_name(ast_arry_slice);


    /* convert start expression, if any, to IR-form */
    ast_start_idx = ast_array_slice_ref_get_start(ast_arry_slice);
    if (ast_start_idx != NULL)
    {
        ir_start_idx =
            expression_to_ir(compile_status, symbols, ast_start_idx);
    }

    /* convert end expression, if any, to IR-form */
    ast_end_idx = ast_array_slice_ref_get_end(ast_arry_slice);
    if (ast_end_idx != NULL)
    {
        ir_end_idx = expression_to_ir(compile_status, symbols, ast_end_idx);
    }

    line_number = ast_node_get_line_num(ast_arry_slice);
    return IR_EXPRESSION(ir_array_slice_new(name,
                                            ir_start_idx,
                                            ir_end_idx,
                                            line_number));
}

static IrExpression *
array_cell_ref_to_ir(compilation_status_t *compile_status,
                     sym_table_t *symbols,
                     AstArrayCellRef *array_cell_ref)
{
    assert(compile_status);
    assert(symbols);
    assert(AST_IS_ARRAY_CELL_REF(array_cell_ref));

    AstExpression *ast_index_exp;
    IrExpression *ir_index_exp;

    /*
     * convert array index expression to IR form
     */
    ast_index_exp = ast_array_cell_ref_get_index(array_cell_ref);
    ir_index_exp = expression_to_ir(compile_status, symbols, ast_index_exp);
            /* create IR array cell ref */
    return
        IR_EXPRESSION(
            ir_array_cell_new(ast_array_cell_ref_get_name(array_cell_ref),
                              ir_index_exp,
                              ast_node_get_line_num(array_cell_ref)));
}

static IrExpression *
variable_ref_to_ir(compilation_status_t *compile_status,
                   sym_table_t *symbols,
                   AstVariableRef *var_ref)
{
    char *var_name;
    IrSymbol *var_symb;

    var_name = ast_variable_ref_get_name(var_ref);
    var_symb = sym_table_get_symbol(symbols, var_name, NULL);

    if (IR_IS_VARIABLE(var_symb) &&
        DT_IS_ARRAY_TYPE(ir_variable_get_data_type(IR_VARIABLE(var_symb))))
    {
        /* this is an array handle expression */
        return IR_EXPRESSION(ir_array_new(IR_VARIABLE(var_symb)));
    }

    return
        IR_EXPRESSION(ir_scalar_new(var_name, ast_node_get_line_num(var_ref)));
}

static IrExpression *
property_to_ir(compilation_status_t *compile_status,
               sym_table_t *symbols,
               AstProperty *ast_property)
{
    assert(compile_status);
    assert(symbols);
    assert(AST_IS_PROPERTY(ast_property));

    IrExpression *exp;
    IrProperty *prop;

    exp = expression_to_ir(compile_status,
                           symbols,
                           ast_property_get_expression(ast_property));

    prop = ir_property_new(exp,
                           ast_property_get_name(ast_property),
                           ast_node_get_line_num(ast_property));

    if (prop == NULL)
    {
        compile_error(compile_status,
                      IR_NODE(exp),
                      "unknown property '%s'\n",
                      ast_property_get_name(ast_property));
       return NULL;
    }

    return IR_EXPRESSION(prop);
}

/**
 * Convert AST expression to IR form.
 */
static IrExpression *
expression_to_ir(compilation_status_t *compile_status,
                 sym_table_t *symbols,
                 AstExpression *ast_expression)
{
    assert(compile_status);
    assert(symbols);
    assert(ast_expression);

    if (AST_IS_INT_CONSTANT(ast_expression))
    {
        gint32 val;

        val = ast_int_constant_get_value(AST_INT_CONSTANT(ast_expression));
        return
            IR_EXPRESSION(
                ir_int_constant_new(
                    val, ast_node_get_line_num(AST_NODE(ast_expression))));
    }
    else if (AST_IS_UINT_CONSTANT(ast_expression))
    {
        guint32 val;

        val = ast_uint_constant_get_value(AST_UINT_CONSTANT(ast_expression));
        return
            IR_EXPRESSION(
                ir_uint_constant_new(
                    val, ast_node_get_line_num(AST_NODE(ast_expression))));
    }
    else if (AST_IS_BOOL_CONSTANT(ast_expression))
    {
        gboolean val;

        val = 
            ast_bool_constant_get_value(AST_BOOL_CONSTANT(ast_expression));
        return IR_EXPRESSION(ir_bool_constant_new(val));
    }
    else if (AST_IS_ARRAY_LITERAL(ast_expression))
    {
        AstArrayLiteral *arry_literal;

        arry_literal = AST_ARRAY_LITERAL(ast_expression);
        return array_literal_to_ir(compile_status, symbols, arry_literal);
    }
    else if (AST_IS_ARRAY_SLICE_REF(ast_expression))
    {
        AstArraySliceRef *array_slice;

        array_slice = AST_ARRAY_SLICE_REF(ast_expression);
        return array_slice_to_ir(compile_status, symbols, array_slice);
    }
    else if (AST_IS_ARRAY_CELL_REF(ast_expression))
    {
        AstArrayCellRef *array_cell_ref;

        array_cell_ref = AST_ARRAY_CELL_REF(ast_expression);
        return array_cell_ref_to_ir(compile_status, symbols, array_cell_ref);
    }
    else if (AST_IS_VARIABLE_REF(ast_expression))
    {
        AstVariableRef *var_ref;

        var_ref = AST_VARIABLE_REF(ast_expression);
        return variable_ref_to_ir(compile_status, symbols, var_ref);
    }
    else if (AST_IS_UNARY_OPERATION(ast_expression))
    {
        AstUnaryOperation *op;

        op = AST_UNARY_OPERATION(ast_expression);

        return ast_unary_op_to_ir(compile_status, symbols, op);
    }
    else if (AST_IS_BINARY_OPERATION(ast_expression))
    {
        AstBinaryOperation *bin_op;

        bin_op = AST_BINARY_OPERATION(ast_expression);

        return binary_op_to_ir(compile_status, symbols, bin_op);
    }
    else if (AST_IS_FUNCTION_CALL(ast_expression))
    {
        AstFunctionCall *func_call;

        func_call = AST_FUNCTION_CALL(ast_expression);
        return func_call_to_ir(compile_status, symbols, func_call);
    }
    else if (AST_IS_PROPERTY(ast_expression))
    {
        return property_to_ir(compile_status,
                              symbols,
                              AST_PROPERTY(ast_expression));
    }

    /* unexpected expression type */
    printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(ast_expression)));
    g_assert_not_reached();
    return NULL;
}


static void
var_def_to_ir(compilation_status_t *compile_status,
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
            expression_to_ir(compile_status, sym_table, ast_initializer);
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
