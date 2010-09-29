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
#include "iml_register.h"
#include "iml_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum {
    x86_reg_eax,
    x86_reg_ebx,
    x86_reg_ecx,
    x86_reg_edx,
    x86_reg_esi,
    x86_reg_edi,
    x86_reg_ebp,
};

#define TEMP_REG_NAME "ecx"

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

/**
 * Get the set of available registers on x86 platform.
 *
 * The registers of different categories are returns as list of
 * iml_register_t objects.
 *
 * @param scratch the list of scratch registers
 * @param preserved the list of registers that are preserved across
 *                  function calls
 */
static void
x86_get_registers(GSList **scratch,
                  GSList **preserved);

/**
 * generate x86 assembly from IR
 */
static void
x86_gen_code(IrModule *module,
             FILE *out_stream,
             const char *source_file);

static void
x86_assign_var_locations(iml_func_frame_t *frame);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_init(arch_backend_t *backend)
{
    backend->get_registers = x86_get_registers;
    backend->assign_var_locations = x86_assign_var_locations;
    backend->gen_code = x86_gen_code;
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
    else if (IR_IS_ENUM_MEMBER(expression))
    {
        IrEnumMember *enum_member = IR_ENUM_MEMBER(expression);

        x86_compile_expression(params,
                               ir_enum_member_get_value(enum_member),
                               sym_table);
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

    return dt_data_type_get_size(ir_expression_get_data_type(expression));
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

static void
x86_get_registers(GSList **scratch,
                  GSList **preserved)
{
    GSList *s_regs = NULL; /* scratch registers */
    GSList *p_regs = NULL; /* preserved registers */

    s_regs = g_slist_prepend(s_regs,
                             iml_register_new(x86_reg_edx, "edx"));

    p_regs = g_slist_prepend(p_regs,
                             iml_register_new(x86_reg_ebx, "ebx"));
    p_regs = g_slist_prepend(p_regs,
                             iml_register_new(x86_reg_esi, "esi"));
    p_regs = g_slist_prepend(p_regs,
                             iml_register_new(x86_reg_edi, "edi"));


    *scratch = s_regs;
    *preserved = p_regs;
}

static void
x86_assign_var_locations(iml_func_frame_t *frame)
{
    GSList *params;
    GSList *i;
    gint offset;

    /* assign locations to parameters */
    params = iml_func_frame_get_parameters(frame);
    if (params != NULL)
    {
        params = g_slist_reverse(g_slist_copy(params));

        /* last parameter is passed via eax register */
        iml_variable_set_register(IML_VARIABLE(params->data),
                                  iml_register_new(x86_reg_eax, "eax"));

        for (i = g_slist_next(params), offset = 8;
             i != NULL;
             i = g_slist_next(i), offset += 4)
        {
            iml_variable_set_frame_offset(IML_VARIABLE(i->data), offset);
        }

        /* clean-up */
        g_slist_free(params);
    }

    /*
     * assign offset location to local variables that
     * have not been assigned a register
     */
    offset = -4;

    i = iml_func_frame_get_locals(frame, iml_32b);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            iml_variable_set_frame_offset(var, offset);
            offset -= 4;
        }
    }

    i = iml_func_frame_get_locals(frame, iml_16b);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            iml_variable_set_frame_offset(var, offset);
            offset -= 4;
        }
    }

    i = iml_func_frame_get_locals(frame, iml_8b);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            iml_variable_set_frame_offset(var, offset);
            offset -= 4;
        }
    }

    iml_func_frame_set_size(frame, (guint)(-(offset + 4)));

}

