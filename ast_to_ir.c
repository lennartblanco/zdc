#include "ast_to_ir.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_if_else.h"
#include "ast_while.h"
#include "ast_for.h"
#include "ast_foreach.h"
#include "ast_foreach_range.h"
#include "ast_break.h"
#include "ast_return.h"
#include "ast_function_call.h"
#include "ast_assignment.h"
#include "ast_conditional.h"
#include "ast_binary_operation.h"
#include "ast_array_literal.h"
#include "ast_string_literal.h"
#include "ast_array_cell_ref.h"
#include "ast_bool_constant.h"
#include "ast_char_constant.h"
#include "ast_int_constant.h"
#include "ast_uint_constant.h"
#include "ast_postfix_exp.h"
#include "ast_ptr_dref.h"
#include "ast_address_of.h"
#include "ast_cast.h"
#include "ast_alias.h"
#include "ast_enum.h"
#include "ast_enum_member.h"
#include "ast_struct.h"
#include "ir_function_call.h"
#include "ir_if_else.h"
#include "ir_if_block.h"
#include "ir_variable.h"
#include "ir_while.h"
#include "ir_for.h"
#include "ir_foreach.h"
#include "ir_foreach_range.h"
#include "ir_break.h"
#include "ir_return.h"
#include "ir_assignment.h"
#include "ir_array_slice.h"
#include "ir_array_literal.h"
#include "ir_array_cell.h"
#include "ir_conditional.h"
#include "ir_cast.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_basic_constant.h"
#include "ir_property.h"
#include "ir_ptr_dref.h"
#include "ir_address_of.h"
#include "ir_enum_member.h"
#include "ir_struct.h"
#include "ir_dot.h"
#include "ir_ident.h"
#include "ir_var_value.h"
#include "ir_var_ref.h"
#include "dt_basic.h"
#include "errors.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static DtEnum *
enum_to_dt(compilation_status_t *compile_status,
           sym_table_t *symbols,
           AstEnum *ast_enum);

static IrStruct *
struct_to_ir(compilation_status_t *compile_status,
             AstStruct *ast_struct);

static IrFunctionDecl *
func_decl_to_ir(AstFunctionDecl *ast_func_decl, IrModule *parent_module);

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
for_to_ir(compilation_status_t *compile_status,
          sym_table_t *parent_symbols,
          AstFor *ast_for);

static IrStatment *
foreach_to_ir(compilation_status_t *compile_status,
              sym_table_t *parent_symbols,
              AstForeach *ast_foreach);

static IrStatment *
foreach_range_to_ir(compilation_status_t *compile_status,
                    sym_table_t *parent_symbols,
                    AstForeachRange *ast_foreach_range);

static IrStatment *
return_to_ir(compilation_status_t *compile_status,
             sym_table_t *symbols,
             AstReturn *ast_return);

static IrStatment *
assignment_to_ir(compilation_status_t *compile_status,
                sym_table_t *symbols,
                AstAssignment *ast_assignment);

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
string_literal_to_ir(compilation_status_t *compile_status,
                     sym_table_t *symbols,
                     AstStringLiteral *ast_str_literal);

static IrExpression *
array_slice_to_ir(compilation_status_t *compile_status,
                  sym_table_t *symbols,
                  AstArraySliceRef *ast_arry_slice);

static IrExpression *
array_cell_ref_to_ir(compilation_status_t *compile_status,
                     sym_table_t *symbols,
                     AstArrayCellRef *array_cell_ref);

static IrExpression *
postfix_exp_to_ir(compilation_status_t *compile_status,
                  sym_table_t *symbols,
                  AstPostfixExp *ast_postfix);

static IrVariable *
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

static void
import_module(compilation_status_t *compile_status,
              IrModule *parent_module,
              AstModule *ast_module);

static void
process_user_types(compilation_status_t *compile_status,
                   IrModule *module,
                   GSList *user_types);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

