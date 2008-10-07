#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

#include <assert.h>

ast_node_t *new_binary_operation(oper_type_t oper_type, ast_node_t *left, ast_node_t *right)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = binary_oper;
    node->data.binary_op.oper_type = oper_type;
    node->data.binary_op.left = left;
    node->data.binary_op.right = right;

    return node;
}

ast_node_t *new_const(int value)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));
    
    node->type = constant;
    node->data.constant.value = value;

    return node;
}

ast_node_t *new_negation(ast_node_t *value)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = negation;
    node->data.negation.value = value;

    return node;
}

ast_node_t *new_var_declaration(data_type_t var_type, char *var_name)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = var_declaration;  
    node->data.var_decl.type = var_type;
    node->data.var_decl.name = var_name;

    return node;  
}

ast_node_t *new_assigment(char *lvalue, ast_node_t *value)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = assigment;  
    node->data.assigment.lvalue = lvalue;
    node->data.assigment.value = value;

    return node;  
}

ast_node_t *new_var_value(char *name)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = var_value;  
    node->data.var_val.name = name;

    return node;  

}

ast_node_t *new_function_call(char *name, 
                              func_args_list_t *args)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = function_call;  
    node->data.function_call.name = name;
    node->data.function_call.args = args;

    return node;    
}

ast_node_t *
new_statment_list(ast_node_t *cur, ast_node_t *next)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = statment_list;
    node->data.stmt.car = cur;
    node->data.stmt.next = next;

    return node;
}

ast_node_t *
new_function_definition(char *name, data_type_t ret_type, 
                        void *arg_lst, ast_node_t *body)
{
    ast_node_t *node;

    node = malloc(sizeof(*node));

    node->type = function_definition;
    node->data.function_def.name = name;
    node->data.function_def.ret_type = ret_type;
    node->data.function_def.formal_args = arg_lst;
    node->data.function_def.body = body;
 
    return node;
}

func_args_list_t *
new_func_args_list(ast_node_t *value, func_args_list_t *next)
{
    func_args_list_t *args;

    args = malloc(sizeof(*args));

    args->value = value;
    args->next = next;

    return args;
}

func_parameters_t *
new_func_parameters()
{
    func_parameters_t *param;

    param = malloc(sizeof(*param));
    param->params = NULL;

    return param;
}

void 
func_parameters_add_param(func_parameters_t *func_param, var_decl_t *new_param)
{
    func_param->params = g_slist_prepend(func_param->params, new_param);
}

void func_parameters_for_each(func_parameters_t *func_params, 
                              void (*iter) (var_decl_t *param))
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
    node = malloc(sizeof(*node));

    node->type = return_statment;  
    node->data.ret_stmt.ret_val = ret_val;

    return node;  

}



