#include <stdio.h>

#include "ast.h"
#include "sym_table.h"
#include "gen_java.h"

extern sym_table_t *global_sym_table;

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
".method public static main([Ljava/lang/String;)V\n"
"   .limit stack 16\n"
"   .limit locals 16\n"
    );
}

void
gen_java_epilog()
{
    printf(
"   return\n"
".end method\n"
     );
}

void
gen_java_binary_op(ast_node_t *node)
{
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
handle_var_value(ast_node_t *node)
{
    int var_num;
#ifdef UNCOMMENT    
    switch (find_element(node->data.var_val.name, &var_num))
    {
        case 0:
            if (0 >= var_num && var_num <= 3)
            {
                printf("    iload_%d\n", var_num);
            }
            else
            {
                printf("    iload %d\n", var_num);
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
    func_args_list_t *arg;

    printf("; call %s\n", node->data.function_call.name);
    arg = node->data.function_call.args;
    printf("; --- eval arguments\n");
    while (arg != NULL)
    {
        gen_java_code(arg->value);
        arg = arg->next;
    }
    printf("    invokestatic foo/%s(I)V\n", node->data.function_call.name);
}

void
gen_java_handle_node(ast_node_t *node)
{
    switch (node->type)
    {
        case ast_function_call_node:
            gen_func_call(node);
            break;
        case ast_var_value_node:
            handle_var_value(node);
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
            gen_java_code(node->data.negation.value);
            printf("    ineg\n");
            break;
    }
}

void
handle_glob_symbol(ast_node_t *node)
{
    printf("wee %p\n", node);
}

void
gen_java_code()
{
    sym_table_foreach_sym(global_sym_table, handle_glob_symbol);
}
