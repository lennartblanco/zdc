#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

#include <assert.h>

ast_node_t *
new_binary_operation(ast_oper_type_t oper_type, 
                     ast_node_t *left, 
                     ast_node_t *right)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_binary_oper_node;
    node->data.binary_op.oper_type = oper_type;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;

    return node;
}

ast_node_t *
new_const(int value)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));
    
    node->type = ast_constant_node;
    node->data.constant.value = value;

    return node;
}

ast_node_t *
new_negation(ast_node_t *value)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_negation_node;
    node->data.negation.value = value;

    return node;
}

ast_node_t *
new_var_declaration(ast_data_type_t var_type, 
                    char *var_name)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_var_declaration_node;
    node->data.var_decl.type = var_type;
    node->data.var_decl.name = var_name;

    return node;  
}

ast_node_t *
new_assigment(char *lvalue, ast_node_t *value)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_assigment_node;
    node->data.assigment.lvalue = lvalue;
    node->data.assigment.value = value;

    return node;  
}

ast_node_t *
new_var_value(char *name)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_var_value_node;
    node->data.var_val.name = name;

    return node;
}

ast_node_t *
new_function_call(char *name, 
                  ast_func_args_list_t *args)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_function_call_node;
    node->data.function_call.name = name;
    node->data.function_call.args = args;

    return node;    
}

ast_node_t *
new_statment_list(ast_node_t *cur, ast_node_t *next)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_statment_list_node;
    node->data.stmt.car = cur;
    node->data.stmt.next = next;

    return node;
}

ast_node_t *
new_function_definition(char *name, 
                        ast_data_type_t ret_type,
                        void *arg_lst, 
                        ast_node_t *body)
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_function_definition_node;
    node->data.function_def.name = name;
    node->data.function_def.ret_type = ret_type;
    node->data.function_def.formal_args = arg_lst;
    node->data.function_def.body = body;
 
    return node;
}

ast_func_args_list_t *
new_func_args_list(ast_node_t *value,
                   ast_func_args_list_t *next)
{
    ast_func_args_list_t *args;

    args = g_malloc(sizeof(*args));

    args->value = value;
    args->next = next;

    return args;
}

ast_func_parameters_t *
new_func_parameters()
{
    ast_func_parameters_t *param;

    param = g_malloc(sizeof(*param));
    param->params = NULL;

    return param;
}

void 
func_parameters_add_param(ast_func_parameters_t *func_param,
                          ast_var_decl_t *new_param)
{
    func_param->params = g_slist_prepend(func_param->params, new_param);
}

void
func_parameters_for_each(ast_func_parameters_t *func_params, 
                         void (*iter) (ast_var_decl_t *param))
{
    assert(func_params && iter);

    GSList *listp = func_params->params;

    while(listp != NULL)
    {
        iter(listp->data);
        listp = listp->next;
    }
}


ast_node_t *
new_return_statment(ast_node_t *ret_val)
{
    ast_node_t *node;
    node = g_malloc(sizeof(*node));

    node->type = ast_return_statment_node;
    node->data.ret_stmt.ret_val = ret_val;

    return node;
}


ast_node_t *
new_compile_unit()
{
    ast_node_t *node;

    node = g_malloc(sizeof(*node));

    node->type = ast_compile_unit_node;
    node->data.compile_unit.functions = NULL;
    
    return node;
}

void
compile_unit_add_function(ast_node_t *compile_unit,
                          ast_node_t *function)
{
    compile_unit->data.compile_unit.functions = 
                       g_slist_append(
                           compile_unit->data.compile_unit.functions, function);
}
