#include <stdbool.h>
#include <string.h>

#include "label_gen.h"
#include "types.h"
#include "x86.h"
#include "x86_data.h"
#include "x86_cast.h"
#include "x86_array.h"
#include "x86_if_else.h"
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
        fprintf(out,
                "    pushl $%d\n",
                iml_constant_get_val_32b(IML_CONSTANT(oper)));
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
            x86_move_from_reg(out,
                              iml_register_get_name(src_reg),
                              dst);
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

/**
 * Generate assembly for integer comparison operations
 * (iml_equal and iml_nequal)
 */
static void
x86_compile_icmp(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_equal ||
           iml_operation_get_opcode(op) == iml_nequal);

//    Syntax
//    cmpl <reg>,<reg>
//    cmpl <reg>,<mem>
//    cmpl <mem>,<reg>
//    cmpl <reg>,<con>
    /* not implemented */
    assert(false);
}

/**
 * Generate assembly for passing arguments to a D function.
 */
static void
x86_compile_dcall_args(FILE *out, GSList *arguments)
{
    GSList *i;

    for (i = arguments; i != NULL; i = g_slist_next(i))
    {
        if (g_slist_next(i) != NULL)
        {
            x86_push_operand(out, IML_OPERAND(i->data));
        }
        else
        {
            /* store last argument in eax */
            x86_move_to_reg(out, "eax", IML_OPERAND(i->data));
        }
    }
}

/**
 * Generate assembly for passing arguments to a C function.
 */
static void
x86_compile_ccall_args(FILE *out, GSList *arguments)
{
    GSList *args;
    GSList *i;

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
    }
    g_slist_free(args);
}

static void
x86_compile_call(FILE *out, iml_operation_t *op)
{
    assert(op);
    assert(iml_operation_get_opcode(op) == iml_call ||
            iml_operation_get_opcode(op) == iml_call_c);

    GSList *args = iml_operation_get_operand(op, 2);
    ImlVariable *res;

    /*
     * generate assembly for passing arguments according
     * to calling convention
     */
    switch (iml_operation_get_opcode(op))
    {
        case iml_call:
            x86_compile_dcall_args(out, args);
            break;
        case iml_call_c:
            x86_compile_ccall_args(out, args);
            break;
        default:
            /* unexpected operand */
            assert(false);
    }

    fprintf(out,
            "    call %s\n",
            (char*)iml_operation_get_operand(op, 1));

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
    char return_label[LABEL_MAX_LEN];


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
            case iml_cast:
                /*
                 * currently variables of all supported iml types are
                 * stored as 32-bit integers,
                 * thus iml_cast is equivalent to iml_copy operation
                 */
                x86_compile_copy(params->out, op);
                break;
            case iml_add:
            case iml_sub:
                x86_compile_binop(params->out, op);
                break;
            case iml_equal:
            case iml_nequal:
                x86_compile_icmp(params->out, op);
                break;
            case iml_call:
            case iml_call_c:
                x86_compile_call(params->out, op);
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
