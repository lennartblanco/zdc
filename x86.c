#include <stdbool.h>
#include <string.h>

#include "x86.h"
#include "x86_frame_offset.h"
#include "x86_reg_location.h"
#include "ast_int_constant.h"
#include "ast_bool_constant.h"
#include "ast_binary_operation.h"
#include "ast_basic_type.h"
#include "ast_static_array_type.h"
#include "ast_variable_declaration.h"
#include "ast_variable_ref.h"
#include "ast_return.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_int_constant.h"
#include "ir_bool_constant.h"
#include "ir_variable.h"
#include "ir_return.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define FUNCTION_EXIT_LABEL_POSTFIX "_exit"

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_prelude(FILE *out, const char *source_file);

static void
x86_compile_function_def(FILE *out, IrFunction *func);

static void
x86_compile_code_block(FILE *out,
                       IrCodeBlock *code_block,
                       char *return_label);

static void
x86_compile_expression(FILE *out,
                       IrExpression *expression,
                       sym_table_t *sym_table);
static void
x86_compile_binary_op(FILE *out,
                      IrBinaryOperation *op,
                      sym_table_t *sym_table);

static void
x86_compile_unary_op(FILE *out,
                     IrUnaryOperation *op,
                     sym_table_t *sym_table);

static int
x86_code_block_assign_addrs(FILE *out,
                            int first_num,
                            IrCodeBlock *code_block);

static void
x86_gen_variable_assigment(FILE *out,
                           IrVariable *variable,
                           IrExpression *expression,
                           sym_table_t *sym_table);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream,
             const char *source_file)
{
    sym_table_t *global_sym_table;
    GList *symbols_list;
    GList *p;

    x86_prelude(out_stream, source_file);
    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    symbols_list = sym_table_get_all_symbols(global_sym_table);
    for (p = symbols_list; p != NULL; p = g_list_next(p))
    {
        if (IR_IS_FUNCTION(p->data))
        {
            x86_compile_function_def(out_stream, p->data);
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

static void
x86_prelude(FILE *out, const char *source_file)
{
    fprintf(out,
            "    .file \"%s\"\n"
            "    .text\n",
            source_file);
}

static void
x86_compile_function_def(FILE *out, IrFunction *func)
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
    fprintf(out,
            ".globl %s\n"
            "    .type %s, @function\n"
            "%s:\n"
            "    pushl %%ebp\n"
            "    movl %%esp, %%ebp\n",
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
            fprintf(out, "# variable '%s' location %d\n",
                    ir_variable_get_name(variable), addr);
        }
        else
        {
            /* last argument is stored in EAX register */
            push_last_arg = true;
            stack_start = -4;
            ir_variable_set_location(variable,
                                     G_OBJECT(x86_frame_offset_new(-4)));
            fprintf(out, "# variable '%s' location %d\n",
                    ir_variable_get_name(variable), -4);

        }
        addr -= 4;
    }

    /* assign stack offset to local variables in function body */
    stack_size = 
        x86_code_block_assign_addrs(out, stack_start, ir_function_get_body(func));

    /* pad stack to allign it on 4-byte boundary */
    if ((stack_size % 4) != 0)
    {
        stack_size -= 4 + stack_size % 4;
    }

    /* generate code to store last function argument on the stack */
    if (push_last_arg)
    {
        fprintf(out, "    pushl %%eax\n");
    }

    /* generate code to allocate function frame on the stack */
    fprintf(out,
            "    subl $%d, %%esp\n",
            -stack_size);

    /* set-up function exit label */
    char exit_label[strlen(func_name) + 
                    strlen(FUNCTION_EXIT_LABEL_POSTFIX) + 1];

    sprintf(exit_label, "%s"FUNCTION_EXIT_LABEL_POSTFIX, func_name);

    /* generate code for function body */
    x86_compile_code_block(out, ir_function_get_body(func), exit_label);

    /* generate function exit part */
    fprintf(out,
            "%s:\n"
            "    movl %%ebp, %%esp\n"
            "    popl %%ebp\n"
            "    ret\n",
            exit_label);

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

        switch (ast_basic_type_get_data_type(basic_type))
        {
            case int_type:
                return 4;
            case bool_type:
                return 1;
            default:
                /* unexpected basic data type */
                assert(false);
        }
    }
    else if (XDP_IS_AST_STATIC_ARRAY_TYPE(variable_type))
    {
        AstStaticArrayType *array_type;
        int len;

        array_type = XDP_AST_STATIC_ARRAY_TYPE(variable_type);
        len = ast_static_array_type_get_length(array_type);
        switch (ast_static_array_type_get_data_type(array_type))
        {
            case int_type:
                return len * 4;
            case bool_type:
                return len * 1;
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
    /* we should not get here */
    assert(false);
    return 0;
}

static int
x86_code_block_assign_addrs(FILE *out,
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
        fprintf(out, "# variable '%s' location %d\n",
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
            vars = x86_code_block_assign_addrs(out, num, j->data);
            /* 
             * keep track if lowers frame offset assigned 
             * in our sub-blocks 
             */
        }

        if (vars < last_num)
        {
            last_num = vars;
        }
    }

    return last_num;
}

static void
x86_compile_code_block(FILE *out,
                       IrCodeBlock *code_block,
                       char *return_label)
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
            x86_gen_variable_assigment(out, var, var_init, locals);
        }
        else if (XDP_AST_STATIC_ARRAY_TYPE(var_type))
        {
//            AstStaticArrayType *sarray = XDP_AST_STATIC_ARRAY_TYPE(var_type);
            printf("static array\n");
            /* not implemented */
            assert(false);
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
        if (IR_IS_RETURN(stmts->data))
        {
            IrReturn *ret = IR_RETURN(stmts->data);
            IrExpression *return_val = ir_return_get_return_value(ret);
            if (return_val != NULL)
            {
                x86_compile_expression(out, return_val, locals);
                /* todo: 
                 * here we should take into account the size
                 * of the data type we are about to return
                 * popl %eax only works if we are returning 32-bit value,
                 *    if we want to return for example boolean, this would put
                 *     garbage into most significant parts of %eax and stack 
                 *                              will underflow with 3 bytes
                 *
                 */
                fprintf(out,
                        "    popl %%eax\n");
            }
            fprintf(out, "    jmp %s\n", return_label);
        }
        else
        {
            /* unexpected statment type */
            printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(stmts->data)));
            assert(false);
        }
    }
}

