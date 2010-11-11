#include <stdbool.h>
#include <string.h>

#include "types.h"
#include "x86.h"
#include "x86_data.h"
#include "ir_property.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_char_constant.h"
#include "ir_bool_constant.h"
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

#define TEMP_REG1_NAME "ecx"
#define TEMP_REG2_NAME "eax"

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_text_prelude(x86_comp_params_t *params,
                 sym_table_t *sym_table);

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunctionDef *func_def);

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
    ImlVariable *last_param = NULL;

    /* assign locations to parameters */
    params = iml_func_frame_get_parameters(frame);
    if (params != NULL)
    {
        params = g_slist_reverse(g_slist_copy(params));

        /* deal with last parameter later */
        last_param = params->data;

        for (i = g_slist_next(params), offset = 8;
             i != NULL;
             i = g_slist_next(i), offset += 4)
        {
            iml_variable_set_frame_offset(IML_VARIABLE(i->data), offset);
        }

        /* clean-up */
        g_slist_free(params);
    }

    offset = 0;

    /* assign frame offset to last parameter if needed */
    if (last_param != NULL)
    {
        offset -= 4;
        iml_variable_set_frame_offset(last_param, offset);
    }

    /*
     * assign offset location to local variables that
     * have not been assigned a register
     */
    i = iml_func_frame_get_locals(frame, iml_32b);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            offset -= 4;
            iml_variable_set_frame_offset(var, offset);
        }
    }

    i = iml_func_frame_get_locals(frame, iml_16b);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            offset -= 4;
            iml_variable_set_frame_offset(var, offset);
        }
    }

    i = iml_func_frame_get_locals(frame, iml_8b);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            offset -= 4;
            iml_variable_set_frame_offset(var, offset);
        }
    }

    i = iml_func_frame_get_locals(frame, iml_ptr);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            offset -= 4;
            iml_variable_set_frame_offset(var, offset);
        }
    }


    /* assign offset locations to blob variables */
    i = iml_func_frame_get_locals(frame, iml_blob);
    for (; i != NULL; i = g_slist_next(i))
    {
        ImlVariable *var = IML_VARIABLE(i->data);

        if (iml_variable_get_register(var) == NULL)
        {
            offset -= iml_variable_get_size(var);
            iml_variable_set_frame_offset(var, offset);
        }
    }

    iml_func_frame_set_size(frame, (guint)(-(offset)));
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
    params.module = module;
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
        IrFunction *main_func;
        bool exit_code_returned;

        main_func = IR_FUNCTION(main_symb);
      
        /* only entry point without arguments supported */
        assert(ir_function_get_parameters(main_func) == NULL);

        /*
         * if main() returns a value, use it as exit code,
         * otherwise exit with code 0
         */
        exit_code_returned = 
            types_is_int(ir_function_get_return_type(main_func));

        fprintf(params->out,
                ".globl _init\n"
                "_init:\n"
                "    ret\n"
                ".globl main\n"
                "main:\n"
                "    call %s\n"
                "    pushl %s\n"
                "    call exit\n",
                ir_function_get_mangled_name(main_func),
                exit_code_returned ? "%eax" : "$0");
    }
}

/**
 * Generate assembly that will push the operand on the top of the stack.
 */
static void
x86_push_operand(FILE *out, ImlOperand *oper)
{
    assert(IML_IS_OPERAND(oper));

    if (IML_IS_CONSTANT(oper))
    {
        if (iml_operand_get_data_type(oper) == iml_ptr)
        {
            fprintf(out,
                    "    pushl $%s\n",
                    iml_constant_get_val_ptr(IML_CONSTANT(oper)));

        }
        else
        {
            fprintf(out,
                    "    pushl $%d\n",
                    iml_constant_get_val_32b(IML_CONSTANT(oper)));
        }
    }
    else
    {
        assert(IML_IS_VARIABLE(oper));
        ImlVariable *var = IML_VARIABLE(oper);
        iml_register_t *reg = iml_variable_get_register(var);

        if (reg == NULL)
        {
            fprintf(out,
                    "    pushl %d(%%ebp)\n",
                    iml_variable_get_frame_offset(var));
        }
        else
        {
            fprintf(out,
                    "    pushl %%%s\n",
                    iml_register_get_name(reg));
        }
    }
}

/**
 * Generate assembly that will move the operand to a register.
 */
