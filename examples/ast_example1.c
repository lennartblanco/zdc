#include <stdio.h>
#include "ast_compile_unit.h"
#include "ast_basic_type.h"
#include "ast_return.h"
#include "ast_int_constant.h"
#include "ast_bool_constant.h"
#include "ast_variable_declaration.h"
#include "ast_binary_operation.h"
#include "ast_unary_operation.h"
#include "ast_variable_definition.h"
#include "ast_assigment.h"
#include "ast_function_call.h"

/**
 * return ast for the following D function:
 * int tri_area(int basen, int hojden)
 * {
 *     int x;
 *     bool yarol = false;
 *
 *     x = 3;
 *
 *     return basen + 5;
 * }
 */
AstFunction *
build_tri_area()
{
    AstCodeBlock *code_block;
    AstBinaryOperation *bin_op;
    AstUnaryOperation *unary_op;
    AstReturn *ret;
    AstVariableDefinition *var_def;
    AstBoolConstant *bool_const;
    AstVariableDeclaration *var_dec;
    AstAssigment *assign;
    GSList *params = NULL;

    /* create the function body code_block node */
    code_block = ast_code_block_new();

    /* int x */
    var_def = 
        ast_variable_definition_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                                    "x", NULL);
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(var_def));

    /* int yarol = false */
    bool_const = ast_bool_constant_new(false);
    var_def =
        ast_variable_definition_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)),
                                    "yarol", XDP_AST_EXPRESSION(bool_const));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(var_def));

    /* x = 3 */
    assign =
        ast_assigment_new(ast_variable_ref_new("x"),
                          XDP_AST_EXPRESSION(ast_int_constant_new(3)));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(assign));

    /* return -(basen + 5) */
    bin_op = 
        ast_binary_operation_new(ast_plus_op,
                                 XDP_AST_EXPRESSION(ast_variable_ref_new("basen")),
                                 XDP_AST_EXPRESSION(ast_int_constant_new(5)));
    unary_op =
        ast_unary_operation_new(ast_arithm_neg_op, XDP_AST_EXPRESSION(bin_op));

    ret = ast_return_new(XDP_AST_EXPRESSION(unary_op));
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
 *     return bar(42);
 *     return false;
 * }
 */
AstFunction *
build_foo()
{
    AstCodeBlock *code_block;
    AstReturn *ret;
    AstIntConstant *c42;
    GSList *list = NULL;
    AstVariableDeclaration *var_dec;
    AstFunctionCall *func_call;

    /* return bar(42) */
    code_block = ast_code_block_new();
    c42 = ast_int_constant_new(42);
    list = g_slist_append(list, c42);
    func_call = ast_function_call_new("bar", list);
    ret = ast_return_new(XDP_AST_EXPRESSION(func_call));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(ret));

    /* return false */
    ret = ast_return_new(XDP_AST_EXPRESSION(ast_bool_constant_new(false)));
    ast_code_block_add_statment(code_block, XDP_AST_STATMENT(ret));

    list = NULL;
    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), "a");
    list = g_slist_append(list, var_dec);
    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)), "b");
    list = g_slist_append(list, var_dec);
    var_dec = 
        ast_variable_declaration_new(XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type)), "foo");
    list = g_slist_append(list, var_dec);

    return ast_function_new("foo", list, 
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
