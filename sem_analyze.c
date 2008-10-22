#include <stdio.h>
#include <stdbool.h>

#include <assert.h>

#include "sem_analyze.h"

void
sem_analyze_function(ast_node_t *node, void *p)
{
    GSList *listp;
    ir_function_def_t *func;
    ir_compile_unit_t *comp_unit = (ir_compile_unit_t*) p;

    func = new_ir_function_def();

    printf("func <%s>\n", node->data.function_def.name);
    
    /*
     * Store arguments in the IR function object
     */
    if (node->data.function_def.formal_args != NULL)
    {
        listp = node->data.function_def.formal_args->params;

        while (listp != NULL)
        {
            ast_var_decl_t *param = listp->data;
            ir_variable_def_t *var = 
                             new_ir_variable_def(param->name, param->type);
            ir_function_def_add_parameter(func, var);
            listp = listp->next;
        }
    }


    /*
     * Analyze the statments in the function body
     */
    printf("body statments\n");

    ast_node_t *body =  node->data.function_def.body;
    assert(body->type == ast_code_block_node);
    ast_node_t *new_body = new_code_block();
    listp = body->data.code_block.statments;

    while(listp != NULL)
    {
        ast_node_t *stat;
        ir_variable_def_t *var;

        stat = listp->data;
        switch (stat->type)
        {
            case ast_var_declaration_node:
                var = new_ir_variable_def(stat->data.var_decl.name, 
                                          stat->data.var_decl.type);
                ir_function_def_add_local_var(func, var);
                ast_node_del(stat);
                break;
            default:
                code_block_add_statment(new_body, stat);
                break;
        }
        listp = listp->next;
    }

    ast_node_del(body);

    ir_function_def_set_name(func, node->data.function_def.name);
    ir_function_def_set_body(func, new_body);
    ir_compile_unit_add_function(comp_unit, func);

    printf("\n");
}


ir_compile_unit_t *
semantic_analyze(ast_node_t *node)
{
    ir_compile_unit_t *comp_unit;

    comp_unit = new_ir_compile_unit();

    compile_unit_for_each_function(node, sem_analyze_function, comp_unit);

    return comp_unit;
}


