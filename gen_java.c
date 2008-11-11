#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "ast.h"
#include "sym_table.h"
#include "gen_java.h"
#include "gen_java_int.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

void
gen_java_code(ir_compile_unit_t *comp_unit,
              FILE *out_stream, 
              const char *klass_name)
{
    GList *p;
    gen_java_comp_params_t params; 

    params.out = out_stream;
    params.class_name = klass_name;
    strcpy(params.next_label, "A");
    gen_java_prelude(&params);

    p = ir_compile_unit_get_functions(comp_unit);
    for (; p != NULL; p = g_list_next(p))
    {
        gen_java_handle_function_def(&params, 
                                     ir_symbol_get_function(p->data));
    }
    g_list_free(p);

    gen_java_epilog(&params);

}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
gen_java_prelude(gen_java_comp_params_t *params)
{
    fprintf(params->out,
".class public %s\n"
".super java/lang/Object\n"
"\n"
".method public <init>()V\n"
"   aload_0\n"
"\n" 
"   invokespecial java/lang/Object/<init>()V\n"
"   return\n"
".end method\n"
"\n",
params->class_name
    );
}

static void
gen_java_epilog(gen_java_comp_params_t *params)
{
    fprintf(params->out,
".method public static main([Ljava/lang/String;)V\n"
"   .limit stack 16\n"
"   .limit locals 16\n"
"   return\n"
".end method\n"
     );
}

static void
gen_java_handle_code_block(gen_java_comp_params_t *params,
                           ast_node_t *node,
                           sym_table_t *sym_table)
{
    GSList *stmts;

    stmts = code_block_get_statments(node);

    for (;stmts != NULL; stmts = g_slist_next(stmts))
    {
        gen_java_handle_node(params, stmts->data, sym_table);
    }
}

static void
gen_java_handle_node(gen_java_comp_params_t *params,
                     ast_node_t *node, sym_table_t *sym_table)
{
    switch (node->type)
    {
        case ast_code_block_node:
            gen_java_handle_code_block(params, node, sym_table);
            break;
        case ast_return_statment_node:
            gen_java_handle_return_statment(params, node, sym_table);
            break;
        case ast_var_value_node:
            gen_java_handle_var_value(params, node, sym_table);
            break;	
        case ast_assigment_node:
            gen_java_handle_assigment(params, node, sym_table);
            break;
        case ast_constant_node:
            fprintf(params->out,
                    "    ldc %d\n", node->data.constant.value);
	    break;
        case ast_binary_oper_node:
            gen_java_handle_binary_op(params, node, sym_table);
            break;
        case ast_function_call_node:
            gen_java_handle_func_call(params, node, sym_table);
            break;
        case ast_negation_node:
            gen_java_handle_node(params, node->data.negation.value, sym_table);
            fprintf(params->out, "    ineg\n");
            break;
        default:
            fprintf(params->out,
                    "; node->type %s\n", ast_node_to_str(node->type));
            break;
    }
}

static char *
gen_java_data_type_to_str(ast_data_type_t t)
{
    switch(t)
    {
        case ast_void_type:
            return "V";
        case ast_integer_type:
            return "I";
        case ast_bool_type:
            return "Z";
        default:
            assert(false);
    }
    return NULL;
}

STATIC void
gen_java_get_next_label(gen_java_comp_params_t *params,
                        char *label)
{
    int i;
    char *p;

    strcpy(label, params->next_label);

    p = &(params->next_label[0]);

    i = 0;
    while (true)
    {
        assert(i < MAX_JAVA_LABEL);
        p[i] += 1;
        if (p[i] > 'Z' && p[i] < 'a')
        {
            p[i] = 'a';
            break;
        }
        else if (p[i] > 'z')
        {
            p[i] = 'A';
            i += 1;
            if (p[i] == '\0')
            {
                p[i] = 'A';
                p[i+1] = '\0';
                break;
            }
        }
        else
        {
            break;
        }
    }
}

static void
gen_java_handle_func_call(gen_java_comp_params_t *params,
                          ast_node_t *node, sym_table_t *sym_table)
{
    assert(node);
    assert(node->type == ast_function_call_node);
    assert(sym_table);

    ast_func_args_list_t *arg;
    ir_symbol_t *symb;
    ir_function_def_t *func;
    int res;
    GSList *p;


    res = sym_table_get_symbol(sym_table, node->data.function_call.name,
                               &symb);
    if (res == -1)
    {
        printf("undefined reference to function '%s'\n",
               node->data.function_call.name);
        return;
    }
    func = ir_symbol_get_function(symb);

    arg = node->data.function_call.args;
    while (arg != NULL)
    {
        gen_java_handle_node(params, arg->value, sym_table);
        arg = arg->next;
    }

    fprintf(params->out,
            "    invokestatic %s/%s(",
            params->class_name,
            node->data.function_call.name);

    p = ir_function_def_get_parameters(func);
    for (; p != NULL; p = g_slist_next(p))
    {
        ir_variable_def_t *v = p->data;
        fprintf(params->out, 
                "%s", gen_java_data_type_to_str(ir_variable_def_get_type(v)));
    }

    fprintf(params->out, ")%s\n", 
           gen_java_data_type_to_str(ir_function_def_get_return_type(func)));
}


