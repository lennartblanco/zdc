#include <stdbool.h>
#include <string.h>

#include "label_gen.h"
#include "types.h"
#include "x86.h"
#include "x86_data.h"
#include "x86_cast.h"
#include "x86_array.h"
#include "x86_if_else.h"
#include "x86_func_call.h"
#include "x86_code_block.h"
#include "x86_frame_offset.h"
#include "x86_reg_location.h"
#include "ir_property.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_char_constant.h"
#include "ir_bool_constant.h"
#include "ir_scalar.h"
#include "ir_variable.h"
#include "ir_return.h"
#include "ir_function_call.h"
#include "ir_if_else.h"
#include "ir_assigment.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_text_prelude(x86_comp_params_t *params,
                 sym_table_t *sym_table);

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunctionDef *func_def);

static void
x86_compile_binary_op(x86_comp_params_t *params,
                      IrBinaryOperation *op,
                      sym_table_t *sym_table);

static void
x86_compile_unary_op(x86_comp_params_t *params,
                     IrUnaryOperation *op,
                     sym_table_t *sym_table);

static void
x86_gen_scalar_assigment(x86_comp_params_t *params,
                         IrVariable *variable,
                         IrExpression *expression,
                         sym_table_t *sym_table);

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
x86_compile_scalar(x86_comp_params_t *params,
                   IrScalar *scalar);

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

static void
x86_compile_property(x86_comp_params_t *params,
                     IrProperty *property,
                     sym_table_t *sym_table);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_code(IrModule *module,
             FILE *out_stream,
             const char *source_file)
{
    x86_comp_params_t params;
    sym_table_t *global_sym_table;
    GSList *i;

    params.out = out_stream;
    label_gen_init(&(params.label_gen));
    global_sym_table = ir_module_get_symbols(module);

    fprintf(out_stream,
            "    .file \"%s\"\n",
            source_file);

    x86_gen_data_section(&params,
                         ir_module_get_data_section(module));

    x86_text_prelude(&params, global_sym_table);

    i = ir_module_get_function_defs(module);
    for (; i != NULL; i = g_slist_next(i))
    {
        assert(IR_IS_FUNCTION_DEF(i->data));
        x86_compile_function_def(&params, i->data);
    }
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
                "    pushl $%d     # push integer constant onto stack\n", 
                ir_int_constant_get_value(IR_INT_CONSTANT(expression)));
    }
    else if (IR_IS_UINT_CONSTANT(expression))
    {
        fprintf(params->out,
                "    pushl $%u     # push unsigned integer constant onto stack\n", 
                ir_uint_constant_get_value(IR_UINT_CONSTANT(expression)));
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
    else if (IR_IS_CHAR_CONSTANT(expression))
    {
        fprintf(params->out,
                "    pushl $%u     # push char constant onto stack\n", 
                ir_char_constant_get_value(IR_CHAR_CONSTANT(expression)));
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
    else if (IR_IS_ARRAY(expression))
    {
        x86_compile_array(params, IR_ARRAY(expression), sym_table);
    }
    else if (IR_IS_ARRAY_CELL(expression))
    {
        x86_compile_array_cell(params,
                               IR_ARRAY_CELL(expression),
                               sym_table);
    }
    else if (IR_IS_ARRAY_LITERAL(expression))
    {
        x86_compile_array_literal(params,
                                  IR_ARRAY_LITERAL(expression),
                                  sym_table);
    }
    else if (IR_IS_SCALAR(expression))
    {
        x86_compile_scalar(params, IR_SCALAR(expression));
    }
    else if (IR_IS_FUNCTION_CALL(expression))
    {
        x86_compile_func_call(params,
                              IR_FUNCTION_CALL(expression),
                              sym_table,
                              true);
    }
    else if (IR_IS_ARRAY_SLICE(expression))
    {
        x86_compile_array_slice(params,
                                IR_ARRAY_SLICE(expression),
                                sym_table);
                               
    }
    else if (IR_IS_CAST(expression))
    {
        x86_compile_cast(params,
                         IR_CAST(expression),
                         sym_table);
    }
    else if (IR_IS_PROPERTY(expression))
    {
        x86_compile_property(params, IR_PROPERTY(expression), sym_table);
    }
    else
    {
        /* unexpected expression type */
        printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(expression)));
        assert(false);
    }
}

/* @todo:
 * add following unit test
 *
 * int asd;
 * for (asd = -20; asd < 31; asd++) {
 *  assert(x86_inc_to_word_boundary(asd) % 4 == 0);
 * }
 */