IrModule *
ast_module_to_ir(compilation_status_t *compile_status, AstModule *ast_module)
{
    IrModule *module;
    GSList *i;
    sym_table_t *module_sym_table;

    module = ir_module_new(ast_module_get_package(ast_module));
    compile_status->module = module;
    module_sym_table = ir_module_get_symbols(module);

    /*
     * Handle imports
     */
    i = ast_module_get_imports(ast_module);
    for (; i != NULL; i = g_slist_next(i))
    {
        import_module(compile_status, module, ast_import_get_module(i->data));
    }

    /*
     * store all function declarations in module's symbol table
     */
    i = ast_module_get_function_decls(ast_module);
    for (;i != NULL; i = i->next)
    {
        IrFunctionDecl *ir_func_decl;

        ir_func_decl =
            func_decl_to_ir(AST_FUNCTION_DECL(i->data), module);
        if (!ir_module_add_function_decl(module, ir_func_decl))
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_decl),
                          "redeclaration of function '%s'\n",
                          ir_function_get_name(ir_function(ir_func_decl)));
        }
    }

    /* process all user defined types */
    process_user_types(compile_status,
                       module,
                       ast_module_get_user_types(ast_module));

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
                          ir_function_get_name(ir_function(ir_func_def)));
        }
    }

    return module;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
process_user_types(compilation_status_t *compile_status,
                   IrModule *module,
                   GSList *user_types)
{
    GSList *i;

    IrModule *old_module = compile_status->module;
    compile_status->module = module;

    for (i = user_types; i != NULL; i = i->next)
    {
        AstUserType *user_type = AST_USER_TYPE(i->data);

        if (AST_IS_ENUM(user_type))
        {
            DtEnum *dt_enum;

            dt_enum = enum_to_dt(compile_status,
                                 ir_module_get_symbols(module),
                                 AST_ENUM(i->data));
            if (!ir_module_add_enum(module, dt_enum))
            {
                compile_error(compile_status,
                              dt_enum,
                              "enum declaration conflicts with"
                              " other user type '%s' definition\n",
                              ast_enum_get_tag(AST_ENUM(i->data)));
            }

        }
        else if (AST_IS_STRUCT(user_type))
        {
            IrStruct *ir_struct;

            ir_struct = struct_to_ir(compile_status,
                                     AST_STRUCT(i->data));
            if (!ir_module_add_struct(module, ir_struct))
            {
                compile_error(compile_status,
                              i->data,
                              "struct declaration conflicts with"
                              " other user type '%s' definition\n",
                              ast_struct_get_name(AST_STRUCT(i->data)));
            }
        }
        else if (AST_IS_ALIAS(user_type))
        {
            DtAlias *alias =
                dt_alias_new(ast_alias_get_data_type(i->data),
                             ast_alias_get_name(i->data));

            if (!ir_module_add_type_alias(module, alias)) {
                        compile_error(compile_status,
                                      i->data,
                                      "alias conflicts with"
                                      " other user type '%s' definition\n",
                                      dt_alias_get_name(alias));
            }
        }
        else
        {
            /* unexpected user type */
            assert(false);
        }
    }

    compile_status->module = old_module;
}

static void
import_module(compilation_status_t *compile_status,
              IrModule *parent_module,
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

        ir_func_decl = func_decl_to_ir(AST_FUNCTION_DECL(i->data), module);
        if (!ir_module_add_function_decl(module, ir_func_decl))
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_decl),
                          "redeclaration of function '%s'\n",
                          ir_function_get_name(ir_function(ir_func_decl)));
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
        if (!ir_module_add_function_def(module, ir_func_def))
        {
            compile_error(compile_status,
                          IR_NODE(ir_func_def),
                          "redifinition of function '%s'\n",
                          ir_function_get_name(ir_function(ir_func_def)));
        }
    }

    /* process all user defined types */
    process_user_types(compile_status,
                       module,
                       ast_module_get_user_types(ast_module));

    ir_module_add_import(parent_module, module);

    /* handle public imports */
    i = ast_module_get_imports(ast_module);
    for (; i != NULL; i = g_slist_next(i))
    {
        AstImport *imp = AST_IMPORT(i->data);

        if (ast_import_is_private(imp))
        {
            /* skip private imports */
            continue;
        }

        import_module(compile_status,
                      parent_module,
                      ast_import_get_module(imp));
    }

}

