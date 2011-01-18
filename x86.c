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
#include "ir_assignment.h"
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
#define TEMP_REG2_BYTE_NAME "al"

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

/**
 * @return true if the param should be stored in
 *         eax register when used as last argument
 */
static bool
x86_param_stored_in_reg(ImlVariable *param)
{
    guint size;

    if (iml_operand_get_data_type(IML_OPERAND(param)) != iml_blob)
    {
        return true;
    }

    size = iml_variable_get_size(param);

    return size < 4 && size != 3;
}

static void
x86_assign_var_locations(iml_func_frame_t *frame)
{
    GSList *parameters;
    GSList *i;
    gint offset = 0;

    /* assign locations to parameters */
    parameters = iml_func_frame_get_parameters(frame);
    if (parameters != NULL)
    {
        ImlVariable *param;
        guint params_offset = 8;
        parameters = g_slist_reverse(g_slist_copy(parameters));

        /*
         * deal with last parameter
         */
        param = parameters->data;
        if (x86_param_stored_in_reg(param))
        {
            /*
             * last parameter is passed via eax
             * and will be stored in the stack frame
             */
            offset -= 4;
            iml_variable_set_frame_offset(param, offset);
        }
        else
        {
            /* last parameter is passed on stack */
            iml_variable_set_frame_offset(param, params_offset);
            params_offset += iml_variable_get_size(param);
        }

        for (i = g_slist_next(parameters);
             i != NULL;
             i = g_slist_next(i))
        {
            param = parameters->data;
            iml_variable_set_frame_offset(IML_VARIABLE(i->data), params_offset);
            params_offset += iml_variable_get_size(param);
        }

        /* clean-up */
        g_slist_free(parameters);
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
    GList *data_section;

    params.out = out_stream;
    params.module = module;
    global_sym_table = ir_module_get_symbols(module);

    fprintf(out_stream,
            "    .file \"%s\"\n",
            source_file);

    data_section = ir_module_get_data_section(module);
    x86_gen_data_section(&params, data_section);
    g_list_free(data_section);

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
 *
 * @return number of bytes pushed on stack
 */
static guint
x86_push_operand(FILE *out, ImlOperand *oper)
{
    assert(IML_IS_OPERAND(oper));

    guint size = 4;

    if (iml_is_constant(oper))
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
        assert(iml_is_variable(oper));
        ImlVariable *var = IML_VARIABLE(oper);
        iml_register_t *reg = iml_variable_get_register(var);

        size = iml_variable_get_size(var);

        if (size > 4)
        {
            /* variables bigger then 4 bytes should not be in a register */
            assert(reg == NULL);

            /* push blob on stack, 4 bytes at a time, in reversed order */

            gint i;
            gint offset = iml_variable_get_frame_offset(var);

            i = offset + ((gint)size - 4);

            for (i = offset + ((gint)size - 4); i >= offset; i -= 4)
            {
                fprintf(out,
                        "    pushl %d(%%ebp)\n",
                        i);
            }
        }
        else if (reg == NULL)
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

    return size;
}

/**
 * Generate assembly that will move the operand to a register.
 */
static void
x86_move_to_reg(FILE *out, const char *dest_reg, ImlOperand *oper)
{
    assert(IML_IS_OPERAND(oper));

    if (iml_is_constant(oper))
    {
        fprintf(out,
                "    movl $%d, %%%s\n",
                iml_constant_get_val_32b(IML_CONSTANT(oper)),
                dest_reg);
    }
    else
    {
        assert(iml_is_variable(oper));
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

    if (iml_is_constant(oper))
    {
        /* not imlemented */
        assert(false);
    }
    else
    {
        assert(iml_is_variable(oper));
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
        x86_move_to_reg(out, "eax", ret_value);
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

    if (iml_is_constant(src))
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
    else if (iml_is_variable(src))
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
x86_compile_setelm_blob(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_setelm);

    ImlOperand *src = iml_operation_get_operand(op, 1);
    ImlVariable *dest = iml_operation_get_operand(op, 2);
    ImlOperand *index = iml_operation_get_operand(op, 3);
    guint size = GPOINTER_TO_UINT(iml_operation_get_operand(op, 4));
    const char *index_reg;
    const gchar *move_suffix;

    assert(iml_operand_get_data_type(IML_OPERAND(dest)) == iml_blob);

    /*
     * Generate code that will store array index into a register,
     * remember which register it ends up in
     */
    if (iml_is_constant(index))
    {
        fprintf(out,
                "    movl $%d, %%" TEMP_REG1_NAME "\n",
                iml_constant_get_val_32b(IML_CONSTANT(index)));
        index_reg = TEMP_REG1_NAME;
    }
    else
    {
        assert(iml_is_variable(index));
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

    /* pick appropriate move instruction suffix based in move size */
    switch (size)
    {
        case 4:
            move_suffix = "l"; /* move 32-bit value */
            break;
        case 1:
            move_suffix = "b"; /* move 8-bit value */
            break;
        default:
            /* unexpected/unsupported size */
            assert(false);
    }

    /*
     * generate the move to the array cell
     */
    if (iml_is_constant(src))
    {
        fprintf(out,
                "    mov%s $%d, %d(%%ebp, %%%s, %u)\n",
                move_suffix,
                iml_constant_get_val_32b(IML_CONSTANT(src)),
                iml_variable_get_frame_offset(dest),
                index_reg,
                size);
    }
    else
    {
        assert(iml_is_variable(src));

        const gchar *src_reg;

        switch (size)
        {
            case 4:
                src_reg = TEMP_REG2_NAME;
                break;
            case 1:
                src_reg = TEMP_REG2_BYTE_NAME;
                break;
            default:
                assert(false);
        }

        x86_move_to_reg(out, TEMP_REG2_NAME, src);
        fprintf(out,
                "    mov%s %%%s, %d(%%ebp, %%%s, %u)\n",
                move_suffix,
                src_reg,
                iml_variable_get_frame_offset(dest),
                index_reg,
                size);
    }
}

static void
x86_compile_setelm_ptr(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_setelm);

    ImlOperand *src = iml_operation_get_operand(op, 1);
    ImlVariable *dest = iml_operation_get_operand(op, 2);
    ImlOperand *index = iml_operation_get_operand(op, 3);
    guint size = GPOINTER_TO_UINT(iml_operation_get_operand(op, 4));
    iml_register_t *reg;
    const gchar *move_suffix;

    assert(iml_operand_get_data_type(IML_OPERAND(dest)) == iml_ptr);

    /* move index value to a temp register */
    x86_move_to_reg(out, TEMP_REG1_NAME, index);

    /* multiply array index with element size to get array offset in bytes */
    if (size > 1)
    {
        /* only array element of 1 or 4 bytes supported for now */
        assert(size == 4);
        fprintf(out, "    sall $%d, %%%s\n", size / 2, TEMP_REG1_NAME);
    }

    /* add dest pointer with array offset */
    reg = iml_variable_get_register(dest);
    if (reg == NULL)
    {
        fprintf(out,
                "    addl %d(%%ebp), %%" TEMP_REG1_NAME "\n",
                iml_variable_get_frame_offset(dest));
    }
    else
    {
        fprintf(out,
                "    addl %%%s, %%" TEMP_REG1_NAME "\n",
                iml_register_get_name(reg));
    }

    /* pick appropriate move instruction suffix based in move size */
    switch (size)
    {
        case 4:
            move_suffix = "l"; /* move 32-bit value */
            break;
        case 1:
            move_suffix = "b"; /* move 8-bit value */
            break;
        default:
            /* unexpected/unsupported size */
            assert(false);
    }

    /*
     * move src value to calculated address
     */
    if (iml_is_constant(src))
    {
        fprintf(out,
                "    mov%s $%d, (%%" TEMP_REG1_NAME ")\n",
                move_suffix, iml_constant_get_val_32b(IML_CONSTANT(src)));
    }
    else
    {
        assert(iml_is_variable(src));

        const gchar *src_reg_name;

        switch (size)
        {
            case 4:
                src_reg_name = TEMP_REG2_NAME;
                break;
            case 1:
                src_reg_name = TEMP_REG2_BYTE_NAME;
                break;
            default:
                assert(false);
        }

        x86_move_to_reg(out, TEMP_REG2_NAME, src);

        fprintf(out,
                "    mov%s %%%s, (%%" TEMP_REG1_NAME ")\n",
                move_suffix, src_reg_name);
    }
}

static void
x86_compile_set(FILE *out, iml_operation_t *op)
{
    assert(out);
    assert(op && iml_operation_get_opcode(op) == iml_set);

    ImlOperand *src = iml_operation_get_operand(op, 1);
    ImlVariable *addr = iml_operation_get_operand(op, 2);
    iml_register_t *reg;
    const char *mov_suffix;
    const char *addr_reg;

    /* offset parameter not implemented */
    assert(iml_operation_get_operand(op, 3) == NULL);

    switch (iml_operand_get_data_type(src))
    {
        case iml_32b:
            mov_suffix = "l";
            break;
        case iml_8b:
            mov_suffix = "b";
            break;
        default:
            /* unexpected data type */
            assert(false);
    }

    reg = iml_variable_get_register(addr);
    if (reg == NULL)
    {
        x86_move_to_reg(out, TEMP_REG1_NAME, IML_OPERAND(addr));
        addr_reg = TEMP_REG1_NAME;
    }
    else
    {
        addr_reg = iml_register_get_name(reg);
    }

    if (iml_is_constant(src))
    {
        fprintf(out,
                "    mov%s $%d, (%%%s)\n",
                mov_suffix,
                iml_constant_get_val_32b(IML_CONSTANT(src)),
                addr_reg);
    }
    else
    {
        assert(iml_is_variable(src));
        const char *src_reg;

        if (iml_operand_get_data_type(src) == iml_8b)
        {
            /*
             * handle the special case of 8-bit write.
             * - zero out eax
             * - move src variable to eax
             * - use lowerst 8-bits in eax (al) for the write operation
             */
            fprintf(out,
                    "    xor %%%s, %%%s\n",
                    TEMP_REG2_NAME,
                    TEMP_REG2_NAME);
            x86_move_to_reg(out, TEMP_REG2_NAME, IML_OPERAND(src));
            src_reg = TEMP_REG2_BYTE_NAME;
        }
        else
        {
            reg = iml_variable_get_register(IML_VARIABLE(src));
            if (reg == NULL)
            {
               x86_move_to_reg(out, TEMP_REG2_NAME, IML_OPERAND(src));
               src_reg = TEMP_REG2_NAME;
            }
            else
            {
               src_reg = iml_register_get_name(reg);
            }
        }

        fprintf(out,
                "    mov%s %%%s, (%%%s)\n",
                mov_suffix, src_reg, addr_reg);
    }
}


static void
x86_compile_setelm(FILE *out, iml_operation_t *op)
{
    ImlVariable *dest = iml_operation_get_operand(op, 2);

    if (iml_operand_get_data_type(IML_OPERAND(dest)) == iml_ptr)
    {
        /* setfld with pointer destination variable */
        x86_compile_setelm_ptr(out, op);
    }
    else
    {
        assert(iml_operand_get_data_type(IML_OPERAND(dest)) == iml_blob);

        /* setfld with blob destination variable */
        x86_compile_setelm_blob(out, op);
    }
}

static void
x86_compile_getelm(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_getelm);

    ImlVariable *src = iml_operation_get_operand(op, 1);
    ImlOperand *index = iml_operation_get_operand(op, 2);
    guint size = GPOINTER_TO_UINT(iml_operation_get_operand(op, 3));
    ImlVariable *dest = iml_operation_get_operand(op, 4);
    const char *base_reg;
    gint offset;
    const char *index_reg = NULL;
    iml_register_t *dest_reg = iml_variable_get_register(dest);
    const char *move_inst;

    /*
     * make sure index is stored in a register
     */
    if (iml_is_constant(index))
    {
        fprintf(out,
                "    movl $%d, %%" TEMP_REG1_NAME "\n",
                iml_constant_get_val_32b(IML_CONSTANT(index)));
        index_reg = TEMP_REG1_NAME;
    }
    else
    {
        assert(iml_is_variable(index));
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

    /*
     * figure out base register and offset depending on
     * the type of source operand
     */
    if (iml_operand_get_data_type(IML_OPERAND(src)) == iml_ptr)
    {
        /* getfld with pointer source variable */
        iml_register_t *reg;


        /* move the pointer to register if needed, and remember that register */
        reg = iml_variable_get_register(src);
        if (reg == NULL)
        {
            /* move the pointer to a register */
            x86_move_to_reg(out, TEMP_REG2_NAME, IML_OPERAND(src));
            base_reg = TEMP_REG2_NAME;
        }
        else
        {
            /* already in register */
            base_reg = iml_register_get_name(reg);
        }
        offset = 0;
    }
    else
    {
        assert(iml_operand_get_data_type(IML_OPERAND(src)) == iml_blob);

        /* getfld with blob source variable */

        base_reg = "ebp";
        offset = iml_variable_get_frame_offset(src);
    }

    /* pick move instruction to use, depending on source size */
    switch (size)
    {
        case 4:
            move_inst = "movl";   /* 32bit to 32bit move */
            break;
        case 1:
            move_inst = "movzbl"; /* zero extend 8bit to 32bit */
            break;
        default:
            /* unexpected/unsupported move size */
            assert(false);
    }

    if (dest_reg != NULL)
    {
        fprintf(out,
                "    %s %d(%%%s, %%%s, %u), %%%s\n",
                move_inst,
                offset,
                base_reg,
                index_reg,
                size,
                iml_register_get_name(dest_reg));
    }
    else
    {
        fprintf(out,
                "    %s %d(%%%s, %%%s, %u), %%" TEMP_REG1_NAME "\n"
                "    movl %%" TEMP_REG1_NAME ", %d(%%ebp)\n",
                move_inst,
                offset,
                base_reg,
                index_reg,
                size,
                iml_variable_get_frame_offset(dest));
    }

}

static void
x86_compile_getaddr(FILE *out, iml_operation_t *op)
{
    assert(out);
    assert(iml_operation_get_opcode(op) == iml_getaddr);

    ImlVariable *blob = IML_VARIABLE(iml_operation_get_operand(op, 1));
    ImlVariable *ptr = IML_VARIABLE(iml_operation_get_operand(op, 2));
    iml_register_t *reg;
    const gchar *reg_name;


    assert(iml_variable_get_data_type(blob) ==  iml_blob);
    assert(iml_variable_get_data_type(ptr) ==  iml_ptr);

    /* check if we need to use a temporary register */
    reg = iml_variable_get_register(ptr);
    if (reg != NULL)
    {
        reg_name = iml_register_get_name(reg);
    }
    else
    {
        reg_name = TEMP_REG1_NAME;
    }


    /* generate code the code for storing blob address in to a register */
    fprintf(out,
            "    lea %d(%%ebp), %%%s\n",
            iml_variable_get_frame_offset(blob),
            reg_name);

    /* move address to result pointer variable if needed */
    if (reg == NULL)
    {
        x86_move_from_reg(out, reg_name, ptr);
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
    if (iml_is_constant(left))
    {
        fprintf(out,
                "    movl $%d, %%%s\n",
                iml_constant_get_val_32b(IML_CONSTANT(left)),
                res_reg_name);
    }
    else
    {
        assert(iml_is_variable(left));

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
    if (iml_is_constant(right))
    {
        fprintf(out,
                "    %s $%d, %%%s\n",
                op_name,
                iml_constant_get_val_32b(IML_CONSTANT(right)),
                res_reg_name);
    }
    else
    {
        assert(iml_is_variable(right));

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

static void
x86_compile_divmod(FILE *out, iml_operation_t *op)
{
    assert(op);
    /* instruction to use for division */
    const char *inst;
    /* instruction to use for setting up edx register before division */
    const char *init_edx;
    const char *res_reg;

    iml_opcode_t opcode = iml_operation_get_opcode(op);
    ImlOperand *right = iml_operation_get_operand(op, 2);
    ImlVariable *res = IML_VARIABLE(iml_operation_get_operand(op, 3));

    switch (opcode)
    {
        case iml_sdiv:
        case iml_smod:
            inst = "idiv";        /* signed division or modulo operation */
            init_edx = "cltd";    /* sign extend eax -> edx:eax */
            break;
        case iml_udiv:
        case iml_umod:
            inst = "div";         /* unsigned division or modulo operation  */
            init_edx = "xor %edx, %edx";  /* zero out edx */
            break;
        default:
            /* unexpected iml opcode */
            assert(false);
    }

    if (opcode == iml_sdiv || opcode == iml_udiv)
    {
        res_reg = "eax";
    }
    else
    {
        assert(opcode == iml_smod || opcode == iml_umod);
        res_reg = "edx";
    }

    x86_move_to_reg(out, "eax", iml_operation_get_operand(op, 1));

    fprintf(out, "    %s\n", init_edx);

    if (iml_is_constant(right))
    {
        x86_move_to_reg(out, TEMP_REG1_NAME, right);
        fprintf(out,
                "    %s %%" TEMP_REG1_NAME "\n",
                inst);
    }
    else
    {
        assert(iml_is_variable(right));
        ImlVariable *var = IML_VARIABLE(right);

        iml_register_t *reg = iml_variable_get_register(var);
        if (reg != NULL)
        {
            fprintf(out,
                    "    %s %%%s\n",
                    inst,
                    iml_register_get_name(reg));
        }
        else
        {
            fprintf(out,
                    "    %sl %d(%%ebp)\n",
                    inst,
                    iml_variable_get_frame_offset(var));
        }
    }

    x86_move_from_reg(out, res_reg, res);
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
    if (iml_is_variable(arg1))
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
    if (iml_is_constant(arg2))
    {
        fprintf(out,
                "    movl $%d, %%" TEMP_REG1_NAME "\n",
                iml_constant_get_val_32b(IML_CONSTANT(arg2)));
        arg2_reg = TEMP_REG1_NAME;
    }
    else
    {
        assert(iml_is_variable(arg2));
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
    else if (iml_is_constant(arg1))
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

    ImlOperand *arg1 = iml_operation_get_operand(op, 2);
    ImlVariable *arg2 = iml_operation_get_operand(op, 1);
    const gchar *label = iml_operation_get_operand(op, 3);
    const gchar *op_cond;
    iml_register_t *right_reg;

    switch (iml_operation_get_opcode(op))
    {
        case iml_jmpneq:
            op_cond = "ne";
            break;
        case iml_jmpuless:
            op_cond = "b";
            break;
        default:
            /* unexpected opcode */
            assert(false);
    }


    /*
     * only variables as right operand is supported at the moment
     */
    assert(iml_is_variable(arg2));


    right_reg = iml_variable_get_register(arg2);
    if (iml_is_constant(arg1))
    {
        if (right_reg == NULL)
        {
            fprintf(out,
                    "    cmpl $%u, %d(%%ebp)\n",
                    iml_constant_get_val_8b(IML_CONSTANT(arg1)),
                    iml_variable_get_frame_offset(arg2));
        }
        else
        {
            fprintf(out,
                    "    cmpl $%u, %%%s\n",
                    iml_constant_get_val_8b(IML_CONSTANT(arg1)),
                    iml_register_get_name(right_reg));
        }
    }
    else
    {
        assert(iml_is_variable(arg1));
        iml_register_t *left_reg;
        const gchar *left_reg_name;

        left_reg = iml_variable_get_register(IML_VARIABLE(arg1));
        if (left_reg != NULL)
        {
            left_reg_name = iml_register_get_name(left_reg);
        }
        else
        {
            left_reg_name = TEMP_REG1_NAME;
            x86_move_to_reg(out, left_reg_name, arg1);
        }

        if (right_reg == NULL)
        {
            fprintf(out,
                    "    cmpl %%%s, %d(%%ebp)\n",
                    left_reg_name,
                    iml_variable_get_frame_offset(arg2));
        }
        else
        {
            fprintf(out,
                    "    cmpl %%%s, %%%s\n",
                    left_reg_name,
                    iml_register_get_name(right_reg));
        }

    }

    fprintf(out, "    j%s %s\n", op_cond, label);
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
        if (g_slist_next(i) != NULL || !x86_param_stored_in_reg(i->data))
        {
            args_stack_size += x86_push_operand(out, IML_OPERAND(i->data));
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
    if (i != NULL && x86_param_stored_in_reg(i->data))
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
    for (i = ir_function_def_get_operations(func_def);
         i != NULL;
         i = g_slist_next(i))
    {
        iml_operation_t *op = (iml_operation_t*) i->data;
        iml_opcode_t opcode = iml_operation_get_opcode(op);

        /* annotate assembly file with compiled IML operations */
        if (opcode != iml_label) /* skip label annotations */
        {
            fprintf(params->out, "\n    # ");
            iml_operation_print(op, params->out, 0);
        }

        switch (opcode)
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
            case iml_setelm:
                x86_compile_setelm(params->out, op);
                break;
            case iml_set:
                x86_compile_set(params->out, op);
                break;
            case iml_getelm:
                x86_compile_getelm(params->out, op);
                break;
            case iml_getaddr:
                x86_compile_getaddr(params->out, op);
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
            case iml_udiv:
            case iml_sdiv:
            case iml_umod:
            case iml_smod:
                x86_compile_divmod(params->out, op);
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
            case iml_jmpuless:
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