int
x86_inc_to_word_boundary(int addr)
{
    int mod4 = addr % 4;

    if (mod4 == 0)
    {
        return addr;
    }

    return addr + (4 - (mod4));
}

int
x86_get_expression_storage_size(IrExpression *expression)
{
    assert(IR_IS_EXPRESSION(expression));

    DtDataType *data_type;
    data_type = ir_expression_get_data_type(expression);

    if (DT_IS_BASIC_TYPE(data_type) || DT_IS_STATIC_ARRAY_TYPE(data_type))
    {
        return dt_data_type_get_size(data_type);
    }
    else if (DT_IS_ARRAY_TYPE(data_type))
    {
        /* 4 bytes for array length + 4 bytes for array data pointer */
        return 8;
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

/**
 * @return False if this type of parameter is passed via stack, when
 *         used as last argument in function call.
 *         True if it is passed via eax register.
 */ 
bool
x86_in_reg_as_last_func_arg(DtDataType *parameter_type)
{
    /*
     * @todo: here we need to take into account 3 byte structs and
     * floating point variables
     */
    return dt_data_type_get_size(parameter_type) <= 4;
}

void
x86_compile_assigment(x86_comp_params_t *params,
                      IrAssigment *assigment,
                      sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_ASSIGMENT(assigment));
    assert(sym_table);

    IrLvalue *lvalue;

    lvalue = ir_assigment_get_lvalue(assigment);

    if (IR_IS_SCALAR(lvalue))
    {
        x86_gen_scalar_assigment(params,
                                 ir_lvalue_get_variable(lvalue),
                                 ir_assigment_get_value(assigment),
                                 sym_table);
    }
    else if (IR_IS_ARRAY(lvalue))
    {
        x86_gen_array_handle_assigment(params, assigment, sym_table);
    }
    else if (IR_IS_ARRAY_CELL(lvalue))
    {
        x86_gen_array_cell_assigment(params, assigment, sym_table);
    }
    else if (IR_IS_ARRAY_SLICE(lvalue))
    {
        x86_compile_array_slice_assigment(params, assigment, sym_table);
    }
    else
    {
        /* unexpected lvalue type */
        assert(false);
    }
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

/**
 * Generate prelude of the .text section for assembly file.
 */
static void
x86_text_prelude(x86_comp_params_t *params,
                 sym_table_t *sym_table)
{
    IrSymbol *main_symb;

    fprintf(params->out, "    .text\n");

    /*
     * if entry point function main() is defined,
     * generate code to call it and to terminate the application
     * after main() have returned
     */
    main_symb = sym_table_get_symbol(sym_table, "main", NULL);
    if (IR_IS_FUNCTION_DEF(main_symb))
    {
      IrFunctionDef *main_func;
      bool exit_code_returned;

      main_func = IR_FUNCTION_DEF(main_symb);
      
      /* only entry point without arguments supported */
      assert(ir_function_def_get_parameters(main_func) == NULL);

      /*
       * if main() returns a value, use it as exit code,
       * otherwise exit with code 0
       */
      exit_code_returned = 
        types_is_int(ir_function_def_get_return_type(main_func));

      fprintf(params->out,
              ".globl _init\n"
              "_init:\n"
              "    ret\n"
              ".globl main\n"
              "main:\n"
              "    call %s\n"
              "    pushl %s\n"
              "    call exit\n",
              ir_function_def_get_mangled_name(main_func),
              exit_code_returned ? "%eax" : "$0");
    }
}

static void
x86_func_params_assign_addrs(x86_comp_params_t *params,
                             IrFunctionDef *func_def,
                             bool *last_arg_in_reg)
{
    GSList *rev_params;
    GSList *i;
    IrVariable *param;
    DtDataType *param_type;
    X86FrameOffset* offset;
    int next_offset;

    *last_arg_in_reg = false;

    i = ir_function_def_get_parameters(func_def);
    if (i == NULL)
    {
      /* function without any in-parameters, we are done here */
      return;
    }

    /* we want to proccess function parameters in reversed order */     
    rev_params = g_slist_reverse(g_slist_copy(i));
    i = rev_params;

    /*
     * Depending on if last argument is passed via stack or eax,
     * assign correct stack frame offset and set next available offset
     */
    if (IR_IS_VARIABLE(i->data))
    {
        param = IR_VARIABLE(i->data);
        param_type = ir_variable_get_data_type(param);
    }
    else
    {
        /* unnamed parameter */
        assert(DT_IS_DATA_TYPE(i->data));
        param = NULL;
        param_type = i->data;
    }

    *last_arg_in_reg =  x86_in_reg_as_last_func_arg(param_type);
    if (*last_arg_in_reg)
    {
        offset = x86_frame_offset_new(-4);
        next_offset = 8;
    }
    else
    {
        offset = x86_frame_offset_new(8);
        next_offset = 
            x86_inc_to_word_boundary(8 + dt_data_type_get_size(param_type));
    }
    if (param != NULL)
    {
        ir_variable_set_location(param, G_OBJECT(offset));
    }

    /*
     * assign stack frame offset to the rest of parametrs
     */
    i = g_slist_next(i);
    for (; i != NULL; i = g_slist_next(i))
    {
        if (IR_IS_VARIABLE(i->data))
        {
            param = IR_VARIABLE(i->data);
            offset = x86_frame_offset_new(next_offset);
            ir_variable_set_location(param, G_OBJECT(offset));
            next_offset += x86_get_expression_storage_size(IR_EXPRESSION(param));
        }
        else
        {
            /* unnamed parameter */
            assert(DT_IS_DATA_TYPE(i->data));
            next_offset += dt_data_type_get_size(i->data);
        }
        next_offset = x86_inc_to_word_boundary(next_offset);
    }

    /*
     * dump offset allocations into generated file for
     * for ease of debugging of generated assembly
     */
    i = ir_function_def_get_parameters(func_def);
    for (; i != NULL; i = g_slist_next(i))
    {
        if (IR_IS_VARIABLE(i->data))
        {
            param = IR_VARIABLE(i->data);
            offset = X86_FRAME_OFFSET(ir_variable_get_location(param));
            fprintf(params->out,
                    "# variable '%s' location '%d'\n",
                    ir_variable_get_name(param),
                    x86_frame_offset_get_offset(offset));
        }
    }

    /* clean-up */
    g_slist_free(rev_params);
}

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunctionDef *func_def)
{
    char *func_name;
    int stack_start;
    int stack_size;
    bool push_last_arg;

    func_name = ir_function_def_get_mangled_name(func_def);
    /* generate function symbol declaration and function entry point label */
    fprintf(params->out,
            ".globl %s\n"
            "    .type %s, @function\n"
            "%s:\n",
            func_name, func_name, func_name);

    x86_func_params_assign_addrs(params, func_def, &push_last_arg);

    stack_start = push_last_arg ? -4 : 0;    

    /* assign stack offset to local variables in function body */
    stack_size = 
        x86_code_block_assign_addrs(params,
                                    stack_start,
                                    ir_function_def_get_body(func_def));

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
                           ir_function_def_get_body(func_def));
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
x86_gen_scalar_assigment(x86_comp_params_t *params,
                         IrVariable *variable,
                         IrExpression *expression,
                         sym_table_t *sym_table)
{
    X86FrameOffset *addr;

    addr = X86_FRAME_OFFSET(ir_variable_get_location(variable));
    x86_compile_expression(params, expression, sym_table);
    x86_gen_store_value(
        params,
        x86_frame_offset_get_offset(addr),
        x86_get_expression_storage_size(IR_EXPRESSION(variable)));
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
    if (ir_binary_operation_is_arithm(op))
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
x86_compile_scalar(x86_comp_params_t *params,
                   IrScalar *scalar)
{
    IrVariable *var;
    X86FrameOffset *addr;

    var = ir_lvalue_get_variable(IR_LVALUE(scalar));
    addr = X86_FRAME_OFFSET(ir_variable_get_location(var));

    /* generate code to put the value of the variable on top of the stack */
    switch (x86_get_expression_storage_size(IR_EXPRESSION(var)))
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

static void
x86_compile_property(x86_comp_params_t *params,
                     IrProperty *property,
                     sym_table_t *sym_table)
{
    assert(params);
    assert(IR_IS_PROPERTY(property));
    assert(sym_table);

    /* only .length property implemented */
    assert(ir_property_get_id(property) == ir_prop_length);

    /* only .length property on lvalues implemented */
    assert(IR_IS_LVALUE(ir_property_get_expression(property)));

    X86FrameOffset *array_loc;

    array_loc =
        X86_FRAME_OFFSET(
            ir_lvalue_get_location(
                IR_LVALUE(ir_property_get_expression(property))));

    fprintf(params->out,
            "# get dynamic array length property\n"
            "    movl %d(%%ebp), %%eax\n"
            "    pushl %%eax\n",
            x86_frame_offset_get_offset(array_loc));
}
