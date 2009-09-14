#include <stdbool.h>
#include <string.h>

#include "label_gen.h"
#include "types.h"
#include "x86.h"
#include "x86_cast.h"
#include "x86_if_else.h"
#include "x86_frame_offset.h"
#include "x86_reg_location.h"
#include "ast_basic_type.h"
#include "ast_static_array_type.h"
#include "ast_variable_declaration.h"
#include "ir_array_literal.h"
#include "ir_array_cell_ref.h"
#include "ir_cast.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_int_constant.h"
#include "ir_bool_constant.h"
#include "ir_variable.h"
#include "ir_return.h"
#include "ir_function_call.h"
#include "ir_if_else.h"
#include "ir_assigment.h"
#include "ir_while.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_prelude(FILE *out, const char *source_file);

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunction *func);

static void
x86_compile_while(x86_comp_params_t *params,
                  IrWhile *while_statment,
                  sym_table_t *sym_table);

static void
x86_compile_binary_op(x86_comp_params_t *params,
                      IrBinaryOperation *op,
                      sym_table_t *sym_table);

static void
x86_compile_unary_op(x86_comp_params_t *params,
                     IrUnaryOperation *op,
                     sym_table_t *sym_table);

static void
x86_compile_func_call(x86_comp_params_t *params,
                      IrFunctionCall *func_call,
                      sym_table_t *sym_table,
                      bool retain_return_value);

static void
x86_gen_variable_assigment(x86_comp_params_t *params,
                           IrVariable *variable,
                           IrExpression *expression,
                           sym_table_t *sym_table);
static void
x86_gen_array_literal_assigment(x86_comp_params_t *params, 
                                IrVariable *variable,
                                IrArrayLiteral *array_literal,
                                sym_table_t *sym_table);

static void
x86_compile_array_cell_ref(x86_comp_params_t *params,
                           IrArrayCellRef *array_cell,
                           sym_table_t *sym_table);

static int
x86_get_variable_storage_size(IrVariable *variable);

static void
x86_compile_iarithm_op(x86_comp_params_t *params,
                       IrBinaryOperation *op,
                       sym_table_t *sym_table);

static void
x86_compile_icomp_op(x86_comp_params_t *params,
                     IrBinaryOperation *op,
                     sym_table_t *sym_table);

static void
x86_compile_conditional_op(x86_comp_params_t *params,
                           IrBinaryOperation *op,
                           sym_table_t *sym_table);

static void
x86_compile_variable_ref(x86_comp_params_t *params,
                         IrVariable *var);

void
x86_compile_expression(x86_comp_params_t *params,
                       IrExpression *expression,
                       sym_table_t *sym_table);

/**
 * Generate code to pop 32-bit value off the stack and
 * move it to frame offset location (e.g. local variable).
 *
 * @param params       compilation pass parameters handle
 * @param frame_offset function frame offset (offset relative to eps register)
 *                     where to move pop-ed data
 * @param storage_size number of bytes to store, if less then 4 bytes (32-bit),
 *                     more significant bytes will be discarded
 */
static void
x86_gen_store_value(x86_comp_params_t *params,
                    int frame_offset,
                    int storage_size);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream,
             const char *source_file)
{
    x86_comp_params_t params;
    sym_table_t *global_sym_table;
    GList *symbols_list;
    GList *p;

    params.out = out_stream;
    label_gen_init(&(params.label_gen));

    x86_prelude(out_stream, source_file);
    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    symbols_list = sym_table_get_all_symbols(global_sym_table);
    for (p = symbols_list; p != NULL; p = g_list_next(p))
    {
        if (IR_IS_FUNCTION(p->data))
        {
            x86_compile_function_def(&params, p->data);
        }
        else
        {
            /* unexpected symbol type */
            assert(false);
        }
    }
    g_list_free(symbols_list);        
}

