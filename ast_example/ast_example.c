#include <stdio.h>
#include "ast_compile_unit.h"
#include "ast_basic_type.h"
#include "ast_return.h"
#include "ast_int_constant.h"
#include "ast_bool_constant.h"
#include "ast_variable_declaration.h"
#include "ast_binary_operation.h"
#include "ast_variable_definition.h"
#include "ast_scalar_variable_ref.h"

/**
 * return ast for the following D function:
 * int tri_area(int basen, int hojden)
 * {
 *     int x;
 *     bool yarol = false;
 *     
 *     return basen + 5;
 * }
 */
AstFunction *
build_tri_area()
{
    AstCodeBlock *code_block;
    AstBinaryOperation *bin_op;
    AstReturn *ret;
    AstVariableDefinition *var_def;
    AstBoolConstant *bool_const;
    AstVariableDeclaration *var_dec;
    GSList *params = NULL;

    /* create the function body code_block node */
    code_block = ast_code_block_new();

    /* int yarol = false */
    bool_const = ast_bool_constant_new(false);
    var_def =
        ast_variable_definition_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                                    "yarol", XDP_AST_EXPRESSION(bool_const));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(var_def));


    /* int x */
    var_def = 
        ast_variable_definition_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                                    "x", NULL);
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(var_def));

    /* return basen + 5 */
    bin_op = 
        ast_binary_operation_new(ast_plus_op,
                                 XDP_AST_EXPRESSION(ast_scalar_variable_ref_new("basen")),
                                 XDP_AST_EXPRESSION(ast_int_constant_new(5)));
    ret = ast_return_new(XDP_AST_EXPRESSION(bin_op));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(ret));

    /* the function arguments */
    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), "basen");
    params = g_slist_append(params, var_dec);
    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), "hojden");
    params = g_slist_append(params, var_dec);

    return ast_function_new("tri_area", 
                            params,
                            XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                            code_block);
}

/**
 * return ast for the following (syntacticly incorrect) D function:
 * int foo(int a, int b, bool foo)
 * {
 *     return 42;
 *     return false;
 * }
 */
AstFunction *
build_foo()
{
    AstCodeBlock *code_block;
    AstReturn *ret;
    AstIntConstant *c42;
    GSList *params = NULL;
    AstVariableDeclaration *var_dec;

    code_block = ast_code_block_new();
    c42 = ast_int_constant_new(42);
    ret = ast_return_new(XDP_AST_EXPRESSION(c42));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(ret));

    ret = ast_return_new(XDP_AST_EXPRESSION(ast_bool_constant_new(false)));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(ret));


    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), "a");
    params = g_slist_append(params, var_dec);
    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), "b");
    params = g_slist_append(params, var_dec);
    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)), "foo");
    params = g_slist_append(params, var_dec);

    return ast_function_new("foo", params, 
                            XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), code_block);
}

int main()
{
    g_type_init();
    AstCompileUnit *comp_unit;

    printf("create some ast objects...\n");

    comp_unit = ast_compile_unit_new();
    ast_compile_unit_add_function(comp_unit, build_foo());
    ast_compile_unit_add_function(comp_unit, build_tri_area());

    printf("print comp_unit\n");
    ast_node_print(XDP_AST_NODE(comp_unit), stdout);

    return 0;
}