static void
x86_move_to_reg(FILE *out, const char *dest_reg, ImlOperand *oper)
{
    assert(IML_IS_OPERAND(oper));

    if (IML_IS_CONSTANT(oper))
    {
        fprintf(out,
                "    movl $%d, %%%s\n",
                iml_constant_get_val_32b(IML_CONSTANT(oper)),
                dest_reg);
    }
    else
    {
        assert(IML_IS_VARIABLE(oper));
        ImlVariable *var = IML_VARIABLE(oper);
        iml_register_t *reg = iml_variable_get_register(var);

        if (reg == NULL)
        {
            fprintf(out,
                    "    movl %d(%%ebp), %%%s\n",
                    iml_variable_get_frame_offset(var),
                    dest_reg);
        }
        else
        {
            fprintf(out,
                    "    movl %%%s, %%%s\n",
                    iml_register_get_name(reg),
                    dest_reg);
        }
    }
}

/**
 * Generate assembly that will move value in the register
 * to specified variable.
 */
static void
x86_move_from_reg(FILE *out, const char *src_reg, ImlVariable *var)
{
    assert(IML_VARIABLE(var));

    iml_register_t *var_reg = iml_variable_get_register(var);

    if (var_reg == NULL)
    {
        fprintf(out,
                "    movl %%%s, %d(%%ebp)\n",
                src_reg,
                iml_variable_get_frame_offset(var));
    }
    else
    {
        fprintf(out,
                "    movl %%%s, %%%s\n",
                src_reg,
                iml_register_get_name(var_reg));
    }
}

/**
 * Generate assembly that will move operand value to an frame offset.
 */
static void
x86_move_to_offset(FILE *out, guint frame_offset, ImlOperand *oper)
{
    assert(IML_IS_OPERAND(oper));

    if (IML_IS_CONSTANT(oper))
    {
        /* not imlemented */
        assert(false);
    }
    else
    {
        assert(IML_IS_VARIABLE(oper));
        ImlVariable *var = IML_VARIABLE(oper);
        iml_register_t *reg = iml_variable_get_register(var);

        if (reg == NULL)
        {
            fprintf(out,
                    "    movl %d(%%ebp), %%" TEMP_REG1_NAME "\n"
                    "    movl %%" TEMP_REG1_NAME ", %d(%%ebp)\n",
                    iml_variable_get_frame_offset(var),
                    frame_offset);
        }
        else
        {
            assert(false); /* code below not tested */
            fprintf(out,
                    "    movl %%%s, %d(%%ebp)\n",
                    iml_register_get_name(reg), frame_offset);

        }
    }
}

static void
x86_compile_return(FILE *out, const char *return_label, iml_operation_t *op)
{
    ImlOperand *ret_value = iml_operation_get_operand(op, 1);

    if (ret_value != NULL)
    {
        x86_move_to_reg(out, "eax", iml_operation_get_operand(op, 1));
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
                    "    movl $%d, %%"TEMP_REG1_NAME"\n"
                    "    movl %%"TEMP_REG1_NAME", %d(%%ebp)\n",
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
                        "    movl %d(%%ebp), %%"TEMP_REG1_NAME"\n"
                        "    movl %%"TEMP_REG1_NAME", %d(%%ebp)\n",
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
            x86_move_from_reg(out,
                              iml_register_get_name(src_reg),
                              dst);
        }
    }
}

static void
x86_compile_mset(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_mset);

    ImlOperand *src = iml_operation_get_operand(op, 1);
    guint num = GPOINTER_TO_UINT(iml_operation_get_operand(op, 2));
    ImlVariable *dest = iml_operation_get_operand(op, 3);
    const gchar *op_suffix;

    /* only mset to blob variables implemented so far */
    assert(iml_variable_get_data_type(dest) == iml_blob);

    switch (iml_operand_get_data_type(src))
    {
        case iml_8b:
            op_suffix = "b";
            break;
        case iml_16b:
            op_suffix = "w";
            break;
        case iml_32b:
            op_suffix = "l";
            break;
        default:
            /* unexpected operand data type */
            assert(false);
    }

    x86_move_to_reg(out, "eax", src);

    fprintf(out,
            "    pushl %%edi\n"
            "    movl $%d, %%ecx\n"
            "    leal %d(%%ebp), %%edi\n"
            "    cld\n"
            "    rep stos%s\n"
            "    popl %%edi\n",
            num,
            iml_variable_get_frame_offset(dest),
            op_suffix);
}

