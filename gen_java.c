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
".method public static printNum(I)V\n"
"    .limit stack 16\n"
"    .limit locals 16\n"
"    getstatic java/lang/System/out Ljava/io/PrintStream;\n"
"    iload_0\n"
"    invokevirtual java/io/PrintStream/println(I)V\n"
"    return\n"
".end method\n"
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
gen_java_binary_op(ast_node_t *node)
{
#ifdef UNCOMMENT
    gen_java_code(node->data.binary_op.left);
    gen_java_code(node->data.binary_op.right);

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
#endif
}

void handle_var_declaration(ast_node_t *node)
{
#ifdef UNCOMMENT
    int res;
    int var_num;

    res = sym_table_add_symbol(node->data.var_decl.type, node->data.var_decl.name, &var_num);
    switch (res)
    {
        case 0:
            /* successfully added */
            /* generate code for default value assigment */
            printf("    iconst_0\n");
            if (0 >= var_num && var_num <= 3)
            {
                printf("    istore_%d\n", var_num);
            }
            else
            {
                printf("    istore %d\n", var_num);
            }
            break;
        case -1:
            printf("redclaration of variable '%s'\n", node->data.var_decl.name);
            break;
    }
#endif
}

void
handle_assigment(ast_node_t *node)
{
#ifdef UNCOMMENT
    int var_num;
    
    gen_java_code(node->data.assigment.value);
    switch (find_element(node->data.assigment.lvalue, &var_num))
    {
        case 0:
            if (0 >= var_num && var_num <= 3)
            {
                printf("    istore_%d\n", var_num);
            }
            else
            {
                printf("    istore %d\n", var_num);
            }
            break;
        case -1:
            printf("variable %s not defined\n", node->data.assigment.lvalue);
            break;
    }
#endif
}

void
gen_func_call(ast_node_t *node)
{
#ifdef UNCOMMENT
    ast_func_args_list_t *arg;

    printf("; call %s\n", node->data.function_call.name);
    arg = node->data.function_call.args;
    printf("; --- eval arguments\n");
    while (arg != NULL)
    {
        gen_java_code(arg->value);
        arg = arg->next;
    }
    printf("    invokestatic foo/%s(I)V\n", node->data.function_call.name);
#endif
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
printf("; node->type %s\n", ast_node_to_str(node->type));

    switch (node->type)
    {
        case ast_code_block_node:
            gen_java_handle_code_block(node, sym_table);
            break;
        case ast_return_statment_node:
            gen_java_handle_return_statment(node, sym_table);
        case ast_function_call_node:
            gen_func_call(node);
            break;
        case ast_var_value_node:
            gen_java_handle_var_value(node, sym_table);
            break;	
        case ast_var_declaration_node:
            handle_var_declaration(node);
            break;
        case ast_assigment_node:
            handle_assigment(node);
            break;
        case ast_binary_oper_node:
            gen_java_binary_op(node);
            break;
        case ast_constant_node:
            printf("    ldc %d\n", node->data.constant.value);
	    break;
        case ast_negation_node:
//            gen_java_code(node->data.negation.value);
            printf("    ineg\n");
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

            if (0 >= addr.java_variable_addr && addr.java_variable_addr <= 3)
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

