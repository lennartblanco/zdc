#include "ast_dbg_dump.h"

#include <assert.h>

static FILE * outs;

static void
dump_type(ast_data_type_t type)
{
    switch(type)
    {
        case ast_void_type:
            fprintf(outs, "void");
            break;
        case ast_integer_type:
            fprintf(outs, "int");
            break;
        default:
            fprintf(outs, "unknown type");
            break;
    }
}

static void
dump_var_decl(ast_var_decl_t *param)
{
    dump_type(param->type);
    fprintf(outs, " %s ", param->name);
}

static void
dump_function_params(ast_func_parameters_t *params)
{
    func_parameters_for_each(params, dump_var_decl);
}

static void
dump_function_definition(ast_node_t *node)
{
    fprintf(outs, "function '%s' return ", node->data.function_def.name);
    dump_type(node->data.function_def.ret_type);
    if (node->data.function_def.formal_args != NULL)
    {
        fprintf(outs, "\n    parameters: ");
        dump_function_params(node->data.function_def.formal_args);
    }
    fprintf(outs, "\n{\n");
    if (node->data.function_def.body != NULL)
    {
        dump_ast(node->data.function_def.body);
    }
    fprintf(outs, "\n}\n");
    
}

static void
dump_statment_list(ast_node_t *lst)
{
    dump_ast(lst->data.stmt.car);
    if (lst->data.stmt.next != NULL) 
    {
        dump_ast(lst->data.stmt.next);
    }
}

static void
dump_return_statment(ast_node_t *stat)
{
    fprintf(outs, "return ");
    if (stat->data.ret_stmt.ret_val != NULL)
    {
        dump_ast(stat->data.ret_stmt.ret_val);
    }
    else
    {
        fprintf(outs, "\n");
    }
}

void
dump_ast_set_outstream(FILE *stream)
{
    outs = stream;
}


void
dump_ast(ast_node_t *root)
{
    switch(root->type)
    {
        case ast_function_definition_node:
            dump_function_definition(root);
            break;
        case ast_return_statment_node:
            dump_return_statment(root);
            break;
        case ast_function_call_node:
            printf("function_call\n");
            break;
        case ast_var_declaration_node:
            printf("var_declaration\n");
            break;
        case ast_var_value_node:
            printf("var_value\n");
            break;
        case ast_assigment_node:
            printf("assigment\n");
            break;
        case ast_binary_oper_node:
            printf("binary_oper\n");
            break;
        case ast_constant_node:
            printf("constant\n");
            break;
        case ast_negation_node:
            printf("negation\n");
            break;
        default:
            fprintf(outs, "don't know how to dump %s\n", 
                    ast_node_to_str(root->type));
            break;
    }
}

char *
ast_node_to_str(ast_node_type_t type)
{
    switch (type)
    {
        case ast_compile_unit_node:
            return "ast_compile_unit_node";
        case ast_return_statment_node:
            return "ast_return_statment_node";
        case ast_function_definition_node:
            return "ast_function_definition_node";
        case ast_code_block_node:
            return "ast_code_block_node";
        case ast_function_call_node:
            return "ast_function_call_node";
        case ast_var_declaration_node:
            return "ast_var_declaration_node";
        case ast_var_value_node:
            return "ast_var_value_node";
        case ast_assigment_node:
            return "ast_assigment_node";
        case ast_binary_oper_node:
            return "ast_binary_oper_node";
        case ast_constant_node:
            return "ast_constant_node";
        case ast_negation_node:
            return "ast_negation_node";
        case ast_if_else_block_node:
            return "ast_if_else_block_node";
        default:
            /* unknown node type */
            assert(false);
    }
}

