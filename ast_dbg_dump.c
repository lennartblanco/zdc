#include "ast_dbg_dump.h"

static FILE * outs;

static void
dump_type(data_type_t type)
{
    switch(type)
    {
        case void_type:
            fprintf(outs, "void");
            break;
        case integer_type:
            fprintf(outs, "int");
            break;
        default:
            fprintf(outs, "unknown type");
            break;
    }
}

static void
dump_var_decl(var_decl_t *param)
{
    dump_type(param->type);
    fprintf(outs, " %s ", param->name);
}

static void
dump_function_params(func_parameters_t *params)
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
        case function_definition:
            dump_function_definition(root);
            break;
        case statment_list:
            dump_statment_list(root);
            break;
        case return_statment:
            dump_return_statment(root);
            break;
        case function_call:
            printf("function_call\n");
            break;
        case var_declaration:
            printf("var_declaration\n");
            break;
        case var_value:
            printf("var_value\n");
            break;
        case assigment:
            printf("assigment\n");
            break;
        case binary_oper:
            printf("binary_oper\n");
            break;
        case constant:
            printf("constant\n");
            break;
        case negation:
            printf("negation\n");
            break;
        default:
            fprintf(outs, "unknown node type %d\n", root->type);
            break;
    }
}

