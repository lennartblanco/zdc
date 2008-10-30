#include <stdio.h>
#include <stdbool.h>

#include "ast.h"
#include "sym_table.h"
#include "gen_java.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static char *
gen_java_data_type_to_str(ast_data_type_t t);

static void
gen_java_handle_node(ast_node_t *node, sym_table_t *sym_table);

static void
gen_java_handle_function_def(ir_function_def_t *func);

static void
gen_java_handle_return_statment(ast_node_t *node, sym_table_t *sym_table);

static void
gen_java_handle_var_value(ast_node_t *node, sym_table_t *sym_table);

static void
gen_java_handle_assigment(ast_node_t *node, sym_table_t *sym_table);

static void
gen_java_handle_binary_op(ast_node_t *node, sym_table_t *sym_table);

static void
gen_java_handle_func_call(ast_node_t *node, sym_table_t *sym_table);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

void
gen_java_prelude()
{
    printf(
".class public foo\n"
".super java/lang/Object\n"
"\n"
".method public <init>()V\n"
"   aload_0\n"
"\n" 
"   invokespecial java/lang/Object/<init>()V\n"
"   return\n"
".end method\n"
"\n"
    );
}

void
gen_java_epilog()
{
    printf(
".method public static main([Ljava/lang/String;)V\n"
"   .limit stack 16\n"
"   .limit locals 16\n"
"   return\n"
".end method\n"
     );
}


void
gen_java_code(ir_compile_unit_t *comp_unit)
{
    GList *p;

    gen_java_prelude();

    p = ir_compile_unit_get_functions(comp_unit);
    for (; p != NULL; p = g_list_next(p))
    {
        gen_java_handle_function_def(ir_symbol_get_function(p->data));
    }
    g_list_free(p);

    gen_java_epilog();

}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
gen_java_handle_code_block(ast_node_t *node, sym_table_t *sym_table)
{
    GSList *stmts;

    stmts = code_block_get_statments(node);

    for (;stmts != NULL; stmts = g_slist_next(stmts))
    {
        gen_java_handle_node(stmts->data, sym_table);
    }
}

static void
gen_java_handle_node(ast_node_t *node, sym_table_t *sym_table)
{
    switch (node->type)
    {
        case ast_code_block_node:
            gen_java_handle_code_block(node, sym_table);
            break;
        case ast_return_statment_node:
            gen_java_handle_return_statment(node, sym_table);
            break;
        case ast_var_value_node:
            gen_java_handle_var_value(node, sym_table);
            break;	
        case ast_assigment_node:
            gen_java_handle_assigment(node, sym_table);
            break;
        case ast_constant_node:
            printf("    ldc %d\n", node->data.constant.value);
	    break;
        case ast_binary_oper_node:
            gen_java_handle_binary_op(node, sym_table);
            break;
        case ast_function_call_node:
            gen_java_handle_func_call(node, sym_table);
            break;
/*        case ast_negation_node:*/
/*            gen_java_code(node->data.negation.value);*/
/*            printf("    ineg\n");*/
/*            break;*/
        default:
            printf("; node->type %s\n", ast_node_to_str(node->type));
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
        default:
            assert(false);
    }
    return NULL;
}

static void
gen_java_handle_func_call(ast_node_t *node, sym_table_t *sym_table)
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
        gen_java_handle_node(arg->value, sym_table);
        arg = arg->next;
    }

    printf("    invokestatic foo/%s(", node->data.function_call.name);

    p = ir_function_def_get_parameters(func);
    for (; p != NULL; p = g_slist_next(p))
    {
        ir_variable_def_t *v = p->data;
        printf("%s", gen_java_data_type_to_str(ir_variable_def_get_type(v)));
    }

    printf(")%s\n", 
           gen_java_data_type_to_str(ir_function_def_get_return_type(func)));
}


static void
gen_java_handle_binary_op(ast_node_t *node, sym_table_t *sym_table)
{
    gen_java_handle_node(node->data.binary_op.left, sym_table);
    gen_java_handle_node(node->data.binary_op.right, sym_table);

    switch (node->data.binary_op.oper_type)
    {
        case ast_plus_op:
            printf("    iadd\n");
            break;
        case ast_minus_op:
            printf("    isub\n");
            break;
        case ast_mult_op:
            printf("    imul\n");
            break;
        case ast_division_op:
            printf("    idiv\n");
            break;
    }
}

static void
gen_java_handle_assigment(ast_node_t *node, sym_table_t *sym_table)
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

    gen_java_handle_node(node->data.assigment.value, sym_table);
    addr = ir_variable_def_get_address(ir_symbol_get_variable(symb));

    if (0 <= addr.java_variable_addr && addr.java_variable_addr <= 3)
    {
        printf("    istore_%d\n", addr.java_variable_addr);
    }
    else
    {
        printf("    istore %d\n", addr.java_variable_addr);
    }
}


static void
gen_java_handle_var_value(ast_node_t *node, sym_table_t *sym_table)
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
                printf("    iload_%d\n", addr.java_variable_addr);
            }
            else
            {
                printf("    iload %d\n", addr.java_variable_addr);
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
gen_java_handle_return_statment(ast_node_t *node, sym_table_t *sym_table)
{
    assert(node);
    assert(node->type == ast_return_statment_node);
    assert(sym_table);

    ast_node_t *ret_val;

    ret_val = return_statment_get_value(node);

    if (ret_val == NULL)
    {
        /* assume void return for now */
        printf("    return\n");
    }
    else
    {
        /* assume int return for now */
        gen_java_handle_node(ret_val, sym_table);
        printf("    ireturn\n");
    }
}

static void
gen_java_handle_function_def(ir_function_def_t *func)
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
    printf(".method public static %s(", ir_function_def_get_name(func));

    /* generate function parameters types and count parameters */
    p = ir_function_def_get_parameters(func);
    for (; p != NULL; p = g_slist_next(p))
    {
        var = p->data;
        param_num += 1;
        printf("%s", gen_java_data_type_to_str(ir_variable_def_get_type(var)));
    }

    printf(")%s\n", 
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

    printf("    .limit locals %d\n", param_num + local_var_num);
    printf("    .limit stack 32\n");    
    gen_java_handle_code_block(ir_function_def_get_body(func), local_vars);
    printf(".end method\n");
}

