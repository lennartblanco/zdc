#include <stdio.h>
#include <stdbool.h>

#include "sem_analyze.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_assigment.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
sem_analyze_function(AstFunction *ast_func, ir_compile_unit_t *comp_unit)
{
    GSList *listp;
    ir_function_def_t *ir_func;

    /* create the IR function object */
    ir_func = 
        ir_function_def_new(ir_compile_unit_get_global_sym_table(comp_unit));

    /*
     * Store arguments in the IR function object
     */
    listp = ast_function_get_parameters(ast_func);
    for (;listp != NULL; listp = listp->next)
    {
        AstVariableDeclaration *param;

        param = XDP_AST_VARIABLE_DECLARATION(listp->data);
        ir_variable_def_t *var = 
            ir_variable_def_new(ast_variable_declaration_get_name(param), param->type);
        ir_function_def_add_parameter(ir_func, var);
    }

    /*
     * Analyze the statments in the function body
     */
    AstCodeBlock *body = ast_function_get_body(ast_func);
    AstCodeBlock *new_body = ast_code_block_new();

    listp = ast_code_block_get_statments(body);
    for (; listp != NULL; listp = listp->next)
    {
        AstStatment *stmt = XDP_AST_STATMENT(listp->data);
        ir_variable_def_t *var;

        if (XDP_IS_AST_VARIABLE_DEFINITION(stmt))
        {
            /* put the variable definition into the local symbol table */
            AstVariableDefinition *var_def =
                XDP_AST_VARIABLE_DEFINITION(stmt);

            var = 
                ir_variable_def_new(ast_variable_definition_get_name(var_def), 
                                    ast_variable_definition_get_data_type(var_def));
            ir_function_def_add_local_var(ir_func, var);

            /* check if there is any initialization expression */
            AstExpression *init_exp =
                ast_variable_definition_get_initializer(var_def);
            if (init_exp != NULL)
            {
                /* add the initialization assigment node to new body */
                AstVariableRef *var_ref = ast_variable_ref_new(
                        ast_variable_definition_get_name(var_def));

                AstAssigment *assign =
                    ast_assigment_new(XDP_AST_VARIABLE_REF(var_ref), init_exp);

                ast_code_block_add_statment(new_body, XDP_AST_STATMENT(assign));
       
            }
            /** @todo delete the stmt node here */
        }
        else
        {
            ast_code_block_add_statment(new_body, stmt);
        }
    }
/** @todo: delete (old) body node */
//    ast_node_del(body);

    ir_function_def_set_name(ir_func,
                             ast_function_get_name(ast_func));
    ir_function_def_set_body(ir_func, new_body);
    ir_function_def_set_return_type(ir_func,
                                    ast_function_get_return_type(ast_func));
    ir_compile_unit_add_function(comp_unit, ir_func);
}

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

ir_compile_unit_t *
semantic_analyze(AstCompileUnit *ast_compile_unit)
{
    ir_compile_unit_t *comp_unit;
    GSList *ptr;

    comp_unit = ir_compile_unit_new();
    ptr = ast_compile_unit_get_functions(ast_compile_unit);

    while (ptr != NULL)
    {
        sem_analyze_function(XDP_AST_FUNCTION(ptr->data), comp_unit);
        ptr = ptr->next;
    }

    return comp_unit;
}