static void
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
x86_compile_return(FILE *out, const char *return_label, iml_operation_t *op)
{
    ImlOperand *val;

    val = iml_operation_get_operand(op, 1);

    if (IML_IS_CONSTANT(val))
    {
        ImlConstant *const_val = IML_CONSTANT(val);

        fprintf(out,
                "    movl $%d, %%eax\n",
                iml_constant_get_val_32b(const_val));
    }
    else if (IML_IS_VARIABLE(val))
    {
        ImlVariable *var_val = IML_VARIABLE(val);
        iml_register_t *reg;

        reg = iml_variable_get_register(var_val);
        if (reg == NULL) {
            fprintf(out,
                    "    movl %d(%%ebp), %%eax\n",
                    iml_variable_get_frame_offset(var_val));
        }
        else
        {
            fprintf(out,
                    "    movl %%%s, %%eax\n",
                    iml_register_get_name(reg));
        }
    }
    else
    {
        /* unexpected operand type */
        assert(false);
    }

    fprintf(out, "    jmp %s\n", return_label);
}

static void
x86_compile_copy(FILE *out, iml_operation_t *op)
{
    ImlOperand *src;
    ImlVariable *dst;
    iml_register_t *dst_reg;


    src = iml_operation_get_operand(op, 1);
    dst = IML_VARIABLE(iml_operation_get_operand(op, 2));
    dst_reg = iml_variable_get_register(dst);

    if (IML_IS_CONSTANT(src))
    {
        ImlConstant *const_src = IML_CONSTANT(src);

        if (dst_reg == NULL)
        {
            /* use a temporary register  */
            fprintf(out,
                    "    movl $%d, %%"TEMP_REG_NAME"\n"
                    "    movl %%"TEMP_REG_NAME", %d(%%ebp)\n",
                    iml_constant_get_val_32b(const_src),
                    iml_variable_get_frame_offset(dst));
        }
        else
        {
            fprintf(out,
                    "    movl $%d, %%%s\n",
                    iml_constant_get_val_32b(const_src),
                    iml_register_get_name(dst_reg));
        }
    }
    else if (IML_IS_VARIABLE(src))
    {
        iml_register_t *src_reg;

        src_reg = iml_variable_get_register(IML_VARIABLE(src));
        if (src_reg == NULL)
        {
            if (dst_reg == NULL)
            {
                /* offset to offset copy, use temporary register */
                fprintf(out,
                        "    movl %d(%%ebp), %%"TEMP_REG_NAME"\n"
                        "    movl %%"TEMP_REG_NAME", %d(%%ebp)\n",
                        iml_variable_get_frame_offset(IML_VARIABLE(src)),
                        iml_variable_get_frame_offset(dst));
            }
            else
            {
                /* offset to register copy */
                fprintf(out,
                        "    movl %d(%%ebp), %%%s\n",
                        iml_variable_get_frame_offset(IML_VARIABLE(src)),
                        iml_register_get_name(dst_reg));
            }
        }
        else
        {
            if (dst_reg == NULL)
            {
                /* register to offset copy */
                fprintf(out,
                        "    movl %%%s, %d(%%ebp)\n",
                        iml_register_get_name(src_reg),
                        iml_variable_get_frame_offset(dst));
            }
            else
            {
                /* register to register copy */
                fprintf(out,
                        "    movl %%%s, %%%s\n",
                        iml_register_get_name(src_reg),
                        iml_register_get_name(dst_reg));
            }
        }
    }
}

