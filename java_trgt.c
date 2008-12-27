#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "ast.h"
#include "sym_table.h"
#include "java_trgt.h"
#include "java_trgt_int.h"
#include "ast_basic_type.h"
#include "ast_return.h"
#include "ast_scalar_variable_ref.h"
#include "ast_int_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

void
java_trgt_code(ir_compile_unit_t *comp_unit,
              FILE *out_stream, 
              const char *klass_name)
{
    GList *p;
    java_trgt_comp_params_t params; 

    params.out = out_stream;
    params.class_name = klass_name;
    strcpy(params.next_label, "A");
    java_trgt_prelude(&params);

    p = ir_compile_unit_get_functions(comp_unit);
    for (; p != NULL; p = g_list_next(p))
    {
        java_trgt_handle_function_def(&params, 
                                      ir_symbol_get_function(p->data));
    }
    g_list_free(p);

    java_trgt_epilog(&params);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
java_trgt_prelude(java_trgt_comp_params_t *params)
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
java_trgt_epilog(java_trgt_comp_params_t *params)
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
java_trgt_handle_code_block(java_trgt_comp_params_t *params,
                            AstCodeBlock *code_block,
                            sym_table_t *sym_table)
{
    GSList *stmts;

    stmts = ast_code_block_get_statments(code_block);

    for (;stmts != NULL; stmts = g_slist_next(stmts))
    {
        AstNode *statment = XDP_AST_NODE(stmts->data);

        if (XDP_IS_AST_FUNCTION_CALL(statment))
        {
            java_trgt_handle_func_call(params,
                                       XDP_AST_FUNCTION_CALL(statment),
                                       sym_table, true);            
        }
        else
        {
            java_trgt_handle_node(params, statment, sym_table);
        }
    }
}

static void
java_trgt_handle_expression(java_trgt_comp_params_t *params,
                            AstExpression *exp, 
                            sym_table_t *sym_table)
{
    assert(params);
    assert(exp);
    assert(XDP_IS_AST_EXPRESSION(exp));

    if (XDP_IS_AST_BINARY_OPERATION(exp))
    {
        java_trgt_handle_binary_op(params,
                                   XDP_AST_BINARY_OPERATION(exp),
                                   sym_table);
    }
    else if (XDP_IS_AST_UNARY_OPERATION(exp))
    {
        java_trgt_handle_unary_op(params,
                                  XDP_AST_UNARY_OPERATION(exp),
                                  sym_table);
    }
    else if (XDP_IS_AST_SCALAR_VARIABLE_REF(exp))
    {
        java_trgt_handle_scalar_var_value(params, 
                                          XDP_AST_SCALAR_VARIABLE_REF(exp),
                                          sym_table);
    }
    else if (XDP_IS_AST_FUNCTION_CALL(exp))
    {
        java_trgt_handle_func_call(params,
                                   XDP_AST_FUNCTION_CALL(exp),
                                   sym_table,
                                   false);
    }
    else if (XDP_IS_AST_INT_CONSTANT(exp))
    {
        AstIntConstant *icons = XDP_AST_INT_CONSTANT(exp);

        fprintf(params->out,
                "    ldc %d\n", ast_int_constant_get_value(icons));
    }
    else
    {
        g_error("%s:%d unexpected AstExpression type '%s'\n", 
                __FILE__, __LINE__,
                g_type_name(G_TYPE_FROM_INSTANCE(exp)));
        assert(false);
    }
}

static void
java_trgt_handle_node(java_trgt_comp_params_t *params,
                      AstNode *node, sym_table_t *sym_table)
{

    if (XDP_IS_AST_CODE_BLOCK(node))
    {
        java_trgt_handle_code_block(params,
                                    XDP_AST_CODE_BLOCK(node),
                                    sym_table);
    }
    else if (XDP_IS_AST_RETURN(node))
    {
        java_trgt_handle_return_statment(params,
                                         XDP_AST_RETURN(node),
                                         sym_table);
    }
    else if (XDP_IS_AST_ASSIGMENT(node))
    {
        java_trgt_handle_assigment(params, XDP_AST_ASSIGMENT(node), sym_table);
    }

//        case ast_if_else_block_node:
//            java_trgt_handle_if_else_block(params, node, sym_table);
//            break;
//        default:
//            printf("Unexpected node->type %s\n", ast_node_to_str(node->type));
//            assert(false);
//            break;
    else
    {
        g_error("%s:%d unexpected AstNode type '%s'\n", 
                __FILE__, __LINE__,
                g_type_name(G_TYPE_FROM_INSTANCE(node)));
        assert(false);
    }
}

static char *
java_trgt_data_type_to_str(AstDataType *data_type)
{
    /* only basic data types supperted at the moment */
    assert(XDP_IS_AST_BASIC_TYPE(data_type));
    AstBasicType *basic_type = XDP_AST_BASIC_TYPE(data_type);

    switch(ast_basic_type_get_data_type(basic_type))
    {
        case void_type:
            return "V";
        case int_type:
            return "I";
        case bool_type:
            return "Z";
        default:
            /* unexpected basic data type */
            assert(false);
    }
    return NULL;
}

STATIC void
java_trgt_get_next_label(java_trgt_comp_params_t *params,
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
java_trgt_handle_func_call(java_trgt_comp_params_t *params,
                           AstFunctionCall *fun_call,
                           sym_table_t *sym_table,
                           bool pop_return_value)
{
    assert(params);
    assert(fun_call);
    assert(XDP_IS_AST_FUNCTION_CALL(fun_call));
    assert(sym_table);

    GSList *arg;
    ir_symbol_t *symb;
    ir_function_def_t *func;
    int res;
    GSList *p;


    res = sym_table_get_symbol(sym_table,
                               ast_function_call_get_name(fun_call),
                               &symb);
    if (res == -1)
    {
        printf("undefined reference to function '%s'\n",
               ast_function_call_get_name(fun_call));
        return;
    }
    func = ir_symbol_get_function(symb);

    arg = ast_function_call_get_arguments(fun_call);
    while (arg != NULL)
    {
        java_trgt_handle_expression(params, 
                                    XDP_AST_EXPRESSION(arg->data),
                                    sym_table);
        arg = arg->next;
    }

    fprintf(params->out,
            "    invokestatic %s/%s(",
            params->class_name,
            ast_function_call_get_name(fun_call));

    p = ir_function_def_get_parameters(func);
    for (; p != NULL; p = g_slist_next(p))
    {
        ir_variable_def_t *v = p->data;
        fprintf(params->out, 
                "%s", java_trgt_data_type_to_str(ir_variable_def_get_type(v)));
    }

    AstDataType *ret_type = ir_function_def_get_return_type(func);
    fprintf(params->out, ")%s\n", 
           java_trgt_data_type_to_str(ret_type));

    if (pop_return_value && XDP_IS_AST_BASIC_TYPE(ret_type))
    {
        basic_data_type_t t =
            ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(ret_type));
        if (t != void_type)
        {
            fprintf(params->out, "    pop\n");
        }
    }
}


static void
java_trgt_handle_if_else_block(java_trgt_comp_params_t *params,
                               ast_node_t *node,
                               sym_table_t *sym_table)
{
      /* needs to be update to the new AST implementation */
      assert(false);
//    assert(params);
//    assert(node);
//    assert(sym_table);
//    assert(node->type == ast_if_else_block_node);

//    char endLabel[MAX_JAVA_LABEL];
//    ast_node_t *cond;
//    const char* operationCond;

//    if (node->data.if_else_block.else_block != NULL)
//    {
//        printf("else-clouses not impelemented\n");
//        assert(false);
//    }

//    cond = node->data.if_else_block.condition;
//    assert(cond->type == ast_binary_oper_node);

//    java_trgt_handle_node(params, cond->data.binary_op.left, sym_table);
//    java_trgt_handle_node(params, cond->data.binary_op.right, sym_table);

//    /* figure out the negation of the if condition */
//    switch (cond->data.binary_op.oper_type)
//    {
//        case ast_less_op:
//            /*  !(a<b) == (a >= b) */
//            operationCond = "ge";
//            break;
//        case ast_greater_op:
//            /* !(a>b) == (a <= b) */
//            operationCond = "le";
//            break;
//        case ast_less_or_eq_op:
//            /* !(a<=b) == (a > b) */
//            operationCond = "gt";
//            break;
//        case ast_greater_or_eq_op:
//            /* !(a>=b) == (a < b) */
//            operationCond = "lt";
//            break;
//        case ast_equal_op:
//            /* !(a == b) == (a != b) */
//            operationCond = "ne";
//            break;
//        case ast_not_equal_op:
//            /* !(a != b) == (a == b) */
//            operationCond = "eq";
//            break;
//        default:
//            /* unexpected comparison operation */
//            assert(false);
//    }
//    java_trgt_get_next_label(params, endLabel);

//    fprintf(params->out,
//            "    if_icmp%s %s\n", operationCond, endLabel);

//    java_trgt_handle_node(params, node->data.if_else_block.if_block, sym_table);
//    fprintf(params->out, "%s:\n", endLabel);
}

static void
java_trgt_comp_ops_body(java_trgt_comp_params_t *params,
                        ast_binary_op_type_t type)
{
    char trueLabel[MAX_JAVA_LABEL];
    char endLabel[MAX_JAVA_LABEL];
    const char* operationCond;

    java_trgt_get_next_label(params, trueLabel);
    java_trgt_get_next_label(params, endLabel);

    switch (type)
    {
        case ast_less_op:
            operationCond = "lt";
            break;
        case ast_greater_op:
            operationCond = "gt";
            break;
        case ast_less_or_eq_op:
            operationCond = "le";
            break;
        case ast_greater_or_eq_op:
            operationCond = "ge";
            break;
        case ast_equal_op:
            operationCond = "eq";
            break;
        case ast_not_equal_op:
            operationCond = "ne";
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
java_trgt_handle_unary_op(java_trgt_comp_params_t *params,
                          AstUnaryOperation *operation,
                          sym_table_t *sym_table)
{
    assert(params);
    assert(operation);
    assert(XDP_IS_AST_UNARY_OPERATION(operation));

    java_trgt_handle_expression(params,
                                ast_unary_operation_get_operand(operation),
                                sym_table);

    switch (ast_unary_operation_get_operation(operation))
    {
        case ast_arithm_neg_op:
            fprintf(params->out, "    ineg\n");
            break;
        case ast_bool_neg_op:
            g_error("Boolean negation operation not implemented in java target");
            break;
        default:
            g_error("%s:%d unexpected Unary operation %d\n", 
                    __FILE__, __LINE__, 
                    ast_unary_operation_get_operation(operation));
    }
}
static void
java_trgt_handle_binary_op(java_trgt_comp_params_t *params,
                           AstBinaryOperation *operation,
                           sym_table_t *sym_table)
{
    assert(params);
    assert(operation);
    assert(XDP_IS_AST_BINARY_OPERATION(operation));

    java_trgt_handle_expression(params,
                                ast_binary_operation_get_left(operation),
                                sym_table);
    java_trgt_handle_expression(params,
                                ast_binary_operation_get_right(operation),
                                sym_table);

    ast_binary_op_type_t operationType =
        ast_binary_operation_get_operation(operation);

    switch (operationType)
    {
        case ast_less_op:
        case ast_greater_op:
        case ast_less_or_eq_op:
        case ast_greater_or_eq_op:
        case ast_equal_op:
        case ast_not_equal_op:
            java_trgt_comp_ops_body(params, operationType);
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
java_trgt_handle_assigment(java_trgt_comp_params_t *params,
                           AstAssigment *node,
                           sym_table_t *sym_table)
{

    ir_symbol_t *symb;
    int res;
    ir_symbol_address_t addr;
    AstScalarVariableRef *assign_target =
        XDP_AST_SCALAR_VARIABLE_REF(ast_assigment_get_target(node));
    /* only assigments to scalar variable supported at this time */
    assert(XDP_IS_AST_SCALAR_VARIABLE_REF(assign_target));

    res = sym_table_get_symbol(sym_table,
                               ast_scalar_variable_get_name(assign_target),
                               &symb);
    if (res == -1)
    {
        printf("variable '%s' not defined\n", 
               ast_scalar_variable_get_name(assign_target));
        return;
    }

    java_trgt_handle_expression(params, 
                                ast_assigment_get_value(node), 
                                sym_table);

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
java_trgt_handle_scalar_var_value(java_trgt_comp_params_t *params,
                                  AstScalarVariableRef *var_ref,
                                  sym_table_t *sym_table)
{
    ir_symbol_t *symb;
    int res;

    res = sym_table_get_symbol(sym_table,
                               ast_scalar_variable_get_name(var_ref),
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
            printf("variable '%s' not defined\n",
                   ast_scalar_variable_get_name(var_ref));
            break;
        default: /* unexpected return value */
            assert(false);
    }
}

static void
java_trgt_handle_return_statment(java_trgt_comp_params_t *params, 
                                 AstReturn *ret, 
                                 sym_table_t *sym_table)
{
    assert(ret);
    assert(XDP_IS_AST_RETURN(ret));
    assert(sym_table);

    AstExpression *ret_val;

    ret_val = ast_return_get_return_value(ret);

    if (ret_val == NULL)
    {
        /* assume void return for now */
        fprintf(params->out, "    return\n");
    }
    else
    {
        /* assume int return for now */
        java_trgt_handle_expression(params, ret_val, sym_table);
        fprintf(params->out, "    ireturn\n");
    }
}

static void
java_trgt_handle_function_def(java_trgt_comp_params_t *params,
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
                java_trgt_data_type_to_str(ir_variable_def_get_type(var)));
    }

    fprintf(params->out, ")%s\n", 
           java_trgt_data_type_to_str(ir_function_def_get_return_type(func)));

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
    java_trgt_handle_code_block(params, 
                               ir_function_def_get_body(func),
                               local_vars);
    fprintf(params->out, ".end method\n");
}