static IrEnumMember *
enum_member_to_ir(compilation_status_t *compile_status,
                  sym_table_t *symbols,
                  DtEnum *enum_def,
                  AstEnumMember *ast_member)
{
    IrExpression *member_val = NULL;
    AstExpression *member_init;

    member_init = ast_enum_member_get_initializer(ast_member);
    if (member_init != NULL)
    {
        member_val = expression_to_ir(compile_status,
                                      symbols,
                                      member_init);
    }

    return ir_enum_member_new(enum_def,
                              ast_enum_member_get_name(ast_member),
                              member_val);
}

static DtEnum *
enum_to_dt(compilation_status_t *compile_status,
           sym_table_t *symbols,
           AstEnum *ast_enum)
{
    DtEnum *enum_def;
    GSList *i;
    GSList *ir_enum_members = NULL;

    enum_def = dt_enum_new(ast_enum_get_tag(ast_enum),
                           ast_enum_get_base_type(ast_enum),
                           compile_status->module,
                           ast_node_get_line_num(ast_enum));

    for (i = ast_enum_get_members(ast_enum); i != NULL; i = g_slist_next(i))
    {
        ir_enum_members = g_slist_prepend(ir_enum_members,
                                          enum_member_to_ir(compile_status,
                                                            symbols,
                                                            enum_def,
                                                            i->data));
    }
    dt_enum_set_members(enum_def, g_slist_reverse(ir_enum_members));


    return enum_def;
}

static IrStruct *
struct_to_ir(compilation_status_t *compile_status,
             AstStruct *ast_struct)
{
    GSList *i;
    GSList *members = NULL;
    sym_table_t *symbols;

    symbols = sym_table_new(NULL);
    for (i = ast_struct_get_members(ast_struct);
         i != NULL;
         i = g_slist_next(i))
    {
        assert(AST_IS_VARIABLE_DEFINITION(i->data));

        IrVariable *var =
                var_def_to_ir(compile_status,
                              AST_VARIABLE_DEFINITION(i->data),
                              symbols);
        if (sym_table_add_symbol(symbols, ir_symbol(var)) != 0)
        {
            compile_error(compile_status,
                          var,
                          "redeclaration of symbol '%s'\n",
                          ir_symbol_get_name(ir_symbol(var)));
        }

        members = g_slist_prepend(members, var);
    }

    return ir_struct_new(ast_struct_get_name(ast_struct),
                         g_slist_reverse(members),
                         compile_status->module,
                         symbols);
}

static IrVariable *
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
        ir_variable_new(false,
                        var_data_type,
                        ast_variable_definition_get_name(var_def),
                        initializer,
                        ast_node_get_line_num(var_def));

    return sym;
}

static IrVariable *
var_decl_to_ir(AstVariableDeclaration *var_decl)
{
    assert(AST_IS_VARIABLE_DECLARATION(var_decl));

    return
        ir_variable_new(ast_variable_declaration_is_ref(var_decl),
                        ast_variable_declaration_get_data_type(var_decl),
                        ast_variable_declaration_get_name(var_decl),
                        NULL,
                        ast_node_get_line_num(var_decl));
}

static GSList *
func_params_to_ir(GSList *ast_func_params)
{
    GSList *parameters = NULL;
    GSList *i;

    for (i = ast_func_params; i != NULL; i = g_slist_next(i))
    {
        parameters =
            g_slist_prepend(parameters,
                            var_decl_to_ir(AST_VARIABLE_DECLARATION(i->data)));
    }

    return g_slist_reverse(parameters);
}

static ir_linkage_type_t
parse_linkage_type(const char *linkage_name)
{
    ir_linkage_type_t linkage_type;

    if (linkage_name == NULL || /* default linkage type is 'D' */
        g_str_equal("D", linkage_name))
    {
        linkage_type = ir_d_linkage;
    }
    else if (g_str_equal("C", linkage_name))
    {
        linkage_type = ir_c_linkage;
    } else {
        /* unexpected linkage type string */
        assert(false);
    }

    return linkage_type;
}