static void
x86_compile_setfld(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_setfld);

    ImlOperand *src = iml_operation_get_operand(op, 1);
    ImlVariable *dest = iml_operation_get_operand(op, 2);
    ImlOperand *index = iml_operation_get_operand(op, 3);
    guint size = GPOINTER_TO_UINT(iml_operation_get_operand(op, 4));
    const char *index_reg;

    /*
     * Generate code that will store array index into a register,
     * remember which register it ends up in
     */
    if (IML_IS_CONSTANT(index))
    {
        fprintf(out,
                "    movl $%d, %%" TEMP_REG1_NAME "\n",
                iml_constant_get_val_32b(IML_CONSTANT(index)));
        index_reg = TEMP_REG1_NAME;
    }
    else
    {
        assert(IML_IS_VARIABLE(index));
        ImlVariable *var = IML_VARIABLE(index);
        iml_register_t *reg = iml_variable_get_register(var);

        if (reg != NULL)
        {
            index_reg = iml_register_get_name(reg);
        }
        else
        {
            fprintf(out,
                    "    movl %d(%%ebp), %%" TEMP_REG1_NAME "\n",
                    iml_variable_get_frame_offset(var));
            index_reg = TEMP_REG1_NAME;
        }
    }

    /*
     * generate the move to the array cell
     */
    if (IML_IS_CONSTANT(src))
    {
        fprintf(out,
                "    movl $%d, %d(%%ebp, %%%s, %u)\n",
                iml_constant_get_val_32b(IML_CONSTANT(src)),
                iml_variable_get_frame_offset(dest),
                index_reg,
                size);
    }
    else
    {
        assert(IML_IS_VARIABLE(src));

        ImlVariable *var = IML_VARIABLE(src);
        iml_register_t *reg = iml_variable_get_register(var);
        const gchar *src_reg;

        if (reg != NULL)
        {
            src_reg = iml_register_get_name(reg);
        }
        else
        {
            fprintf(out,
                    "    movl %d(%%ebp), %%" TEMP_REG2_NAME "\n",
                    iml_variable_get_frame_offset(var));
            src_reg = TEMP_REG2_NAME;
        }

        fprintf(out,
                "    movl %%%s, %d(%%ebp, %%%s, %u)\n",
                src_reg,
                iml_variable_get_frame_offset(dest),
                index_reg,
                size);
    }
}

static void
x86_compile_getfld(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_getfld);

    ImlVariable *src = iml_operation_get_operand(op, 1);
    ImlOperand *index = iml_operation_get_operand(op, 2);
    guint size = GPOINTER_TO_UINT(iml_operation_get_operand(op, 3));
    ImlVariable *dest = iml_operation_get_operand(op, 4);
    const char *index_reg = NULL;
    iml_register_t *dest_reg = iml_variable_get_register(dest);

    if (IML_IS_CONSTANT(index))
    {
        fprintf(out,
                "    movl $%d, %%" TEMP_REG1_NAME "\n",
                iml_constant_get_val_32b(IML_CONSTANT(index)));
        index_reg = TEMP_REG1_NAME;
    }
    else
    {
        assert(IML_IS_VARIABLE(index));
        ImlVariable *var = IML_VARIABLE(index);
        iml_register_t *reg = iml_variable_get_register(var);

        if (reg != NULL)
        {
            index_reg = iml_register_get_name(reg);
        }
        else
        {
            fprintf(out,
                    "    movl %d(%%ebp), %%" TEMP_REG1_NAME "\n",
                    iml_variable_get_frame_offset(var));
            index_reg = TEMP_REG1_NAME;
        }
    }

    assert(index_reg != NULL);

    if (dest_reg != NULL)
    {
        fprintf(out,
                "    movl %d(%%ebp, %%%s, %u), %%%s\n",
                iml_variable_get_frame_offset(src),
                index_reg,
                size,
                iml_register_get_name(dest_reg));
    }
    else
    {
        fprintf(out,
                "    movl %d(%%ebp, %%%s, %u), %%" TEMP_REG1_NAME "\n"
                "    movl %%" TEMP_REG1_NAME ", %d(%%ebp)\n",
                iml_variable_get_frame_offset(src),
                index_reg,
                size,
                iml_variable_get_frame_offset(dest));
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
        case iml_and:
            op_name = "andl";
            break;
        case iml_or:
            op_name = "orl";
            break;
        default:
            /* unexpeted opcode */
            assert(false);
    }

    if (res_reg == NULL)
    {
        res_reg_name = TEMP_REG1_NAME;
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
x86_compile_mult(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_smult ||
           iml_operation_get_opcode(op) == iml_umult);
    x86_move_to_reg(out, "eax", iml_operation_get_operand(op, 1));
    x86_move_to_reg(out, TEMP_REG1_NAME, iml_operation_get_operand(op, 2));
    fprintf(out,
            "    %smul %%" TEMP_REG1_NAME "\n",
            iml_operation_get_opcode(op) == iml_smult ? "i" : "");
    x86_move_from_reg(out, "eax", iml_operation_get_operand(op, 3));
}