void
x86_compile_expression(x86_comp_params_t *params,
                       IrExpression *expression,
                       sym_table_t *sym_table)
{
    assert(params);
    assert(expression);
    assert(IR_IS_EXPRESSION(expression));

    if (IR_IS_INT_CONSTANT(expression))
    {
        fprintf(params->out,
                "# push integer constant onto stack\n"
                "    pushl $%d\n", 
                ir_int_constant_get_value(IR_INT_CONSTANT(expression)));
    }
    else if (IR_IS_BOOL_CONSTANT(expression))
    {
        gboolean val;

        val = ir_bool_constant_get_value(IR_BOOL_CONSTANT(expression));
        fprintf(params->out,
               "# push boolean constant onto stack\n"
               "    pushl $%d\n",
               val ? 1 : 0);
    }
    else if (IR_IS_UNARY_OPERATION(expression))
    {
        x86_compile_unary_op(params,
                             IR_UNARY_OPERATION(expression),
                             sym_table);
    }
    else if (IR_IS_BINARY_OPERATION(expression))
    {
        x86_compile_binary_op(params,
                              IR_BINARY_OPERATION(expression),
                              sym_table);
    }
    else if (IR_IS_VARIABLE(expression))
    {
        x86_compile_variable_ref(params,
                                 IR_VARIABLE(expression));
    }
    else if (IR_IS_FUNCTION_CALL(expression))
    {
        x86_compile_func_call(params,
                              IR_FUNCTION_CALL(expression),
                              sym_table,
                              true);
    }
    else if (IR_IS_ARRAY_CELL_REF(expression))
    {
        x86_compile_array_cell_ref(params,
                                   IR_ARRAY_CELL_REF(expression),
                                   sym_table);
    }
    else if (IR_IS_CAST(expression))
    {
        x86_compile_cast(params,
                         IR_CAST(expression),
                         sym_table);
    }
    else
    {
        /* unexpected expression type */
        printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(expression)));
        assert(false);
    }
}

void
x86_compile_code_block(x86_comp_params_t *params,
                       IrCodeBlock *code_block)
{
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
        IrExpression *var_init = ir_variable_get_initializer(var);

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
                        var_init = IR_EXPRESSION(ir_int_constant_new(0));
                        break;
                    case bool_type:
                        var_init = IR_EXPRESSION(ir_bool_constant_new(false));
                        break;
                    default:
                        assert(false);
                }
            }
            else if (XDP_IS_AST_STATIC_ARRAY_TYPE(var_type))
            {
                /* not implemented */
                assert(false);
            }
            else
            {
                /* unsupported complex type */
                assert(false);
            }
        }

        if (XDP_IS_AST_BASIC_TYPE(var_type))
        {
            x86_gen_variable_assigment(params, var, var_init, locals);
        }
        else if (XDP_AST_STATIC_ARRAY_TYPE(var_type))
        {
            x86_gen_array_literal_assigment(params,
                                            var,
                                            IR_ARRAY_LITERAL(var_init),
                                            locals);
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
    GSList *stmts = ir_code_block_get_statments(code_block);
    for (;stmts != NULL; stmts = g_slist_next(stmts))
    {
        IrStatment *statment = IR_STATMENT(stmts->data);

        if (IR_IS_RETURN(statment))
        {
            IrReturn *ret = IR_RETURN(statment);
            IrExpression *return_val = ir_return_get_return_value(ret);
            if (return_val != NULL)
            {
                x86_compile_expression(params, return_val, locals);
                fprintf(params->out,
                        "    popl %%eax\n");
            }
            fprintf(params->out,
                    "    leave\n"
                    "    ret\n");
        }
        else if (IR_IS_FUNCTION_CALL(statment))
        {
            x86_compile_func_call(params, 
                                  IR_FUNCTION_CALL(statment),
                                  locals,
                                  false);
        }
        else if (IR_IS_ASSIGMENT(statment))
        {
            IrAssigment *assig = IR_ASSIGMENT(statment);
            x86_gen_variable_assigment(params, 
                                       ir_assigment_get_target(assig),
                                       ir_assigment_get_value(assig),
                                       locals);
        }
        else if (IR_IS_CODE_BLOCK(statment))
        {
            x86_compile_code_block(params,
                                   IR_CODE_BLOCK(statment));
        }
        else if (IR_IS_IF_ELSE(statment))
        {
            x86_compile_if_else(params,
                                IR_IF_ELSE(statment),
                                locals);
        }
        else if (IR_IS_WHILE(statment))
        {
            x86_compile_while(params,
                              IR_WHILE(statment),
                              locals);
        }
        else
        {
            /* unexpected statment type */
            printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(stmts->data)));
            assert(false);
        }
    }
}

