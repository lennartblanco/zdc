#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "java_trgt_int.h"
#include "java_trgt.h"
#include "java_local_slot.h"
#include "ast_basic_type.h"
#include "ast_return.h"
#include "ast_int_constant.h"
#include "ast_bool_constant.h"
#include "ast_static_array_type.h"
#include "ast_array_slice_ref.h"
#include "ast_array_literal.h"
#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

void
java_trgt_gen_code(IrCompileUnit *comp_unit,
                   FILE *out_stream,
                   const char *source_file)
{
    GList *symbols_list;
    GList *p;
    java_trgt_comp_params_t params;
    sym_table_t *global_sym_table;

    params.out = out_stream;
    params.class_name = get_class_name(source_file);
    label_gen_init(&params.label_gen);

    java_trgt_prelude(&params, source_file);

    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    symbols_list = sym_table_get_all_symbols(global_sym_table);
    for (p = symbols_list; p != NULL; p = g_list_next(p))
    {
        if (IR_IS_FUNCTION(p->data))
        {
            java_trgt_handle_function_def(&params,
                                          p->data);
        }
        else
        {
            /* unexpected symbol type */
            assert(false);
        }
    }
    g_list_free(symbols_list);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
get_class_name(const char *file_name)
{
    char *class_name;
    const char *last_slash;
    char *last_dot;

    class_name = g_malloc(strlen(file_name) - 2);

    last_slash = strrchr(file_name, '/');
    if (last_slash == NULL)
    {
        last_slash = file_name;
    }
    else
    {
        last_slash += 1;
    }

    strcpy(class_name, last_slash);

    last_dot = strrchr(class_name, '.');
    assert(last_dot != NULL);

    last_dot[0] = '\0';

    return class_name;
}

static void
java_trgt_prelude(java_trgt_comp_params_t *params,
                  const char *source_file)
{
    fprintf(params->out,
".source %s\n"
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
source_file,
params->class_name
    );
}

static void
java_trgt_const_int(java_trgt_comp_params_t *params, int value)
{
    /*
     * pick the most compact bytecode to push the requested value
     * on the stack
     */
    if (-1 <= value && value <= 5)
    {
        fprintf(params->out, "    iconst_%d\n", value);
    }
    else if (-128 <= value && value <= 127)
    {
        fprintf(params->out, "    bipush %d\n", value);
    }
    else if (-32768 <= value && value <= 32767)
    {
        fprintf(params->out, "    sipush %d\n", value);
    }
    else
    {
        fprintf(params->out, "    ldc %d\n", value);
    }
}

static void
java_trgt_handle_code_block(java_trgt_comp_params_t *params,
                            IrCodeBlock *code_block)
{
    GSList *stmts;
    GList *symbols_list;
    GList *l;
    sym_table_t *locals;


    locals = ir_code_block_get_symbols(code_block);
    /*
     * generate code to initialize all new variables in this code block
     */
    symbols_list = sym_table_get_all_symbols(locals);
    for (l = symbols_list; l != NULL; l = g_list_next(l))
    {
        IrVariable *var = l->data;
        AstDataType *var_type = ir_variable_get_data_type(var);
        AstExpression *var_init = ir_variable_get_initializer(var);
        int addr =
            java_local_slot_get_number(
                JAVA_LOCAL_SLOT(ir_variable_get_location(var)));

        /* construct default value for the type */
        if (var_init == NULL)
        {
            if (XDP_IS_AST_BASIC_TYPE(var_type))
            {
                basic_data_type_t bdt =
                    ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(var_type));
                switch (bdt)
                {
                    case int_type:
                        var_init = XDP_AST_EXPRESSION(ast_int_constant_new(0));
                        break;
                    case bool_type:
                        var_init =
                            XDP_AST_EXPRESSION(ast_bool_constant_new(false));
                        break;
                    default:
                        assert(false);
                }
            }
            else if (XDP_IS_AST_STATIC_ARRAY_TYPE(var_type))
            {
                /* 
                 * we don't need a initialization expression,
                 * as java VM will assign 0's to all array cells on creation
                 */
                /* nop */
            }
            else
            {
                /* unsupported complex type */
                assert(false);
            }
        }

        if (XDP_IS_AST_BASIC_TYPE(var_type))
        {
            java_trgt_handle_var_assigment(params,
                                           addr,
                                           var_init,
                                           locals);
        }
        else if (XDP_AST_STATIC_ARRAY_TYPE(var_type))
        {
            AstStaticArrayType *sarray = XDP_AST_STATIC_ARRAY_TYPE(var_type);

            /*
             * Genereate the code to create the array object and
             * save a reference to it on a local variable
             */

            /* put the array length on the stack */            
            java_trgt_const_int(params,
                                ast_static_array_type_get_length(sarray));


            /* create array and a reference to it in the local array */
            fprintf(params->out,
                    "    newarray int\n"
                    "    astore%s%d\n",
                    (0 <= addr && addr <= 3) ? "_" : " ", addr);

            if (var_init != NULL)
            {
                java_trgt_handle_array_slice_assigment(params,
                                                       addr,
                                                       NULL,
                                                       var_init,
                                                       locals);

            }
        }
        else
        {
            /* unexpected data type */
            assert(false);
        }
    }
    g_list_free(symbols_list);


    /*
     * generate code for the statments in this code block
     */
    stmts = ir_code_block_get_statments(code_block);
    for (;stmts != NULL; stmts = g_slist_next(stmts))
    {
        if (XDP_IS_AST_FUNCTION_CALL(stmts->data))
        {
            java_trgt_handle_func_call(params,
                                       XDP_AST_FUNCTION_CALL(stmts->data),
                                       locals, true);            
        }
        else if (XDP_IS_AST_NODE(stmts->data))
        {
            java_trgt_handle_node(params, stmts->data, locals);
        }
        else if (IR_IS_CODE_BLOCK(stmts->data))
        {
            java_trgt_handle_code_block(params, stmts->data);
        }
        else if (IR_IS_IF_ELSE(stmts->data))
        {
            java_trgt_handle_if_else(params, stmts->data, locals);
        }
        else if (IR_IS_WHILE(stmts->data))
        {
            java_trgt_handle_while(params, stmts->data, locals);
        }
        else if (IR_IS_FOREACH(stmts->data))
        {
            java_trgt_handle_foreach(params, stmts->data, locals);
        }
        else
        {
            /* unexpected statment type */
            assert(false);
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
    /* 
     * array_cell_ref check must pressed variable_ref, becouse
     * array_cell_ref is a subtype of variable_ref
     */ 
    else if (XDP_IS_AST_ARRAY_CELL_REF(exp))
    {
        java_trgt_handle_array_cell_ref(params,
                                        XDP_AST_ARRAY_CELL_REF(exp),
                                        sym_table);
    }
    else if (XDP_IS_AST_VARIABLE_REF(exp))
    {
        java_trgt_handle_var_value(params, 
                                   XDP_AST_VARIABLE_REF(exp),
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

        java_trgt_const_int(params, ast_int_constant_get_value(icons));
    }
    else if (XDP_IS_AST_BOOL_CONSTANT(exp))
    {
        bool val =
            ast_bool_constant_get_value(XDP_AST_BOOL_CONSTANT(exp));

        fprintf(params->out, "    iconst_%d\n",
                val ? 1 : 0);
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

    if (XDP_IS_AST_RETURN(node))
    {
        java_trgt_handle_return_statment(params,
                                         XDP_AST_RETURN(node),
                                         sym_table);
    }
    else if (XDP_IS_AST_ASSIGMENT(node))
    {
        java_trgt_handle_assigment(params, XDP_AST_ASSIGMENT(node), sym_table);
    }
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
    if (XDP_IS_AST_BASIC_TYPE(data_type))
    {
        /* convert basic data type to string */
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
    }
    else if (XDP_IS_AST_STATIC_ARRAY_TYPE(data_type))
    {
         /* convert static array type to string */
         AstStaticArrayType *sarray_type = XDP_AST_STATIC_ARRAY_TYPE(data_type);

         /** @todo this can be implemented more compact by adding '[' to
             converted basic type by reusing the code above */
         switch(ast_static_array_type_get_data_type(sarray_type))
         {
             case int_type:
                 return "[I";
             case bool_type:
                 return "[Z";
             default:
                 /* unexpected basic data type */
                 assert(false);
         }
    }
    else
    {
        /* unexpected data type */
        assert(false);
    }
    return NULL;
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

    IrFunction *func;
    GSList *arg;
    GSList *p;


    /* look-up called function in the symbol table */
    func = 
        IR_FUNCTION(sym_table_get_symbol(sym_table,
                                         ast_function_call_get_name(fun_call)));
    assert(func);


    /*
     * generate code to evaluate all function call parameters
     * and put them on the stack
     */
    arg = ast_function_call_get_arguments(fun_call);
    for (;arg != NULL; arg = g_slist_next(arg))
    {
        java_trgt_handle_expression(params, 
                                    XDP_AST_EXPRESSION(arg->data),
                                    sym_table);
    }

    /*
     * generate function call code
     */
    fprintf(params->out,
            "    invokestatic %s/%s(",
            params->class_name,
            ast_function_call_get_name(fun_call));

    /* generate function signature */
    p = ir_function_get_parameters(func);
    for (; p != NULL; p = g_slist_next(p))
    {
        AstVariableDeclaration *param =
            p->data;
        fprintf(params->out, 
                "%s", java_trgt_data_type_to_str(
                          ast_variable_declaration_get_data_type(param)));
    }

    /* add return type to function signature */
    AstDataType *ret_type = ir_function_get_return_type(func);
    fprintf(params->out, ")%s\n",
           java_trgt_data_type_to_str(ret_type));

    /*
     * add code to remove function result from the stack if
     * requested and if function returns anything
     */
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
java_trgt_handle_while(java_trgt_comp_params_t *params,
                       IrWhile *while_block,
                       sym_table_t *sym_table)
{
    assert(params);
    assert(while_block);
    assert(IR_IS_WHILE(while_block));
    assert(sym_table);

    char loop_start[MAX_JAVA_LABEL];
    char loop_end[MAX_JAVA_LABEL];

    label_gen_next(&(params->label_gen), loop_start);
    label_gen_next(&(params->label_gen), loop_end);

    fprintf(params->out,"; while start\n");
    fprintf(params->out, "%s:\n", loop_start);
    java_trgt_handle_expression(params,
                                ir_while_get_loop_condition(while_block),
                                sym_table);
    java_trgt_const_int(params, 1);
    fprintf(params->out, "    if_icmpne %s\n", loop_end);
    java_trgt_handle_code_block(params, ir_while_get_body(while_block));
    fprintf(params->out,
            "goto %s\n"
            "%s:\n", loop_start, loop_end);
    
}

static void
java_trgt_handle_if_else(java_trgt_comp_params_t *params, 
                         IrIfElse *if_else, 
                         sym_table_t *sym_table)
{
    assert(params);
    assert(if_else);
    assert(IR_IS_IF_ELSE(if_else));
    assert(sym_table);

    char end_label[MAX_JAVA_LABEL];
    char label[MAX_JAVA_LABEL];
    GSList *p;

    label_gen_next(&(params->label_gen), end_label);

    /*
     * generate code for all if-else clauses
     */
    p = ir_if_else_get_if_else_blocks(if_else);
    for (;p != NULL; p = g_slist_next(p))
    {
        IrIfBlock *if_block = IR_IF_BLOCK(p->data);

        /* generate code for if condition expression evaluation */
        java_trgt_handle_expression(params,
                                    ir_if_block_get_condition(if_block),
                                    sym_table);
        label_gen_next(&(params->label_gen), label);

        /* if condition is not evaluated to true, jump over the if body */
        fprintf(params->out,
                "    ifeq %s\n", label);

        /* generate if blocks's body */
        java_trgt_handle_code_block(params,
                                    ir_if_block_get_body(if_block));

        /* an if branch taken, jump over the rest of the if-else clauses */
        fprintf(params->out, "    goto %s\n%s:\n", end_label, label);
    }

    /* 
     * generate the else-clause if any 
     */
    IrCodeBlock *else_body = ir_if_else_get_else_body(if_else);
    if (else_body != NULL)
    {
        java_trgt_handle_code_block(params, else_body);
    }

    /* insert the end of if-else label */
    fprintf(params->out, "%s:\n", end_label);
}

static void
java_trgt_handle_foreach(java_trgt_comp_params_t *params,
                         IrForeach *foreach,
                         sym_table_t *sym_table)
{
    assert(params);
    assert(foreach);
    assert(IR_IS_FOREACH(foreach));
    assert(sym_table);

    char loop_start_label[MAX_JAVA_LABEL];
    char loop_end_label[MAX_JAVA_LABEL];
    IrVariable *var;
    IrVariable *aggregate_array;
    AstExpression *exp;
    /* address of foreach loop's index variable */
    int index_addr;
    /* address of foreach loop's value variable */
    int value_addr;
    /* address of hidden offset variable */
    int offset_addr;
    /* aggregate array's address */
    int aggregate_addr;
    
    AstArraySliceRef *aggregate_slice;

    fprintf(params->out, "; foreach block start\n");

    /* fetch the aggregate array slice we gonna loop over */
    aggregate_slice = ir_foreach_get_aggregate(foreach);

    /* get aggregate arrays local variable slot */
    aggregate_array =
        IR_VARIABLE(
            sym_table_get_symbol(sym_table,
                                 ast_array_slice_ref_get_name(aggregate_slice)));
    aggregate_addr =
        java_local_slot_get_number(
            JAVA_LOCAL_SLOT(ir_variable_get_location(aggregate_array)));

    /* get index variable's local variable slot */
    var = ir_foreach_get_index(foreach);
    index_addr =
        java_local_slot_get_number(
            JAVA_LOCAL_SLOT(ir_variable_get_location(var)));

    /* get value variable's local variable slot */
    var = ir_foreach_get_value(foreach);
    value_addr =
        java_local_slot_get_number(
            JAVA_LOCAL_SLOT(ir_variable_get_location(var)));

    /* a slot for offset variable is reserved after value's variables slot */
    offset_addr = value_addr + 1;

    /* generate loop start and end lables */
    label_gen_next(&(params->label_gen), loop_start_label);
    label_gen_next(&(params->label_gen), loop_end_label);

    fprintf(params->out,
            "; aggregate ref    %d\n"
            "; $index  address  %d\n"
            "; $value  address  %d\n"
            "; $offset address  %d\n"
            "; loop start label %s\n"
            "; loop end label   %s\n",
            aggregate_addr, index_addr, value_addr, offset_addr,
            loop_start_label, loop_end_label);
           

    /* initilize index variable */
    fprintf(params->out, 
            "; init index variable\n");
    java_trgt_const_int(params, 0);
    fprintf(params->out, "    istore%s%d\n", 
            (0 <= index_addr && index_addr <= 3) ? "_" : " ", index_addr);

    /* push aggregate end index on stack */
    fprintf(params->out, "; calculate aggregate end value\n");
    exp = ast_array_slice_ref_get_end(aggregate_slice);

    if (exp != NULL)
    {
        /* an explicit aggregate slice end expression specified */
        java_trgt_handle_expression(params,
                                    ast_array_slice_ref_get_end(aggregate_slice),
                                    sym_table);
    }
    else
    {
        /* 
         * no aggregate slice end expression provided, use
         * array length
         */
        AstStaticArrayType *type =
            XDP_AST_STATIC_ARRAY_TYPE(ir_variable_get_data_type(aggregate_array));
        java_trgt_const_int(params, ast_static_array_type_get_length(type));
    }

    /* initilize offset variable */
    fprintf(params->out, "; calculate aggregate start value\n");
    exp = ast_array_slice_ref_get_start(aggregate_slice);
    if (exp != NULL)
    {
        /* an explicit aggregate slice start expression specified */
        java_trgt_handle_expression(params, exp, sym_table);
    }
    else
    {
        /* no start expression means start from 0 */
        java_trgt_const_int(params, 0);
    }
    fprintf(params->out,
            "    dup\n"
            "    istore%s%d\n", 
            (0 <= offset_addr && offset_addr <= 3) ? "_" : " ", offset_addr);

    fprintf(params->out,
            "    isub\n"
            "; loop start\n"
            "%s:\n"
            "    dup\n"
            "    iload%s%d\n"
            "    if_icmple %s\n"
            "    aload%s%d\n"
            "; push value of aggregate[$offset + $index] on stack\n"
            "    iload%s%d   ; push $offset on stack\n"
            "    iload%s%d   ; push $index on stack\n"
            "    iadd\n"
            "    iaload\n"
            "    istore%s%d  ; store aggregate[$offset + $index] in $value\n"
            "; loop body\n",
            loop_start_label,
            (0 <= index_addr && index_addr <= 3) ? "_" : " ", index_addr,
            loop_end_label,
            (0 <= aggregate_addr && aggregate_addr <= 3) ? "_" : " ", aggregate_addr,
            (0 <= offset_addr && offset_addr <= 3) ? "_" : " ", offset_addr,
            (0 <= index_addr && index_addr <= 3) ? "_" : " ", index_addr,
            (0 <= value_addr && value_addr <= 3) ? "_" : " ", value_addr);

    /* generate loop body code */
    java_trgt_handle_code_block(params,
                                ir_foreach_get_body(foreach));

    fprintf(params->out,
            "; $index++\n"
            "    iinc %d 1\n"
            "    goto %s\n"
            "%s:\n"
            "    pop\n",
            index_addr, 
            loop_start_label,
            loop_end_label);

    fprintf(params->out, "; foreach block end\n");
}


static void
java_trgt_comp_ops_body(java_trgt_comp_params_t *params,
                        ast_binary_op_type_t type)
{
    char trueLabel[MAX_JAVA_LABEL];
    char endLabel[MAX_JAVA_LABEL];
    const char* operationCond;

    label_gen_next(&(params->label_gen), trueLabel);
    label_gen_next(&(params->label_gen), endLabel);

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
java_trgt_handle_andor_op(java_trgt_comp_params_t *params,
                           AstBinaryOperation *operation,
                           sym_table_t *sym_table)
{
    char abort_label[MAX_JAVA_LABEL];
    char end_label[MAX_JAVA_LABEL];
    char *eval_op;
    int abort_val;
    int fall_throught_val;

    /* generate the abort and end of operation labels */
    label_gen_next(&(params->label_gen), abort_label);
    label_gen_next(&(params->label_gen), end_label);

    ast_binary_op_type_t op_type =
        ast_binary_operation_get_operation(operation);

    /* pick evaluation operation and this operations results */
    switch (op_type)
    {
        case ast_and_op:
            eval_op = "eq";
            abort_val = 1;
            fall_throught_val = 0;
            break;
        case ast_or_op:
            eval_op = "ne";
            abort_val = 0;
            fall_throught_val = 1;
            break;
        default:
            /* unexpedted operation type */
            assert(false);
    }

    /* generate code for left operand evaluation */
    java_trgt_handle_expression(params,
                                ast_binary_operation_get_left(operation),
                                sym_table);

    /* add code for aborting and/or expression evaluation */
    fprintf(params->out,
            "    if%s %s\n", eval_op, abort_label);

    /* generate code for right operand evaluation */
    java_trgt_handle_expression(params,
                                ast_binary_operation_get_right(operation),
                                sym_table);
    fprintf(params->out,
            "    if%s %s\n"
            "    iconst_%d\n"
            "    goto %s\n"
            "%s:\n"
            "    iconst_%d\n"
            "%s:\n",
            eval_op, abort_label, abort_val, end_label, abort_label,
            fall_throught_val, end_label);
}

static void
java_trgt_handle_binary_op(java_trgt_comp_params_t *params,
                           AstBinaryOperation *operation,
                           sym_table_t *sym_table)
{
    assert(params);
    assert(operation);
    assert(XDP_IS_AST_BINARY_OPERATION(operation));

    ast_binary_op_type_t operationType =
        ast_binary_operation_get_operation(operation);


    if (operationType == ast_or_op ||
        operationType == ast_and_op)
    {
        java_trgt_handle_andor_op(params, operation, sym_table);
        return;
    }

    java_trgt_handle_expression(params,
                                ast_binary_operation_get_left(operation),
                                sym_table);
    java_trgt_handle_expression(params,
                                ast_binary_operation_get_right(operation),
                                sym_table);

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
            g_error("%s:%d unexpected binary operation %d\n", 
                    __FILE__, __LINE__, operationType);
    }
}

static void
java_trgt_handle_var_assigment(java_trgt_comp_params_t *params,
                               guint var_num,
                               AstExpression *exp,
                               sym_table_t *sym_table)
{
    java_trgt_handle_expression(params, exp, sym_table);

    fprintf(params->out, "    istore%s%d\n", 
            (0 <= var_num && var_num <= 3) ? "_" : " ", var_num);
}

static void
java_trgt_handle_array_slice_assigment(java_trgt_comp_params_t *params,
                                       guint var_num,
                                       AstExpression *start_exp,
                                       AstExpression *val,
                                       sym_table_t *sym_table)
{
    /* only constant expression supported at this time */
    assert(XDP_IS_AST_ARRAY_LITERAL(val));

    gint32 array_idx;

    /* 
     * only assigment to array slices with static dimensions supported
     * at this time
     */
    assert(start_exp == NULL || XDP_IS_AST_INT_CONSTANT(start_exp));

    if (start_exp == NULL)
    {
        /* handle 'slice[]' case, where start is implicitly 0 */
        array_idx = 0;
    }
    else
    {
        array_idx = 
            ast_int_constant_get_value(XDP_AST_INT_CONSTANT(start_exp));
    }

    /* 
     * here we assume that the length of the provided array constant
     * matches the dimensions of the specified array slice
     * this should be checked for static cases during semantic checks
     * BEWARE: the semantic checks for array assigments are not implemented yet
     */    
    GSList *p = ast_array_literal_get_values(XDP_AST_ARRAY_LITERAL(val));

    for (;p != NULL; p = g_slist_next(p), ++array_idx)
    {
        AstExpression *val = XDP_AST_EXPRESSION(p->data);

        fprintf(params->out, "    aload%s%d\n",
                (0 <= var_num && var_num <= 3) ? "_" : " ", var_num);
        java_trgt_const_int(params, array_idx);
        java_trgt_handle_expression(params, val, sym_table);
        fprintf(params->out, "    iastore\n");
    }
}

static void
java_trgt_handle_array_assigment(java_trgt_comp_params_t *params,
                                 guint var_num,
                                 AstExpression *index,
                                 AstExpression *exp,
                                 sym_table_t *sym_table)
{
    /* push array reference on stack */
    fprintf(params->out,
            "    aload%s%d\n",
            (0 <= var_num && var_num <= 3) ? "_" : " ", var_num);

    /* push index expression on stack */
    java_trgt_handle_expression(params, index, sym_table);

    /* push the value to assign on stack */
    java_trgt_handle_expression(params, exp, sym_table);

    /* store the value in the array */
    fprintf(params->out, "    iastore\n");  
}

static void
java_trgt_handle_assigment(java_trgt_comp_params_t *params,
                           AstAssigment *node,
                           sym_table_t *sym_table)
{
    IrVariable *lvalue_var;
    int lvalue_addr;
    AstVariableRef *lvalue;

    /* get lvalue node */
    lvalue = ast_assigment_get_target(node);

    /* look-up lvalue in the symbol table */
    lvalue_var = 
        IR_VARIABLE(sym_table_get_symbol(sym_table,
                                         ast_variable_ref_get_name(lvalue)));
    assert(lvalue_var);

    /* get lvalue's slot number in local variables array */
    lvalue_addr =
        java_local_slot_get_number(
            JAVA_LOCAL_SLOT(ir_variable_get_location(lvalue_var)));

    if (XDP_IS_AST_ARRAY_CELL_REF(lvalue))
    {
        AstArrayCellRef *acell = XDP_AST_ARRAY_CELL_REF(lvalue);

        java_trgt_handle_array_assigment(params,
                                         lvalue_addr,
                                         ast_array_cell_ref_get_index(acell),
                                         ast_assigment_get_value(node),
                                         sym_table);
    }
    else if (XDP_IS_AST_ARRAY_SLICE_REF(lvalue))
    {
        AstArraySliceRef *sref =
            XDP_AST_ARRAY_SLICE_REF(lvalue);

        java_trgt_handle_array_slice_assigment(params,
                                               lvalue_addr,
                                               ast_array_slice_ref_get_start(sref),
                                               ast_assigment_get_value(node),
                                               sym_table);
    }
    else if (XDP_IS_AST_VARIABLE_REF(lvalue))
    {
        java_trgt_handle_var_assigment(params,
                                       lvalue_addr,
                                       ast_assigment_get_value(node),
                                       sym_table);
    }
    else
    {
        /* unexpected lvalue type */
        assert(false);
    }
}

static void
java_trgt_handle_array_cell_ref(java_trgt_comp_params_t *params,
                                AstArrayCellRef *acell,
                                sym_table_t *sym_table)
{
    assert(params);
    assert(acell);
    assert(XDP_IS_AST_ARRAY_CELL_REF(acell));
    assert(sym_table);

    IrVariable *var;
    int addr;
    char *name;

    /* look-up variable in the symbol table */
    name = ast_variable_ref_get_name(XDP_AST_VARIABLE_REF(acell));
    var = IR_VARIABLE(sym_table_get_symbol(sym_table, name));
    assert(var != NULL);

    /* get variables slot number in local variables array */
    addr =
        java_local_slot_get_number(
            JAVA_LOCAL_SLOT(ir_variable_get_location(var)));


    /* generate code to put array reference on the stack */
    fprintf(params->out, 
            "    aload%s%d\n",
            (0 <= addr && addr <= 3) ? "_" : " ", 
            addr);

    /* generate code to put array index in the stack */
    java_trgt_handle_expression(params,
                                ast_array_cell_ref_get_index(acell),
                                sym_table);

    /* generate code to fetch the value from the array to the stack */
    fprintf(params->out, "    iaload\n");
}

static void
java_trgt_handle_var_value(java_trgt_comp_params_t *params,
                           AstVariableRef *var_ref,
                           sym_table_t *sym_table)
{
    IrVariable *var;
    int addr;
    char *name;
    AstDataType *type;

    /* look-up variable in the symbol table */
    name = ast_variable_ref_get_name(XDP_AST_VARIABLE_REF(var_ref));
    var = IR_VARIABLE(sym_table_get_symbol(sym_table, name));
    assert(var != NULL);

    /* get variables slot number in local variables array */
    addr =
        java_local_slot_get_number(
            JAVA_LOCAL_SLOT(ir_variable_get_location(var)));

    /* get variable type */
    type = ir_variable_get_data_type(var);


    if (XDP_IS_AST_BASIC_TYPE(type))
    {
        AstBasicType *btype = XDP_AST_BASIC_TYPE(type);
        /* only integer and boolean basic type supported at the moment */
        assert(ast_basic_type_get_data_type(btype) == int_type ||
               ast_basic_type_get_data_type(btype) == bool_type);

        /* load int/bool value to the stack from the variable */
        fprintf(params->out, "    iload%s%d\n", 
                (0 <= addr && addr <= 3) ? "_" : " ", addr);
    }
    else if (XDP_IS_AST_STATIC_ARRAY_TYPE(type))
    {
        /* only integer and boolean static arrays supported at the moment */
        AstStaticArrayType *sarray_type = XDP_AST_STATIC_ARRAY_TYPE(type);
        assert(ast_static_array_type_get_data_type(sarray_type) == int_type ||
               ast_static_array_type_get_data_type(sarray_type) == bool_type);

        /* load array refernce to the stack from the variable */
        fprintf(params->out, "    aload%s%d\n", 
                (0 <= addr && addr <= 3) ? "_" : " ", addr);
    }
    else
    {
        /* unexpected data type */
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
                              IrFunction *func)
{
    GSList *p;
    guint param_num = 0;
    guint local_var_num = 0;
    guint i;
    sym_table_t *local_vars;
    
    /*
     * generate function header
     */
    fprintf(params->out,
            ".method public static %s(", ir_function_get_name(func));

    /* generate function parameters types and count parameters */
    p = ir_function_get_parameters(func);
    for (; p != NULL; p = g_slist_next(p))
    {
        param_num += 1;
        fprintf(params->out, "%s", 
                java_trgt_data_type_to_str(
                    ast_variable_declaration_get_data_type(p->data)));
    }

    /* generate java assembly function return type declaration */
    fprintf(params->out, ")%s\n",
           java_trgt_data_type_to_str(
               ir_function_get_return_type(func)));

    /* assign numbers to local variables in function body */
    local_var_num = java_trgt_code_block_assign_addrs(param_num, 
                                                   ir_function_get_body(func));

    /* 
     * assign numbers to function parameter variables
     */
    p = ir_function_get_parameters(func);
    local_vars = ir_function_get_parameter_symbols(func);
    for (i = 0; p != NULL; i++, p = g_slist_next(p))
    {        
        AstVariableDeclaration *var_decl = p->data;

        /* convert ast variable declaration to IR variable object */
        IrVariable *variable =
            IR_VARIABLE(sym_table_get_symbol(local_vars,
                                 ast_variable_declaration_get_name(var_decl)));

        /* assign variable number */
        ir_variable_set_location(variable, G_OBJECT(java_local_slot_new(i)));
    }

    fprintf(params->out, "    .limit locals %d\n", local_var_num + 1);
    fprintf(params->out, "    .limit stack 32\n");

    java_trgt_handle_code_block(params, 
                                ir_function_get_body(func));
    fprintf(params->out, ".end method\n");
}

static int
java_trgt_if_else_assign_addrs(int first_num,
                               IrIfElse *if_else)
{
    GSList *i;
    IrCodeBlock *else_body;
    int last_num = first_num;

    /* assign numbers to bodies of all if and if-else clauses */
    i = ir_if_else_get_if_else_blocks(if_else);
    for (; i != NULL; i = g_slist_next(i))
    {
        int num = 0;
        IrIfBlock *if_block = i->data;
        num = java_trgt_code_block_assign_addrs(first_num,
                                                ir_if_block_get_body(if_block));
        if (num > last_num)
        {
            last_num = num;
        }
    }

    /* assign numbers to else clause's body */
    else_body = ir_if_else_get_else_body(if_else);
    if (else_body != NULL)
    {
        int num = 0;
        num = java_trgt_code_block_assign_addrs(first_num, else_body);
        if (num > last_num)
        {
            last_num = num;
        }
    }

    return last_num;
}

static int
java_trgt_foreach_assign_addrs(int first_num,
                               IrForeach *foreach)
{
    assert(foreach);
    IrVariable *var;
    int num = first_num;

    /*
     * assign local variable number to index variable
     */
    var = ir_foreach_get_index(foreach);
    if (var == NULL)
    {
        var = ir_variable_new(XDP_AST_DATA_TYPE(ast_basic_type_new(int_type)),
                              "foreach_hidden_index",
                              NULL);
        ir_foreach_set_index(foreach, var);
    }
    ir_variable_set_location(var, G_OBJECT(java_local_slot_new(num)));

    /*
     * assign local variable number to value variable
     */
    var = ir_foreach_get_value(foreach);
    num += 1;
    ir_variable_set_location(var, G_OBJECT(java_local_slot_new(num)));

    /*
     * reserve one local variable slot for 'offset' variable
     * that will be used by generated java bytecode
     */
    num += 1;
    
    return num;
}


static int
java_trgt_code_block_assign_addrs(int first_num,
                                  IrCodeBlock *code_block)
{
    sym_table_t *symbols;
    GList *symbols_list;
    GList *i;
    GSList *j;
    int num = first_num;
    int last_num;

    /*
     * assign number to this code block's local variables
     */
    symbols = ir_code_block_get_symbols(code_block);

    symbols_list = sym_table_get_all_symbols(symbols);
    for (i = symbols_list; i != NULL; i = g_list_next(i), num += 1)
    {
        IrVariable *symb = i->data;
        ir_variable_set_location(symb, G_OBJECT(java_local_slot_new(num)));
    }
    g_list_free(symbols_list);
    last_num = num - 1;

    /*
     * assign numbers to children code block's variables
     */
    j = ir_code_block_get_statments(code_block);
    for (; j != NULL; j = g_slist_next(j))
    {
        int vars = 0;
        if (IR_IS_CODE_BLOCK(j->data))
        {
            vars = java_trgt_code_block_assign_addrs(num, j->data);
            /* 
             * keep track if highest local number slot assigned 
             * in our sub-blocks 
             */
        }
        else if (IR_IS_IF_ELSE(j->data))
        {
            vars = java_trgt_if_else_assign_addrs(num, j->data);

        }
        else if (IR_IS_FOREACH(j->data))
        {
            vars = java_trgt_foreach_assign_addrs(num, j->data);
        }
        if (vars > last_num)
        {
            last_num = vars;
        }
    }

    return last_num;
}