static void
x86_compile_binop(FILE *out, iml_operation_t *op)
{
    ImlOperand *left;
    ImlOperand *right;
    ImlVariable *res;
    iml_register_t *res_reg;
    const gchar *res_reg_name;
    const gchar *op_name;

    left = iml_operation_get_operand(op, 1);
    right = iml_operation_get_operand(op, 2);
    res =  IML_VARIABLE(iml_operation_get_operand(op, 3));
    res_reg = iml_variable_get_register(res);

    switch (iml_operation_get_opcode(op))
    {
        case iml_add:
            op_name = "addl";
            break;
        case iml_sub:
            op_name = "subl";
            break;
        default:
            /* unexpeted opcode */
            assert(false);
    }

    if (res_reg == NULL)
    {
        res_reg_name = TEMP_REG_NAME;
    }
    else
    {
        res_reg_name = iml_register_get_name(res_reg);
    }

    /* store left operand in result register */
    if (IML_IS_CONSTANT(left))
    {
        fprintf(out,
                "    movl $%d, %%%s\n",
                iml_constant_get_val_32b(IML_CONSTANT(left)),
                res_reg_name);
    }
    else
    {
        assert(IML_IS_VARIABLE(left));

        ImlVariable *var = IML_VARIABLE(left);
        iml_register_t *reg = iml_variable_get_register(var);

        if (reg == NULL)
        {
            fprintf(out,
                    "    movl %d(%%ebp), %%%s\n",
                    iml_variable_get_frame_offset(var),
                    res_reg_name);
        }
        else
        {
            fprintf(out,
                    "    movl %%%s, %%%s\n",
                    iml_register_get_name(reg),
                    res_reg_name);
        }
    }

    /* perform the binary operation */
    if (IML_IS_CONSTANT(right))
    {
        fprintf(out,
                "    %s $%d, %%%s\n",
                op_name,
                iml_constant_get_val_32b(IML_CONSTANT(right)),
                res_reg_name);
    }
    else
    {
        assert(IML_IS_VARIABLE(right));

        ImlVariable *var = IML_VARIABLE(right);
        iml_register_t *reg = iml_variable_get_register(var);

        if (reg == NULL)
        {
            fprintf(out,
                    "    %s %d(%%ebp), %%%s\n",
                    op_name,
                    iml_variable_get_frame_offset(var),
                    res_reg_name);
        }
        else
        {
            fprintf(out,
                    "    %s %%%s, %%%s\n",
                    op_name,
                    iml_register_get_name(reg),
                    res_reg_name);
        }
    }

    /* move operation result from temp register to destination if needed */
    if (res_reg == NULL)
    {
        fprintf(out,
                "    movl %%%s, %d(%%ebp)\n",
                res_reg_name,
                iml_variable_get_frame_offset(res));
    }
}

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunctionDef *func_def)
{
    char *func_name;
    iml_func_frame_t *frame;
    GSList *i;
    GSList *regs;
    char return_label[LABEL_MAX_LEN];


    func_name = ir_function_def_get_mangled_name(func_def);
    frame = ir_function_def_get_frame(func_def);

    /* generate function symbol declaration and function entry point label */
    fprintf(params->out,
            ".globl %s\n"
            "    .type %s, @function\n"
            "%s:\n",
            func_name, func_name, func_name);

    /* generate code to allocate function frame on the stack */
    fprintf(params->out,
            "    enter $%d, $0\n",
            iml_func_frame_get_size(frame));

    /* store preserved registers value on the stack */
    regs = iml_func_frame_get_used_regs(frame);
    for (i = regs; i != NULL; i = g_slist_next(i))
    {
        fprintf(params->out,
                "    pushl %%%s\n",
                iml_register_get_name(i->data));
    }


    /* generate this function's return label */
    label_gen_next(&(params->label_gen), return_label);

    /* generate code for all operations in this function */
    for (i = ir_function_get_operations(func_def);
         i != NULL;
         i = g_slist_next(i))
    {
        iml_operation_t *op = (iml_operation_t*) i->data;
        switch (iml_operation_get_opcode(op))
        {
            case iml_return:
                x86_compile_return(params->out, return_label, op);
                break;
            case iml_copy:
                x86_compile_copy(params->out, op);
                break;
            case iml_add:
            case iml_sub:
                x86_compile_binop(params->out, op);
                break;
            default:
                /* unexpected opcode */
                assert(false);
        }
    }

    /*
     * generate code for returning from this function
     */

    /* add return label */
    fprintf(params->out, "%s:\n", return_label);

    /* restore values of preserved registers */
    regs = g_slist_reverse(regs);
    for (i = regs; i != NULL; i = g_slist_next(i))
    {
        fprintf(params->out,
                "    popl %%%s\n",
                iml_register_get_name(i->data));
    }
    fprintf(params->out,
            "    leave\n"
            "    ret\n");
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