int
x86_code_block_assign_addrs(x86_comp_params_t *params,
                            int first_num,
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
    for (i = symbols_list; i != NULL; i = g_list_next(i))
    {
        IrVariable *var = i->data;

        if (!IR_IS_VARIABLE(var))
        {
            /* skip non-variables in symbol table */
            continue;
        }

        num -= x86_get_variable_storage_size(var);
        ir_variable_set_location(var, G_OBJECT(x86_frame_offset_new(num)));
        fprintf(params->out, "# variable '%s' location %d\n",
                ir_variable_get_name(var), num);

    }
    g_list_free(symbols_list);
    last_num = num;

    /*
     * assign numbers to children code block's variables
     */
    j = ir_code_block_get_statments(code_block);
    for (; j != NULL; j = g_slist_next(j))
    {
        int vars = 0;
        if (IR_IS_CODE_BLOCK(j->data))
        {
            vars = x86_code_block_assign_addrs(params, num, j->data);
        }
        else if (IR_IS_IF_ELSE(j->data))
        {
            vars = x86_if_else_assign_addrs(params, num, j->data);
        }

        /* 
         * keep track of lowers frame offset assigned 
         * in our sub-blocks 
         */
        if (vars < last_num)
        {
            last_num = vars;
        }
    }

    return last_num;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
x86_prelude(FILE *out, const char *source_file)
{
    fprintf(out,
            "    .file \"%s\"\n"
            "    .text\n",
            source_file);
}

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunction *func)
{
    GSList *i;
    char *func_name;
    int len;
    int addr;
    int stack_start = -4;
    int stack_size;
    sym_table_t *param_symbols;
    bool push_last_arg = false;

    func_name = ir_function_get_name(func);
    /* generate function symbol declaration and function entry point label */
    fprintf(params->out,
            ".globl %s\n"
            "    .type %s, @function\n"
            "%s:\n",
            func_name, func_name, func_name);

    /* assign locations to function parameter variables */
    i = ir_function_get_parameters(func);
    param_symbols = ir_function_get_parameter_symbols(func);

    len = g_slist_length(i);
    addr = len * 4;
    for (; i != NULL; i = g_slist_next(i))
    {
        AstVariableDeclaration *var = i->data;


        /* convert ast variable declaration to IR variable object */
        IrVariable *variable =
            IR_VARIABLE(sym_table_get_symbol(param_symbols,
                                 ast_variable_declaration_get_name(var)));


        /* assign variable number */
        if (addr >= 8)
        {
            ir_variable_set_location(variable,
                                     G_OBJECT(x86_frame_offset_new(addr)));
            fprintf(params->out,
                    "# variable '%s' location %d\n",
                    ir_variable_get_name(variable), addr);
        }
        else
        {
            /* last argument is stored in EAX register */
            push_last_arg = true;
            stack_start = -4;
            ir_variable_set_location(variable,
                                     G_OBJECT(x86_frame_offset_new(-4)));
            fprintf(params->out,
                    "# variable '%s' location %d\n",
                    ir_variable_get_name(variable), -4);

        }
        addr -= 4;
    }

    /* assign stack offset to local variables in function body */
    stack_size = 
        x86_code_block_assign_addrs(params,
                                    stack_start,
                                    ir_function_get_body(func));

    /* pad stack to allign it on 4-byte boundary */
    if ((stack_size % 4) != 0)
    {
        stack_size -= 4 + stack_size % 4;
    }

    /* generate code to allocate function frame on the stack */
    fprintf(params->out,
            "    enter $%d, $0\n",
            -stack_size);

    /* generate code to store last function argument on the stack */
    if (push_last_arg)
    {
        fprintf(params->out, "    movl %%eax, -4(%%ebp)\n");
    }

    /* generate code for function body */
    x86_compile_code_block(params,
                           ir_function_get_body(func));
}

static void
x86_compile_while(x86_comp_params_t *params,
                  IrWhile *while_statment,
                  sym_table_t *sym_table)
{
    char start_label[LABEL_MAX_LEN];
    char end_label[LABEL_MAX_LEN];

    label_gen_next(&(params->label_gen), start_label);
    label_gen_next(&(params->label_gen), end_label);

    fprintf(params->out,
            "# while loop\n"
            "%s: # loop start\n"
            "# eval loop condition\n",
            start_label);

    x86_compile_expression(params,
                          ir_while_get_loop_condition(while_statment),
                          sym_table);

    fprintf(params->out,
            "# exit loop if loop condition is false\n"
            "    popl %%eax\n"
            "    cmp $1, %%eax\n"
            "    jne %s\n"
            "# loop body\n",
            end_label);

    x86_compile_code_block(params, ir_while_get_body(while_statment));

    fprintf(params->out,
            "    jmp %s\n"
            "%s: # loop end\n",
            start_label,
            end_label);
}