static IrFunctionDecl *
func_decl_to_ir(AstFunctionDecl *ast_func_decl, IrModule *parent_module)
{
    assert(AST_IS_FUNCTION_DECL(ast_func_decl));

    IrFunctionDecl *func_decl;
    ir_linkage_type_t linkage_type;
    GSList *parameters;

    linkage_type =
            parse_linkage_type(ast_function_decl_get_linkage(ast_func_decl));

    parameters = 
        func_params_to_ir(ast_function_decl_get_parameters(ast_func_decl));

    func_decl = 
        ir_function_decl_new(ast_function_decl_get_return_type(ast_func_decl),
                             ast_function_decl_get_name(ast_func_decl),
                             parameters,
                             parent_module,
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
    ir_linkage_type_t linkage_type;
    GSList *parameters;

    linkage_type =
            parse_linkage_type(ast_function_def_get_linkage(ast_func_def));

    parameters = 
        func_params_to_ir(ast_function_def_get_parameters(ast_func_def));

    ir_func = 
        ir_function_def_new(ast_function_def_get_return_type(ast_func_def),
                            ast_function_def_get_name(ast_func_def),
                            parameters,
                            parent_module,
                            linkage_type,
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
            IrVariable *var =
                    var_def_to_ir(compile_status,
                                  AST_VARIABLE_DEFINITION(stmt),
                                  symbols);
            if (ir_code_block_add_local_var(ir_code_block, var) != 0)
            {
                compile_error(compile_status,
                              stmt,
                              "redeclaration of symbol '%s'\n",
                              ir_symbol_get_name(ir_symbol(var)));
            }

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
        else if (AST_IS_FOR(stmt))
        {
            ir_stmt = for_to_ir(compile_status,
                                symbols,
                                AST_FOR(stmt));
        }
        else if (AST_IS_FOREACH(stmt))
        {
            ir_stmt = foreach_to_ir(compile_status,
                                    symbols,
                                    AST_FOREACH(stmt));
        }
        else if (AST_IS_FOREACH_RANGE(stmt))
        {
            ir_stmt = foreach_range_to_ir(compile_status,
                                          symbols,
                                          AST_FOREACH_RANGE(stmt));
        }
        else if (AST_IS_BREAK(stmt))
        {
            ir_stmt = IR_STATMENT(ir_break_new(ast_node_get_line_num(stmt)));
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
        else if (AST_IS_ASSIGNMENT(stmt))
        {
            ir_stmt = assignment_to_ir(compile_status,
                                       symbols,
                                       AST_ASSIGNMENT(stmt));
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
            /* unexpected statement type */
            assert(false);
        }

        if (ir_stmt != NULL)
        {
            /*
             * if statement was successfully converted,
             * add it to the code block
             */
            ir_code_block_add_statment(ir_code_block, ir_stmt);
        }
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
    if (condition == NULL)
    {
        /* invalid condition expression, bail out */
        return NULL;
    }

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
        if (ifblock == NULL)
        {
            /* invalid if block, bail out */
            return NULL;
        }
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

    code_block_to_ir(compile_status, ast_while_get_body(ast_while), body);

    return IR_STATMENT(ir_while_new(condition, body));
}

static IrStatment *
for_to_ir(compilation_status_t *compile_status,
          sym_table_t *parent_symbols,
          AstFor *ast_for)
{
    assert(AST_IS_FOR(ast_for));
    AstExpression *ast_exp;

    /* convert loop init code block to ir */
    IrCodeBlock *init = ir_code_block_new(parent_symbols);
    code_block_to_ir(compile_status, ast_for_get_init(ast_for), init);
    sym_table_t *loop_symbols = ir_code_block_get_symbols(init);

    /* convert loop test expression to ir */
    IrExpression *test = NULL;
    if ((ast_exp = ast_for_get_test(ast_for)) != NULL)
    {
        test = expression_to_ir(compile_status, loop_symbols, ast_exp);
    }

    /* convert loop step expression to ir */
    IrExpression *step = NULL;
    if ((ast_exp = ast_for_get_step(ast_for)) != NULL)
    {
        step = expression_to_ir(compile_status, loop_symbols, ast_exp);
    }

    /* convert loop body to ir */
    IrCodeBlock *body = ir_code_block_new(loop_symbols);
    code_block_to_ir(compile_status, ast_for_get_body(ast_for), body);

    return IR_STATMENT(ir_for_new(init, test, step, body));
}

static IrStatment *
foreach_to_ir(compilation_status_t *compile_status,
              sym_table_t *parent_symbols,
              AstForeach *ast_foreach)
{
    IrForeach *foreach;
    IrVariable *ir_index = NULL;
    IrVariable *ir_value;
    IrExpression *ir_aggregate;
    AstVariableDeclaration *var;
    sym_table_t *loop_symbols;
    IrCodeBlock *body;

    /* if index variable specified, convert it to ir */
    var = ast_foreach_get_index(ast_foreach);
    if (var != NULL)
    {
        ir_index = var_decl_to_ir(var);
    }

    /* convert value variable to ir */
    ir_value = var_decl_to_ir(ast_foreach_get_value(ast_foreach));

    /* store index and value variables in foreach's local symbols table */
    loop_symbols = sym_table_new(parent_symbols);

    if (ir_index != NULL)
    {
        sym_table_add_symbol(loop_symbols, ir_symbol(ir_index));
    }

    sym_table_add_symbol(loop_symbols, ir_symbol(ir_value));

    /* convert foreach body to ir */
    body = ir_code_block_new(loop_symbols);
    code_block_to_ir(compile_status, ast_foreach_get_body(ast_foreach), body);

    /* convert aggregate expression to ir form */
    ir_aggregate = expression_to_ir(compile_status,
                                    parent_symbols,
                                    ast_foreach_get_aggregate(ast_foreach));

    foreach = ir_foreach_new(ir_index,
                             ir_value,
                             ir_aggregate,
                             body,
                             ast_node_get_line_num(ast_foreach));

    return IR_STATMENT(foreach);
}

static IrStatment *
foreach_range_to_ir(compilation_status_t *compile_status,
                    sym_table_t *parent_symbols,
                    AstForeachRange *ast_foreach_range)
{
    assert(compile_status);
    assert(parent_symbols);
    assert(AST_IS_FOREACH_RANGE(ast_foreach_range));

    /* convert index variable to ir */
    IrVariable *index =
        var_decl_to_ir(ast_foreach_range_get_index(ast_foreach_range));

    /* store index variable in foreach's local symbols table */
    sym_table_t *loop_symbols = sym_table_new(parent_symbols);
    sym_table_add_symbol(loop_symbols, ir_symbol(index));

    /* conver lower expression to ir */
    IrExpression *lower_exp =
        expression_to_ir(compile_status,
                         parent_symbols,
                         ast_foreach_range_get_lower_exp(ast_foreach_range));

    /* conver upper expression to ir */
    IrExpression *upper_exp =
        expression_to_ir(compile_status,
                         parent_symbols,
                         ast_foreach_range_get_upper_exp(ast_foreach_range));

    /* convert foreach body to ir */
    IrCodeBlock *body = ir_code_block_new(loop_symbols);
    code_block_to_ir(compile_status,
                     ast_foreach_range_get_body(ast_foreach_range),
                     body);

    if (lower_exp == NULL || upper_exp == NULL)
    {
        /* invalid lower/upper expressions, bail out */
        return NULL;
    }

    return
        IR_STATMENT(ir_foreach_range_new(index, lower_exp, upper_exp, body));
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

  return
      IR_STATMENT(ir_return_new(ir_return_value,
                                ast_node_get_line_num(ast_return)));
}

static IrStatment *
assignment_to_ir(compilation_status_t *compile_status,
                sym_table_t *symbols,
                AstAssignment *ast_assignment)
{
    AstExpression *lvalue;
    AstExpression *value;
    IrExpression *ir_lvalue;
    IrExpression *ir_value;

    lvalue = ast_assignment_get_lvalue(ast_assignment);
    value = ast_assignment_get_value(ast_assignment);

    ir_lvalue =
        expression_to_ir(compile_status, symbols, lvalue);

    ir_value =
        expression_to_ir(compile_status, symbols, value);

    if (ir_lvalue == NULL || ir_value == NULL)
    {
        return NULL;
    }

    return
        IR_STATMENT(ir_assignment_new(ir_lvalue,
                                      ir_value,
                                      ast_node_get_line_num(ast_assignment)));
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
        case ast_modulo_op:
        case ast_equal_op:
        case ast_not_equal_op:
        case ast_less_op:
        case ast_greater_op:
        case ast_less_or_eq_op:
        case ast_greater_or_eq_op:
        case ast_and_op:
        case ast_or_op:
            return ir_expression(ir_binary_operation_new(op,
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

static IrExpression *
ast_conditional_to_ir(compilation_status_t *compile_status,
                      sym_table_t *symbols,
                      AstConditional *ast_cond)
{
    IrExpression *cond;
    IrExpression *true_exp;
    IrExpression *false_exp;

    cond =
        expression_to_ir(compile_status,
                         symbols,
                         ast_conditional_get_cond(ast_cond));
    true_exp =
        expression_to_ir(compile_status,
                         symbols,
                         ast_conditional_get_true(ast_cond));

    false_exp =
        expression_to_ir(compile_status,
                         symbols,
                         ast_conditional_get_false(ast_cond));

    if (cond == NULL || true_exp == NULL || false_exp == NULL)
    {
        /* error(s) converting sub-expression to ir, bail out */
        return NULL;
    }

    return
        ir_expression(
            ir_conditional_new(cond,
                               true_exp,
                               false_exp,
                               ast_node_get_line_num(AST_NODE(ast_cond))));
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

    return
        ir_expression(
            ir_unary_operation_new(
                op_type,
                operand,
                ast_node_get_line_num(AST_NODE(ast_operation))));
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

    return ir_expression(ir_function_call_new(func_name,
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
        if (exp == NULL)
        {
            /* invalid expression, bail out */
            return NULL;
        }
        ir_array_literal_add_value(ir_arry_literal, exp);
    }

    return ir_expression(ir_arry_literal);
}

static IrExpression *
string_literal_to_ir(compilation_status_t *compile_status,
                     sym_table_t *symbols,
                     AstStringLiteral *ast_str_literal)
{
    assert(compile_status);
    assert(symbols);
    assert(AST_IS_STRING_LITERAL(ast_str_literal));

    IrArrayLiteral *arry_literal;
    GSList *vals = NULL;
    gchar *str;
    gchar c;

    /*
     * build an IR char array literal from AST string literal
     */

    /* create a list of char constant for each characted in the string */
    str = ast_string_literal_get_str(ast_str_literal);
    while((c = *(str++)) != '\0')
    {
        vals = g_slist_prepend(vals,
                               ir_basic_constant_new_char(c, 0));
    }

    /* build IR array literal object */
    arry_literal = ir_array_literal_new(0);
    ir_array_literal_set_values(arry_literal, g_slist_reverse(vals));

    return ir_expression(arry_literal);
}

static IrExpression *
array_slice_to_ir(compilation_status_t *compile_status,
                  sym_table_t *symbols,
                  AstArraySliceRef *ast_arry_slice)
{
    AstExpression *ast_start_idx;
    IrExpression *ir_array;
    IrExpression *ir_start_idx = NULL;
    AstExpression *ast_end_idx;
    IrExpression *ir_end_idx = NULL;
    guint line_number;

    ir_array = expression_to_ir(compile_status,
                                symbols,
                                ast_array_slice_ref_get_array(ast_arry_slice));
    if (ir_array == NULL)
    {
        return NULL; /* invalid expression, bail out */
    }

    /* convert start expression, if any, to IR-form */
    ast_start_idx = ast_array_slice_ref_get_start(ast_arry_slice);
    if (ast_start_idx != NULL)
    {
        ir_start_idx =
            expression_to_ir(compile_status, symbols, ast_start_idx);
        if (ir_start_idx == NULL)
        {
            return NULL; /* invalid expression, bail out */
        }
    }

    /* convert end expression, if any, to IR-form */
    ast_end_idx = ast_array_slice_ref_get_end(ast_arry_slice);
    if (ast_end_idx != NULL)
    {
        ir_end_idx = expression_to_ir(compile_status, symbols, ast_end_idx);
        if (ir_end_idx == NULL)
        {
            return NULL; /* invalid expression, bail out */
        }
    }

    line_number = ast_node_get_line_num(ast_arry_slice);
    return ir_expression(ir_array_slice_new(ir_array,
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
    const char *array_name;
    IrSymbol *array;
    IrExpression *ir_index_exp;

    /*
     * convert array index expression to IR form
     */
    array_name = ast_array_cell_ref_get_name(array_cell_ref);
    ast_index_exp = ast_array_cell_ref_get_index(array_cell_ref);
    ir_index_exp = expression_to_ir(compile_status, symbols, ast_index_exp);

    /* look-up array symbol */
    array = sym_table_get_symbol(symbols, array_name, NULL);
    if (array == NULL)
    {
        compile_error(compile_status,
                      array_cell_ref,
                      "reference to unknown array symbol '%s'\n",
                      array_name);
        return NULL;
    }
    else if (!IR_IS_VARIABLE(array))
    {
        compile_error(compile_status,
                      array_cell_ref,
                      "unexpected reference to non variable symbol '%s'\n",
                      array_name);
        return NULL;
    }

    if (ir_index_exp == NULL)
    {
        /* invalid index expression, bail out */
        return NULL;
    }


    return
        ir_expression(
            ir_array_cell_new(IR_VARIABLE(array),
                              ir_index_exp,
                              ast_node_get_line_num(array_cell_ref)));
}

static IrExpression *
cast_to_ir(compilation_status_t *compile_status,
           sym_table_t *symbols,
           AstCast *ast_cast)
{
    assert(compile_status);
    assert(symbols);
    assert(AST_IS_CAST(ast_cast));

    IrExpression *exp =
        expression_to_ir(compile_status,
                         symbols,
                         ast_cast_get_value(ast_cast));

    if (exp == NULL)
    {
        /* invalid expression, bail out */
        return NULL;
    }

    return
        ir_expression(
            ir_cast_new(ast_cast_get_target_type(ast_cast), exp));
}

static IrExpression *
postfix_exp_to_ir(compilation_status_t *compile_status,
                  sym_table_t *symbols,
                  AstPostfixExp *ast_postfix)
{
    assert(compile_status);
    assert(symbols);
    assert(AST_IS_POSTFIX_EXP(ast_postfix));
    IrExpression *left;
    IrExpression *right;

    left = expression_to_ir(compile_status,
                            symbols,
                            ast_postfix_exp_get_expression(ast_postfix));
    if (left == NULL)
    {
        /* invalid expression, bail out */
        return NULL;
    }

    right = ir_expression(ir_ident_new(ast_postfix_exp_get_name(ast_postfix),
                                       ast_node_get_line_num(ast_postfix)));

    return ir_expression(ir_dot_new(left,
                                    right,
                                    ast_node_get_line_num(ast_postfix)));
}

static IrExpression *
ptr_dref_exp_to_ir(compilation_status_t *compile_status,
                   sym_table_t *symbols,
                   AstPtrDref *ast_ptr_dref)
{
    IrExpression *ptr_expr;

    ptr_expr = expression_to_ir(compile_status,
                                symbols,
                                ast_ptr_dref_get_ptr_expression(ast_ptr_dref));
    if (ptr_expr == NULL)
    {
        /* invalid expression, bail out */
        return NULL;
    }

    return
        ir_expression(
            ir_ptr_dref_new(ptr_expr,
                            ast_node_get_line_num(AST_NODE(ast_ptr_dref))));
}

static IrExpression *
address_of_to_ir(compilation_status_t *compile_status,
                 sym_table_t *symbols,
                 AstAddressOf *ast_addr_of)
{
    assert(AST_IS_ADDRESS_OF(ast_addr_of));

    IrExpression *expr;

    expr = expression_to_ir(compile_status,
                            symbols,
                            ast_address_of_get_expression(ast_addr_of));
    if (expr == NULL)
    {
        /* invalid expression, bail out */
        return NULL;
    }

    return
        ir_expression(
            ir_address_of_new(expr,
                              ast_node_get_line_num(AST_NODE(ast_addr_of))));
}

static IrExpression *
ident_to_ir(compilation_status_t *compile_status,
            sym_table_t *symbols,
            AstIdent *ident)
{
    assert(compile_status);
    assert(symbols);
    assert(AST_IS_IDENT(ident));

    IrSymbol *symb;
    GError *err = NULL;

    symb = sym_table_get_symbol(symbols, ast_ident_get_name(ident), &err);
    if (symb == NULL)
    {
        switch (err->code)
        {
            case SYM_TABLE_SYMBOL_NOT_FOUND_ERROR:
                compile_error(compile_status,
                              ident,
                              "reference to unknown symbol '%s'\n",
                              ast_ident_get_name(ident));
                break;
            case SYM_TABLE_MULTIPLE_SYMBOLS_FOUND_ERROR:
                compile_error(compile_status,
                              ident,
                              "ambiguous reference '%s', matches: %s\n",
                              ast_ident_get_name(ident),  err->message);
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
                      ast_ident_get_name(ident));
        return NULL;
    }
    else if (IR_IS_VARIABLE(symb))
    {
        IrVariable *var = IR_VARIABLE(symb);

        if (ir_variable_is_ref(var))
        {
            return
                ir_expression(ir_var_ref_new(IR_VARIABLE(var),
                                             ast_node_get_line_num(ident)));
        }
        else
        {
            return
                ir_expression(ir_var_value_new(IR_VARIABLE(var),
                                               ast_node_get_line_num(ident)));
        }
    }

    return ir_expression(symb);
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

    guint line_num = ast_node_get_line_num(AST_NODE(ast_expression));

    if (AST_IS_INT_CONSTANT(ast_expression))
    {
        gint32 val;

        val = ast_int_constant_get_value(AST_INT_CONSTANT(ast_expression));
        return ir_expression(ir_basic_constant_new_int(val, line_num));
    }
    else if (AST_IS_UINT_CONSTANT(ast_expression))
    {
        guint32 val;

        val = ast_uint_constant_get_value(AST_UINT_CONSTANT(ast_expression));
        return ir_expression(ir_basic_constant_new_uint(val, line_num));
    }
    else if (AST_IS_BOOL_CONSTANT(ast_expression))
    {
        bool val;

        val = ast_bool_constant_get_value(AST_BOOL_CONSTANT(ast_expression));
        return ir_expression(ir_basic_constant_new_bool(val, line_num));
    }
    else if (AST_IS_CHAR_CONSTANT(ast_expression))
    {
        guint8 val;

        val = ast_char_constant_get_value(AST_CHAR_CONSTANT(ast_expression));
        return ir_expression(ir_basic_constant_new_char(val, line_num));
    }
    else if (AST_IS_ARRAY_LITERAL(ast_expression))
    {
        AstArrayLiteral *arry_literal;

        arry_literal = AST_ARRAY_LITERAL(ast_expression);
        return array_literal_to_ir(compile_status, symbols, arry_literal);
    }
    else if (AST_IS_STRING_LITERAL(ast_expression))
    {
        AstStringLiteral *string_literal;

        string_literal = AST_STRING_LITERAL(ast_expression);
        return string_literal_to_ir(compile_status, symbols, string_literal);
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
    else if (AST_IS_IDENT(ast_expression))
    {
        return ident_to_ir(compile_status, symbols, AST_IDENT(ast_expression));
    }
    else if (AST_IS_CONDITIONAL(ast_expression))
    {
        AstConditional *cond;

        cond = AST_CONDITIONAL(ast_expression);

        return ast_conditional_to_ir(compile_status, symbols, cond);
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
    else if (AST_IS_CAST(ast_expression))
    {
        return cast_to_ir(compile_status,
                          symbols,
                          AST_CAST(ast_expression));
    }
    else if (AST_IS_POSTFIX_EXP(ast_expression))
    {
        return postfix_exp_to_ir(compile_status,
                                 symbols,
                                 AST_POSTFIX_EXP(ast_expression));
    }
    else if (AST_IS_PTR_DREF(ast_expression))
    {
        return ptr_dref_exp_to_ir(compile_status,
                                  symbols,
                                  AST_PTR_DREF(ast_expression));
    }
    else if (AST_IS_ADDRESS_OF(ast_expression))
    {
        return address_of_to_ir(compile_status,
                                symbols,
                                AST_ADDRESS_OF(ast_expression));
    }

    /* unexpected expression type */
    printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(ast_expression)));
    g_assert_not_reached();
    return NULL;
}