static void
x86_gen_variable_assigment(FILE *out,
                           IrVariable *variable,
                           IrExpression *expression,
                           sym_table_t *sym_table)
{
    X86FrameOffset *addr;

    addr = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    x86_compile_expression(out, expression, sym_table);
    switch (x86_get_variable_storage_size(variable))
    {
        case 4:
            fprintf(out,
                    "# assign 32-bit variable value from the stack\n"
                    "    popl %d(%%ebp)\n",
                    x86_frame_offset_get_offset(addr));
            break;
        case 1:
            fprintf(out,
                    "# assign 8-bit variable value from the stack\n"
                    "    popl %%eax\n"
                    "    movb %%al, %d(%%ebp)\n",
                    x86_frame_offset_get_offset(addr));
            break;
        default:
            /* unexpected/unsupported storage size */
            assert(false);
    }
}

static void
x86_compile_unary_op(FILE *out,
                     IrUnaryOperation *op,
                     sym_table_t *sym_table)
{
    ast_unary_op_type_t op_type;

    op_type = ir_unary_operation_get_operation(op);

    x86_compile_expression(out,
                           ir_unary_operation_get_operand(op),
                           sym_table);

    switch (op_type)
    {    
        case ast_arithm_neg_op:
            fprintf(out,
                    "    negl (%%esp)\n");
            break;
        case ast_bool_neg_op:
            fprintf(out,
                    "    notl (%%esp)\n"
                    "    andl $0x1, (%%esp)\n");
            break;
        default:
            /* unexpected unary operation */
            assert(false);
    }

}

static void
x86_compile_binary_op(FILE *out,
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
    x86_compile_expression(out,
                           ir_binary_operation_get_left(op),
                           sym_table);
    x86_compile_expression(out,
                           ir_binary_operation_get_right(op),
                           sym_table);
    fprintf(out,
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
x86_compile_variable_ref(FILE *out,
                         IrVariable *var)
{
    X86FrameOffset *addr;

    addr = X86_FRAME_OFFSET(ir_variable_get_location(var));

    /* generate code to put the value of the variable on top of the stack */
    switch (x86_get_variable_storage_size(var))
    {
        case 4:
            fprintf(out,
                    "# integer variable (32-bit) value fetch\n"
                    "    pushl %d(%%ebp)\n",
                    x86_frame_offset_get_offset(addr));
            break;
        case 1:
            fprintf(out,
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

static void
x86_compile_expression(FILE *out,
                       IrExpression *expression,
                       sym_table_t *sym_table)
{
    assert(out);
    assert(expression);
    assert(IR_IS_EXPRESSION(expression));

    if (IR_IS_INT_CONSTANT(expression))
    {
        fprintf(out,
                "# push integer constant onto stack\n"
                "    pushl $%d\n", 
                ir_int_constant_get_value(IR_INT_CONSTANT(expression)));
    }
    else if (IR_IS_BOOL_CONSTANT(expression))
    {
        gboolean val;

        val = ir_bool_constant_get_value(IR_BOOL_CONSTANT(expression));
        fprintf(out,
               "# push boolean constant onto stack\n"
               "    pushl $%d\n",
               val ? 1 : 0);
    }
    else if (IR_IS_UNARY_OPERATION(expression))
    {
        x86_compile_unary_op(out,
                             IR_UNARY_OPERATION(expression),
                             sym_table);
    }
    else if (IR_IS_BINARY_OPERATION(expression))
    {
        x86_compile_binary_op(out,
                              IR_BINARY_OPERATION(expression),
                              sym_table);
    }
    else if (IR_IS_VARIABLE(expression))
    {
        x86_compile_variable_ref(out,
                                 IR_VARIABLE(expression));
    }
    else
    {
        /* unexpected expression type */
        printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(expression)));
        assert(false);
    }
}