/**
 * Generate assembly for ineg and bneg operations.
 */
static void
x86_compile_neg(FILE *out, iml_operation_t *op)
{
    assert(op);

    ImlOperand *oper;
    ImlVariable *res;
    iml_register_t *res_reg;
    const char *instr;

    switch (iml_operation_get_opcode(op))
    {
        case iml_ineg:
            instr = "negl ";
            break;
        case iml_bneg:
            instr = "xor $1,";
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }

    oper = IML_OPERAND(iml_operation_get_operand(op, 1));
    res = IML_VARIABLE(iml_operation_get_operand(op, 2));

    res_reg = iml_variable_get_register(res);
    if (res_reg == NULL) {
        x86_move_to_offset(out,
                           iml_variable_get_frame_offset(res),
                           oper);
        fprintf(out,
                "    %s %d(%%ebp)\n",
                instr,
                iml_variable_get_frame_offset(res));
    }
    else
    {
        x86_move_to_reg(out, iml_register_get_name(res_reg), oper);
        fprintf(out, "    %s %%%s\n", instr, iml_register_get_name(res_reg));
    }
}

/**
 * Generate assembly for integer comparison operations
 * (iml_equal and iml_nequal)
 */
static void
x86_compile_icmp(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_equal ||
           iml_operation_get_opcode(op) == iml_nequal ||
           iml_operation_get_opcode(op) == iml_sless ||
           iml_operation_get_opcode(op) == iml_uless ||
           iml_operation_get_opcode(op) == iml_slesseq ||
           iml_operation_get_opcode(op) == iml_ulesseq ||
           iml_operation_get_opcode(op) == iml_sgreater ||
           iml_operation_get_opcode(op) == iml_ugreater ||
           iml_operation_get_opcode(op) == iml_sgreatereq ||
           iml_operation_get_opcode(op) == iml_ugreatereq);



    ImlOperand *arg2 = iml_operation_get_operand(op, 1);
    ImlOperand *arg1 = iml_operation_get_operand(op, 2);
    ImlVariable *res = iml_operation_get_operand(op, 3);
    const char *arg1_reg = NULL;
    const char *arg2_reg = NULL;
    iml_register_t *res_reg;
    const char *set_suffix;