static int
x86_get_variable_storage_size(IrVariable *variable)
{
    assert(variable);
    assert(IR_IS_VARIABLE(variable));

    AstDataType *variable_type;

    variable_type = ir_variable_get_data_type(variable);
    if (XDP_IS_AST_BASIC_TYPE(variable_type))
    {
        AstBasicType *basic_type = XDP_AST_BASIC_TYPE(variable_type);

        return types_get_storage_size(ast_basic_type_get_data_type(basic_type));
    }
    else if (XDP_IS_AST_STATIC_ARRAY_TYPE(variable_type))
    {
        AstStaticArrayType *array_type;
        int len;

        array_type = XDP_AST_STATIC_ARRAY_TYPE(variable_type);
        len = ast_static_array_type_get_length(array_type);

        return 
            len * types_get_storage_size(
                      ast_static_array_type_get_data_type(array_type));
    }
    else
    {
        /* unexpected data type */
        assert(false);
    }
    /* we should not get here */
    assert(false);
    return 0;
}

static void
x86_gen_store_value(x86_comp_params_t *params,
                    int frame_offset,
                    int storage_size)
{
    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "# store 32-bit variable value from the stack\n"
                    "    popl %d(%%ebp)\n",
                    frame_offset);
            break;
        case 1:
            fprintf(params->out,
                    "# store 8-bit variable value from the stack\n"
                    "    popl %%eax\n"
                    "    movb %%al, %d(%%ebp)\n",
                    frame_offset);
            break;
        default:
            /* unexpected/unsupported storage size */
            assert(false);
    }
}

static void
x86_gen_variable_assigment(x86_comp_params_t *params,
                           IrVariable *variable,
                           IrExpression *expression,
                           sym_table_t *sym_table)
{
    X86FrameOffset *addr;

    addr = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    x86_compile_expression(params, expression, sym_table);
    x86_gen_store_value(params,
                        x86_frame_offset_get_offset(addr),
                        x86_get_variable_storage_size(variable));
}

static void
x86_gen_array_literal_assigment(x86_comp_params_t *params, 
                                IrVariable *variable,
                                IrArrayLiteral *array_literal,
                                sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_VARIABLE(variable));
    assert(XDP_IS_AST_STATIC_ARRAY_TYPE(ir_variable_get_data_type(variable)));
    assert(X86_IS_FRAME_OFFSET(ir_variable_get_location(variable)));
    assert(IR_IS_ARRAY_LITERAL(array_literal));
    assert(sym_table);

    X86FrameOffset *array_loc;
    int loc;
    int storage_size;
    GSList *i;
    AstStaticArrayType *array_type;

    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    array_type = XDP_AST_STATIC_ARRAY_TYPE(ir_variable_get_data_type(variable));
    storage_size = 
      types_get_storage_size(ast_static_array_type_get_data_type(array_type));

    loc = x86_frame_offset_get_offset(array_loc);
    i = ir_array_literal_get_values(array_literal);

    fprintf(params->out, "# assign array literal to array '%s'\n",
            ir_variable_get_name(variable));

    for (; i != NULL; i = g_slist_next(i), loc += storage_size)
    {
        fprintf(params->out,
                "# evaluate value for loc %d(%%ebp)\n",
                loc);
        x86_compile_expression(params, i->data, sym_table);
        fprintf(params->out,
                "# store value in array cell\n");

        x86_gen_store_value(params, loc, storage_size);
    }
}