static void
gen_java_comp_ops_body(gen_java_comp_params_t *params,
                       ast_oper_type_t type)
{
    char trueLabel[MAX_JAVA_LABEL];
    char endLabel[MAX_JAVA_LABEL];
    const char* operationCond;

    gen_java_get_next_label(params, trueLabel);
    gen_java_get_next_label(params, endLabel);

    switch (type)
    {
        case ast_less_op:
            operationCond = "lt";
            break;
        default:
            /* unexpected comparison operation */
            assert(false);
    }
    
    fprintf(params->out,
            "    if_icmp%s %s\n"
            "    iconst_0\n"
            "    goto %s\n"
            "%s:\n"
            "    iconst_1\n"
            "%s:\n", 
            operationCond, trueLabel, endLabel, trueLabel, endLabel);

    
    
}

static void
gen_java_handle_binary_op(gen_java_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table)
{
    gen_java_handle_node(params, node->data.binary_op.left, sym_table);
    gen_java_handle_node(params, node->data.binary_op.right, sym_table);

    ast_oper_type_t operationType = node->data.binary_op.oper_type;

    switch (operationType)
    {
        case ast_less_op:
            gen_java_comp_ops_body(params, operationType);
            break;
        case ast_plus_op:
            fprintf(params->out, "    iadd\n");
            break;
        case ast_minus_op:
            fprintf(params->out, "    isub\n");
            break;
        case ast_mult_op:
            fprintf(params->out, "    imul\n");
            break;
        case ast_division_op:
            fprintf(params->out, "    idiv\n");
            break;
        default:
            /* unexpected operation type */
            assert(false);
    }
}

static void
gen_java_handle_assigment(gen_java_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table)
{

    ir_symbol_t *symb;
    int res;
    ir_symbol_address_t addr;

    res = sym_table_get_symbol(sym_table,
                               node->data.var_val.name,
                               &symb);

    if (res == -1)
    {
        printf("variable '%s' not defined\n", node->data.assigment.lvalue);
        return;
    }

    gen_java_handle_node(params, node->data.assigment.value, sym_table);
    addr = ir_variable_def_get_address(ir_symbol_get_variable(symb));

    if (0 <= addr.java_variable_addr && addr.java_variable_addr <= 3)
    {
        fprintf(params->out, "    istore_%d\n", addr.java_variable_addr);
    }
    else
    {
        fprintf(params->out, "    istore %d\n", addr.java_variable_addr);
    }
}


static void
gen_java_handle_var_value(gen_java_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table)
{
    ir_symbol_t *symb;
    int res;

    res = sym_table_get_symbol(sym_table,
                               node->data.var_val.name,
                               &symb);

    switch (res)
    {
        case 0:
        {
            ir_symbol_address_t addr;

            addr = ir_variable_def_get_address(ir_symbol_get_variable(symb));

            if (0 <= addr.java_variable_addr && addr.java_variable_addr <= 3)
            {
                fprintf(params->out, "    iload_%d\n", addr.java_variable_addr);
            }
            else
            {
                fprintf(params->out, "    iload %d\n", addr.java_variable_addr);
            }
            break;
        }
        case -1:
            printf("variable '%s' not defined\n", node->data.assigment.lvalue);
            break;
        default: /* unexpected return value */
            assert(false);
    }
}

static void
gen_java_handle_return_statment(gen_java_comp_params_t *params,
                                ast_node_t *node,
                                sym_table_t *sym_table)
{
    assert(node);
    assert(node->type == ast_return_statment_node);
    assert(sym_table);

    ast_node_t *ret_val;

    ret_val = return_statment_get_value(node);

    if (ret_val == NULL)
    {
        /* assume void return for now */
        fprintf(params->out, "    return\n");
    }
    else
    {
        /* assume int return for now */
        gen_java_handle_node(params, ret_val, sym_table);
        fprintf(params->out, "    ireturn\n");
    }
}

static void
gen_java_handle_function_def(gen_java_comp_params_t *params,
                             ir_function_def_t *func)
{
    GSList *p;
    GList *l;
    ir_variable_def_t *var;
    guint param_num = 0;
    guint local_var_num = 0;
    guint i;
    sym_table_t *local_vars;
    ir_symbol_t *symb;
    ir_symbol_address_t addr;

    /*
     * generate function header
     */
    fprintf(params->out,
            ".method public static %s(", ir_function_def_get_name(func));

    /* generate function parameters types and count parameters */
    p = ir_function_def_get_parameters(func);
    for (; p != NULL; p = g_slist_next(p))
    {
        var = p->data;
        param_num += 1;
        fprintf(params->out, "%s", 
                gen_java_data_type_to_str(ir_variable_def_get_type(var)));
    }

    fprintf(params->out, ")%s\n", 
           gen_java_data_type_to_str(ir_function_def_get_return_type(func)));

    /* count local variables and assign numbers */
    local_vars = ir_function_def_get_local_vars(func);
    l = sym_table_get_all_symbols(local_vars);
    for (; l != NULL; l = g_list_next(l))
    {
        var = ir_symbol_get_variable(l->data);
        addr.java_variable_addr = param_num + local_var_num;
        ir_variable_def_assign_address(var, addr);
        local_var_num += 1;
    }

    /* 
     * assign numbers to parameter variables and
     * put them into the local symb table
     */
    p = ir_function_def_get_parameters(func);
    for (i = 0; i < param_num; i++)
    {
        var = p->data;
        addr.java_variable_addr = i;
        ir_variable_def_assign_address(var, addr);
        ir_function_def_add_local_var(func, var);
        p = g_slist_next(p);
    }

    fprintf(params->out, "    .limit locals %d\n", param_num + local_var_num);
    fprintf(params->out, "    .limit stack 32\n");    
    gen_java_handle_code_block(params, 
                               ir_function_def_get_body(func),
                               local_vars);
    fprintf(params->out, ".end method\n");
}