/*
 * cmpl operations syntax is:
 *
 * cmpl arg1, arg2
 *
 * cmpl will perform "arg2 - arg1" and set EFLAGS
 *
 * Possible operands combinations.
 *
 *    r register
 *    m memory
 *    c constant
 *
 *            arg2
 *       |  r   m   c
 *     ---------------
 * a   r | r r r m r c
 * r     |
 * g   m | m r m m m c
 * 1     |
 *     c | c r c m n/a
 *
 * Usage of temporary register for arg1 operand.
 *
 *    r r
 *    r m
 *    r c - move arg2 to temp reg => r r
 *    m r
 *    m m - move arg2 to temp reg => m r
 *    m c - move arg2 to temp reg => m r
 *    c r
 *    c m
 */

    /*
     * figure out if arg1 operand is stored in register and
     * which one if it is
     */
    if (IML_IS_VARIABLE(arg1))
    {
        iml_register_t *reg;

        reg = iml_variable_get_register(IML_VARIABLE(arg1));
        if (reg != NULL)
        {
            arg1_reg = iml_register_get_name(reg);
        }
    }

    /*
     * Store arg2 operand in temporary register if needed.
     * Save the name of the register where arg2 operand is stored, if any.
     */
    if (IML_IS_CONSTANT(arg2))
    {
        fprintf(out,
                "    movl $%d, %%" TEMP_REG1_NAME "\n",
                iml_constant_get_val_32b(IML_CONSTANT(arg2)));
        arg2_reg = TEMP_REG1_NAME;
    }
    else
    {
        assert(IML_IS_VARIABLE(arg2));
        iml_register_t *reg;

        reg = iml_variable_get_register(IML_VARIABLE(arg2));
        if (reg != NULL)
        {
            arg2_reg = iml_register_get_name(reg);
        }
        else if (arg1_reg == NULL)
        {
            fprintf(out,
                    "    movl %d(%%ebp), %%" TEMP_REG1_NAME "\n",
                    iml_variable_get_frame_offset(IML_VARIABLE(arg2)));
            arg2_reg = TEMP_REG1_NAME;
        }
    }

    /*
     * Compare arg2 and arg1 operands.
     * Generate appropriate assembly depending on how operands are stored.
     */
    if (arg1_reg != NULL && arg2_reg != NULL)
    {
        fprintf(out,
                "    cmpl %%%s, %%%s\n",
                arg1_reg, arg2_reg);
    }
    else if (arg1_reg != NULL)
    {
        assert(arg2_reg == NULL);
        fprintf(out,
                "    cmpl %%%s, %d(%%ebp)\n",
                arg1_reg,
                iml_variable_get_frame_offset(IML_VARIABLE(arg2)));
    }
    else if (IML_IS_CONSTANT(arg1))
    {
        assert(arg2_reg != NULL);
        fprintf(out,
                "    cmpl $%d, %%%s\n",
                iml_constant_get_val_32b(IML_CONSTANT(arg1)),
                arg2_reg);
    }
    else
    {
        assert(arg1_reg == NULL);
        assert(arg2_reg != NULL);
        fprintf(out,
                "    cmpl %d(%%ebp), %%%s\n",
                iml_variable_get_frame_offset(IML_VARIABLE(arg1)),
                arg2_reg);
    }

    /* pick set assembly operation suffix depending on iml operation */
    switch (iml_operation_get_opcode(op))
    {
        case iml_equal:
            set_suffix = "e";
            break;
        case iml_nequal:
            set_suffix = "ne";
            break;
        case iml_sless:
            set_suffix = "l";
            break;
        case iml_uless:
            set_suffix = "b";
            break;
        case iml_slesseq:
            set_suffix = "le";
            break;
        case iml_ulesseq:
            set_suffix = "be";
            break;
        case iml_sgreater:
            set_suffix = "g";
            break;
        case iml_ugreater:
            set_suffix = "a";
            break;
        case iml_sgreatereq:
            set_suffix = "ge";
            break;
        case iml_ugreatereq:
            set_suffix = "ae";
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }

    /* set cl register depending on the results of cmp operation */
    fprintf(out,
            "    set%s %%cl\n",
            set_suffix);

    /*
     * move result to destination variable
     */
    res_reg = iml_variable_get_register(res);
    if (res_reg != NULL)
    {
        fprintf(out,
                "    movsx %%cl, %%%s\n",
                iml_register_get_name(res_reg));
    }
    else
    {
        fprintf(out,
                "    movsx %%cl, %%ecx\n"
                "    movl %%ecx, %d(%%ebp)\n",
                iml_variable_get_frame_offset(res));
    }
}

static void
x86_compile_jmpcond(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_jmpneq);

    ImlConstant *arg1 = iml_operation_get_operand(op, 2);
    ImlVariable *arg2 = iml_operation_get_operand(op, 1);
    const gchar *label = iml_operation_get_operand(op, 3);
    iml_register_t *reg;

    /*
     * only variables as left operand and constant as right operand
     * are supported at the moment
     */
    assert(IML_IS_CONSTANT(arg1));
    assert(IML_IS_VARIABLE(arg2));


    reg = iml_variable_get_register(arg2);
    if (reg == NULL)
    {
        fprintf(out,
                "    cmpl $%u, %d(%%ebp)\n",
                iml_constant_get_val_8b(arg1),
                iml_variable_get_frame_offset(arg2));
    }
    else
    {
        fprintf(out,
                "    cmpl $%u, %%%s\n",
                iml_constant_get_val_8b(arg1),
                iml_register_get_name(reg));
    }

    fprintf(out, "    jne %s\n", label);
}

/**
 * Generate assembly for passing arguments to a D function.
 *
 * @return number of bytes used by the arguments on the stack
 */
static guint
x86_compile_dcall_args(FILE *out, GSList *arguments)
{
    GSList *i;
    guint args_stack_size = 0;

    for (i = arguments; i != NULL; i = g_slist_next(i))
    {
        if (g_slist_next(i) != NULL)
        {
            x86_push_operand(out, IML_OPERAND(i->data));
            args_stack_size += 4;
        }
        else
        {
            /* store last argument in eax */
            x86_move_to_reg(out, "eax", IML_OPERAND(i->data));
        }
    }

    return args_stack_size;
}