static void
x86_compile_array_cell_ref(x86_comp_params_t *params,
                           IrArrayCellRef *array_cell,
                           sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ARRAY_CELL_REF(array_cell));
    assert(sym_table);

    fprintf(params->out,
            "# fetch array cell value\n"
            "  # evaluate index expression");

    x86_compile_expression(params,
                           ir_array_cell_ref_get_index(array_cell),
                           sym_table);


    X86FrameOffset *array_loc;
    IrVariable *variable;
    int storage_size;
    AstStaticArrayType *array_type;


    variable = ir_array_cell_ref_get_symbol(array_cell);
    array_loc = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    array_type = XDP_AST_STATIC_ARRAY_TYPE(ir_variable_get_data_type(variable));
    storage_size = 
      types_get_storage_size(ast_static_array_type_get_data_type(array_type));

    fprintf(params->out,
            "    popl %%eax # store array index in eax\n");

    switch (storage_size)
    {
        case 4:
            fprintf(params->out,
                    "   # fetch 32-bit array element\n"
                    "    movl %d(%%ebp, %%eax, 4), %%eax\n"
                    "    pushl %%eax\n",
                    x86_frame_offset_get_offset(array_loc));
            break;
        case 1:
            fprintf(params->out,
                    "   # fetch 8-bit array element\n"
                    "    xor %%ebx, %%ebx\n"
                    "    movb %d(%%ebp, %%eax, 1), %%bl\n"
                    "    pushl %%ebx\n",
                    x86_frame_offset_get_offset(array_loc));
            break;
        default:
            /* unexpected storage size */
            assert(false);
    }              
}

static void
x86_compile_unary_op(x86_comp_params_t *params,
                     IrUnaryOperation *op,
                     sym_table_t *sym_table)
{
    ast_unary_op_type_t op_type;

    op_type = ir_unary_operation_get_operation(op);

    x86_compile_expression(params,
                           ir_unary_operation_get_operand(op),
                           sym_table);

    switch (op_type)
    {    
        case ast_arithm_neg_op:
            fprintf(params->out,
                    "    negl (%%esp)\n");
            break;
        case ast_bool_neg_op:
            fprintf(params->out,
                    "    notl (%%esp)\n"
                    "    andl $0x1, (%%esp)\n");
            break;
        default:
            /* unexpected unary operation */
            assert(false);
    }

}

static void
x86_compile_iarithm_op(x86_comp_params_t *params,
                       IrBinaryOperation *op,
                       sym_table_t *sym_table)
{
    char *op_mnemonic;
    bool signextend_left_op = false;

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_plus_op:
            op_mnemonic = "addl";
            break;
        case ast_minus_op:
            op_mnemonic = "subl";
            break;
        case ast_mult_op:
            op_mnemonic = "imul";
            break;
        case ast_division_op:
            op_mnemonic = "idiv";
            /*
             * when doing 32-bit signed division divident with idev we
             * must be first extended to 64-bit value
             */
            signextend_left_op = true;
            break;
        default:
            /* unexpected operation type */
            assert(false);
    }
    x86_compile_expression(params,
                           ir_binary_operation_get_left(op),
                           sym_table);
    x86_compile_expression(params,
                           ir_binary_operation_get_right(op),
                           sym_table);
    fprintf(params->out,
            /* move left operand into eax */
            "    movl 4(%%esp), %%eax\n"
            /* place-holder for possible 64-bit extension instruction */
            "%s"
            /* evalute operation */
            "    %s (%%esp), %%eax\n"
            /* remove right operand  from the top of stack */
            "    addl $4, %%esp\n"
            /* overwrite the top stack element with operation result */
            "    movl %%eax, (%%esp)\n",
            /* insert extension of left operand to 64-bit value if needed */
            signextend_left_op ? "    cdq\n" : "",
            op_mnemonic);
    
}

static void
x86_compile_icomp_op(x86_comp_params_t *params,
                     IrBinaryOperation *op,
                     sym_table_t *sym_table)
{
    char *set_suffix;

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_equal_op:
            set_suffix = "e";
            break;
        case ast_not_equal_op:
            set_suffix = "ne";
            break;
        case ast_less_op:
            set_suffix = "l";
            break;
        case ast_greater_op:
            set_suffix = "g";
            break;
        case ast_less_or_eq_op:
            set_suffix = "le";
            break;
        case ast_greater_or_eq_op:
            set_suffix = "ge";
            break;
        default:
            /* unexpected operation type */
            assert(false);
    }

    x86_compile_expression(params,
                           ir_binary_operation_get_left(op),
                           sym_table);
    x86_compile_expression(params,
                           ir_binary_operation_get_right(op),
                           sym_table);
    fprintf(params->out,
            "    xor %%ebx, %%ebx\n"
            "    popl %%eax\n"
            "    cmp %%eax, (%%esp)\n"
            "    set%s %%bl\n"
            "    movl %%ebx, (%%esp)\n",
            set_suffix);
    
}