/**
 * Generate assembly for passing arguments to a C function.
 *
 * @return number of bytes used by the arguments on the stack
 */
static guint
x86_compile_ccall_args(FILE *out, GSList *arguments)
{
    GSList *args;
    GSList *i;
    guint args_stack_size = 0;

    /*
     * in x86 c calling convention, function arguments are
     * pushed in reversed order, so we need to make copy of
     * arguments list and reverse it
     */
    args = g_slist_copy(arguments);
    args = g_slist_reverse(arguments);

    for (i = args; i != NULL; i = g_slist_next(i))
    {
      x86_push_operand(out, IML_OPERAND(i->data));
      args_stack_size += 4;
    }
    g_slist_free(args);

    return args_stack_size;
}

static void
x86_compile_call(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_call ||
            iml_operation_get_opcode(op) == iml_call_c);

    GSList *args = iml_operation_get_operand(op, 2);
    ImlVariable *res;
    guint args_stack_size = 0;

    /*
     * generate assembly for passing arguments according
     * to calling convention
     */
    switch (iml_operation_get_opcode(op))
    {
        case iml_call:
            args_stack_size = x86_compile_dcall_args(out, args);
            break;
        case iml_call_c:
            args_stack_size = x86_compile_ccall_args(out, args);
            break;
        default:
            /* unexpected operand */
            assert(false);
    }

    fprintf(out,
            "    call %s\n",
            (char*)iml_operation_get_operand(op, 1));

    /* generate code to remove call arguments from the stack if needed */
    if (args_stack_size > 0)
    {
        fprintf(out,
                "    addl $%u, %%esp\n",
                args_stack_size);

    }

    res = iml_operation_get_operand(op, 3);
    if (res != NULL)
    {
        x86_move_from_reg(out, "eax", res);
    }
}

static void
x86_compile_function_def(x86_comp_params_t *params, IrFunctionDef *func_def)
{
    char *func_name;
    iml_func_frame_t *frame;
    GSList *i;
    GSList *regs;
    char *return_label;


    func_name = ir_function_get_mangled_name(IR_FUNCTION(func_def));
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

    /* generate the code to store last parameter in stack frame if needed */
    i = g_slist_last(iml_func_frame_get_parameters(frame));
    if (i != NULL)
    {
        fprintf(params->out,
                "    movl %%eax, %d(%%ebp)\n",
                iml_variable_get_frame_offset(i->data));
    }

    /* store preserved registers value on the stack */
    regs = iml_func_frame_get_used_regs(frame);
    for (i = regs; i != NULL; i = g_slist_next(i))
    {
        fprintf(params->out,
                "    pushl %%%s\n",
                iml_register_get_name(i->data));
    }


    /* generate this function's return label */
    return_label = ir_module_gen_label(params->module);

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
            case iml_cast:
                /*
                 * currently variables of all supported iml types are
                 * stored as 32-bit integers,
                 * thus iml_cast is equivalent to iml_copy operation
                 */
                x86_compile_copy(params->out, op);
                break;
            case iml_mset:
                x86_compile_mset(params->out, op);
                break;
            case iml_setfld:
                x86_compile_setfld(params->out, op);
                break;
            case iml_getfld:
                x86_compile_getfld(params->out, op);
                break;
            case iml_add:
            case iml_sub:
            case iml_and:
            case iml_or:
                x86_compile_binop(params->out, op);
                break;
            case iml_umult:
            case iml_smult:
                x86_compile_mult(params->out, op);
                break;
            case iml_ineg:
            case iml_bneg:
                x86_compile_neg(params->out, op);
                break;
            case iml_equal:
            case iml_nequal:
            case iml_sless:
            case iml_uless:
            case iml_sgreater:
            case iml_ugreater:
            case iml_slesseq:
            case iml_ulesseq:
            case iml_sgreatereq:
            case iml_ugreatereq:
                x86_compile_icmp(params->out, op);
                break;
            case iml_jmp:
                fprintf(params->out,
                        "    jmp %s\n",
                        (char*)iml_operation_get_operand(op, 1));
                break;
            case iml_jmpneq:
                x86_compile_jmpcond(params->out, op);
                break;
            case iml_call:
            case iml_call_c:
                x86_compile_call(params->out, op);
                break;
            case iml_label:
                fprintf(params->out,
                        "%s:\n",
                        (char*)iml_operation_get_operand(op, 1));
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