static void
x86_compile_conditional_op(x86_comp_params_t *params,
                           IrBinaryOperation *op,
                           sym_table_t *sym_table)
{
    char end_label[LABEL_MAX_LEN];
    int shortcut_value;

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_and_op:
            shortcut_value = 0;
            break;
        case ast_or_op:
            shortcut_value = 1;
            break;
        default:
            /* unexpected conditional operation type */
            assert(false);
    }

    label_gen_next(&(params->label_gen), end_label);

    x86_compile_expression(params,
                           ir_binary_operation_get_left(op),
                           sym_table);
    fprintf(params->out,
            /* skip evaluating right operand if we know the operations result */
            "    cmpl $%d, (%%esp)\n"
            "    je %s\n"
            /* remove left operands result from the stack */
            "    addl $4, %%esp\n",
            shortcut_value,
            end_label);

    x86_compile_expression(params,
                           ir_binary_operation_get_right(op),
                           sym_table);
    fprintf(params->out,
            "%s:\n",
            end_label);
}

static void
x86_compile_binary_op(x86_comp_params_t *params,
                      IrBinaryOperation *op,
                      sym_table_t *sym_table)
{
    if (ir_binary_operation_is_iarithm(op))
    {
        x86_compile_iarithm_op(params, op, sym_table);
    }
    else if (ir_binary_operation_is_icomp(op))
    {
        x86_compile_icomp_op(params, op, sym_table);
    }
    else if (ir_binary_operation_is_conditional(op))
    {
        x86_compile_conditional_op(params, op, sym_table);
    }
    else
    {
        /* unexpected operation type */
        assert(false);
    }
}

static void
x86_compile_variable_ref(x86_comp_params_t *params,
                         IrVariable *var)
{
    X86FrameOffset *addr;

    addr = X86_FRAME_OFFSET(ir_variable_get_location(var));

    /* generate code to put the value of the variable on top of the stack */
    switch (x86_get_variable_storage_size(var))
    {
        case 4:
            fprintf(params->out,
                    "# integer variable (32-bit) value fetch\n"
                    "    pushl %d(%%ebp)\n",
                    x86_frame_offset_get_offset(addr));
            break;
        case 1:
            fprintf(params->out,
                    "# boolean variable (8-bit) value fetch\n"
                    "    xor %%eax, %%eax\n"
                    "    movb %d(%%ebp), %%al\n"
                    "    pushl %%eax\n",
                    x86_frame_offset_get_offset(addr));
            break;
        default:
            /* unexpected/unsupported storage size */
            assert(false);
    }
}

/**
 * @param retain_return_value if true and called function is not void, the return 
 *                            value of the function is pushed on the stack,
 *                            if false, the return value is discarded
 */
static void
x86_compile_func_call(x86_comp_params_t *params,
                      IrFunctionCall *func_call,
                      sym_table_t *sym_table,
                      bool retain_return_value)
{
    int arg_num;
    GSList *i;
    AstDataType *func_data_type;

    i = ir_function_call_get_arguments(func_call);
    arg_num = g_slist_length(i);
    for (; i != NULL; i = g_slist_next(i))
    {
        x86_compile_expression(params, i->data, sym_table);
    }

    if (arg_num > 0)
    {
        fprintf(params->out,
                "# put last argument into eax\n"
                "    popl %%eax\n");
    }

    fprintf(params->out,
            "# invoke function\n"
            "    call %s\n",
            ir_function_call_get_name(func_call));

    if (arg_num > 1)
    {
        fprintf(params->out,
                "# remove function call aruments from the stack\n"
                "    addl $%d, %%esp\n",
                (arg_num - 1) * 4);
    }

    func_data_type = ir_expression_get_data_type(IR_EXPRESSION(func_call));

    if (!XDP_IS_AST_BASIC_TYPE(func_data_type))
    {
        /* calling function with non-basic return type is not implemented */
        assert(false);
    }

    if (retain_return_value)
    {
        switch (ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(func_data_type)))
        {
           case void_type:
               /* nop */
               break;
           case bool_type:
           case int_type:
               fprintf(params->out,
                       "# push function return value on the stack\n"
                       "    push %%eax\n");
               break;
           default:
               /* unexpected basic data type */
               assert(false);
        }
    }
}
