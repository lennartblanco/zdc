import std.stdio;
import std.algorithm;
import std.string;
import std.conv;
import std.c.stdio;
import std.c.string;
import auxil;
import data_section;
import ir_module;
import ir_function;
import ir_function_def;
import iml_func_frame;
import iml_operation;
import iml_variable;
import iml_constant;
import types;

enum TEMP_REG1 = "ip";
enum TEMP_REG2 = "r0";

extern (C) void
arm_init(arch_backend *backend)
{
    backend.get_registers = &get_registers;
    backend.assign_var_locations = &assign_var_locations;
    backend.gen_code = &gen_code;
}

extern (C) void
get_registers(GSList **scratch, GSList **preserved)
{
    GSList *p_regs;

    p_regs = g_slist_prepend(p_regs, cast(char*)"v7");
    p_regs = g_slist_prepend(p_regs, cast(char*)"v6");
    p_regs = g_slist_prepend(p_regs, cast(char*)"v5");
    p_regs = g_slist_prepend(p_regs, cast(char*)"v4");
    p_regs = g_slist_prepend(p_regs, cast(char*)"v3");
    p_regs = g_slist_prepend(p_regs, cast(char*)"v2");
    p_regs = g_slist_prepend(p_regs, cast(char*)"v1");

    *scratch = null;
    *preserved = p_regs;
}

extern (C) void
assign_var_locations(iml_func_frame *frame, ir_linkage_type linkage)
{
    int start_offset;
    int offset;

    int get_start_offset()
    {
        uint num_of_used_regs;

        num_of_used_regs = g_slist_length(iml_func_frame_get_used_regs(frame));
        return (num_of_used_regs + 3) * -4;
    }

    void assign_params_offset(GSList *params)
    {
        GSList *i;
        uint cntr;

        /*
         * first 4 parameters are passed via r0-r3 registers,
         * they will be stored in the local function frame
         */
        for (i = params, cntr = 0;
             i != null && cntr < 4;
             i = i.next(), cntr += 1)
        {
            assert(iml_variable_get_data_type(cast(ImlVariable*)i.data) !=
                   iml_data_type.blob,
                   "blob function parameters not implemented");
            iml_variable_set_frame_offset(cast(ImlVariable*)i.data, offset);
            offset -= 4;
        }

        /*
         * if there is more then 4 paremeters, the will be passed
         * on the stack
         */
        for (cntr = 4; i != null; i = i.next(), cntr += 4)
        {
            assert(iml_variable_get_data_type(cast(ImlVariable*)i.data) !=
                   iml_data_type.blob,
                   "blob function parameters not implemented");
            iml_variable_set_frame_offset(cast(ImlVariable*)i.data, cntr);
        }
    }

    void assign_offset(iml_data_type vars_type)
    {
        GSList *variables = iml_func_frame_get_locals(frame, vars_type);

        for (auto i = variables; i != null; i = i.next())
        {
            ImlVariable *var = cast(ImlVariable*)i.data;

            if (iml_variable_get_register(var) == null)
            {
                uint var_size = iml_variable_get_size(var);
                iml_variable_set_frame_offset(var, offset - (var_size - 4));
                offset -= var_size;
            }
        }
    }

    offset = start_offset = get_start_offset();

    assign_params_offset(iml_func_frame_get_parameters(frame));
    assign_offset(iml_data_type._32b);
    assign_offset(iml_data_type._16b);
    assign_offset(iml_data_type._8b);
    assign_offset(iml_data_type.ptr);
    assign_offset(iml_data_type.blob);

    iml_func_frame_set_size(frame, start_offset - offset);
}

extern (C) void
gen_code(IrModule *ir_module, FILE *out_stream, const char *source_file)
{
    File f = File.wrapFile(out_stream);
    f.writefln("    .file \"%s\"\n", to!string(source_file));

    data_section.add_literals(f, ir_module);
    gen_text_prelude(f, ir_module_get_symbols(ir_module));

    GSList *funcs = ir_module_get_function_defs(ir_module);
    for (GSList *i = funcs; i != null; i = i.next())
    {
        compile_function_def(f, cast(IrFunctionDef*)i.data);
    }
}

private void
gen_text_prelude(File asmfile, sym_table *sym_table)
{
    IrSymbol *symb;

    asmfile.writefln("    .text");


    /* check if we should generate enty point code for this module */
    symb = sym_table_get_symbol(sym_table, toStringz("main"), null);
    if (symb == null)
    {
        return;
    }

    IrFunctionDef *main_func = cast(IrFunctionDef*)symb;

    assert(ir_function_def_get_parameters(main_func) == null,
           "only entry point without arguments implemented");

    /*
     * if main() returns a value, use it as exit code,
     * otherwise exit with code 0
     */
    bool exit_code_returned = 
        types_is_int(ir_function_def_get_return_type(main_func));

    string main_mangled_name =
        to!string(ir_function_get_mangled_name(cast(IrFunction *) main_func));
    asmfile.writefln(
        "    .align 2\n"
        "    .global main\n"
        "    .type main, %%function\n"
        "main:\n"
        "    stmfd sp!, {lr}\n"
        "    bl %s\n"
        "%s"
        "    ldmfd sp, {pc}\n",
        main_mangled_name,
        exit_code_returned ? "" : "    mov r0, #0\n");
}

/**
 * generates a string of preserved registers that are used
 * in a function frame
 */
private string
get_used_preserved_regs(iml_func_frame *func_frame)
{
    string preserved_regs = "";
    string[] regs;

    /*
     * the registers must be specified in accending order,
     * otherwise assembler will complain
     */

    for (auto i = iml_func_frame_get_used_regs(func_frame);
         i != null;
         i = i.next())
    {
        string reg = to!string(cast(char*)i.data);
        regs = reg ~ regs;
    }
    sort(regs);

    foreach (reg; regs)
    {
        preserved_regs ~= reg ~ ", ";
    }

    return preserved_regs;
}

private void
compile_function_def(File asmfile, IrFunctionDef *func)
{
    IrModule *parent_module;
    string mangled_name;
    string return_label;
    iml_func_frame *frame = ir_function_def_get_frame(func);
    string preserved_regs;
    uint frame_size;
    uint cntr;

    GSList *i;

    mangled_name =
        to!string(ir_function_get_mangled_name(cast(IrFunction *) func));

    frame_size = iml_func_frame_get_size(frame);

    /* generate function return label */
    parent_module = ir_symbol_get_parent_module(cast(IrSymbol *)func);
    return_label = to!string(ir_module_gen_label(parent_module));

    /* get the list of regs that must be preserved */
    preserved_regs = get_used_preserved_regs(frame);

    /*
     * generate function prelude
     */
    asmfile.writefln("    .align 2\n"
                     "    .global %s\n"
                     "    .type %s, %%function\n"
                     "%s:\n"
                     "    mov ip, sp\n"
                     "    stmfd sp!, {%sfp, ip, lr}\n"
                     "    sub fp, ip, #4",
                     mangled_name, mangled_name, mangled_name,
                     preserved_regs);

    if (frame_size > 0)
    {
        /* make room for local variables on stack */
        asmfile.writefln("    sub sp, sp, #%s", frame_size);
    }

    /* store parameters in the function's frame */
    for (i = iml_func_frame_get_parameters(frame), cntr = 0;
         i != null && cntr < 4;
         i = i.next(), cntr += 1)
    {
        asmfile.writefln("    str r%s, [fp, #%s]",
                         cntr,
                         iml_variable_get_frame_offset(
                                                  cast(ImlVariable*)i.data));
    }

    for (i = ir_function_def_get_operations(func);
         i != null;
         i = i.next())
    {
        iml_operation *op = cast(iml_operation *)i.data;
        iml_opcode opcode = iml_operation_get_opcode(op);

        /* annotate assembly file with compiled IML operations */
        if (opcode != iml_opcode.label)
        {
            asmfile.writef("\n    @ ");
            iml_operation_print(op, asmfile.getFP(), 0);
        }

        switch (opcode)
        {
            case iml_opcode.ret:
                compile_ret(asmfile,
                            op,
                            return_label,
                            i.next() == null);
                break;
            case iml_opcode.copy:
                /*
                 * currently variables of all supported iml types are
                 * stored as 32-bit integers,
                 * thus iml_cast is equivalent to iml_copy operation
                 */
            case iml_opcode.cast_op:
                compile_copy(asmfile, op);
                break;
            case iml_opcode.call_c:
            case iml_opcode.call:
                compile_call(asmfile, op);
                break;
            case iml_opcode.add:
            case iml_opcode.sub:
            case iml_opcode.smult:
            case iml_opcode.umult:
            case iml_opcode.and:
            case iml_opcode.or:
                compile_binop(asmfile, op);
                break;
            case iml_opcode.sless:
            case iml_opcode.uless:
            case iml_opcode.slesseq:
            case iml_opcode.ulesseq:
            case iml_opcode.equal:
            case iml_opcode.nequal:
            case iml_opcode.sgreatereq:
            case iml_opcode.ugreatereq:
            case iml_opcode.sgreater:
            case iml_opcode.ugreater:
                compile_icmp(asmfile, op);
                break;
            case iml_opcode.ineg:
            case iml_opcode.bneg:
                compile_neg(asmfile, op);
                break;
            case iml_opcode.jmpneq:
            case iml_opcode.jmpuless:
                compile_jmpcond(asmfile, op);
                break;
            case iml_opcode.set:
                compile_set(asmfile, op);
                break;
            case iml_opcode.get:
                compile_get(asmfile, op);
                break;
            case iml_opcode.setelm:
                compile_setelm(asmfile, op);
                break;
            case iml_opcode.getelm:
                compile_getelm(asmfile, op);
                break;
            case iml_opcode.getaddr:
                compile_getaddr(asmfile, op);
                break;
            case iml_opcode.mset:
                compile_mset(asmfile, op);
                break;
            case iml_opcode.jmp:
                asmfile.writefln("    b %s",
                                 to!string(cast(char*)
                                           iml_operation_get_operand(op, 1)));
                break;
            case iml_opcode.label:
                asmfile.writefln("%s:",
                                 to!string(cast(char*)
                                           iml_operation_get_operand(op, 1)));
                break;
            default:
                assert(false,
                       "unexpected iml opcode '" ~
                          to!string(iml_operation_get_opcode(op)) ~ "'");
        }
    }

    /*
     * generate function return code
     */
    asmfile.writefln("%s:", return_label);
    if (frame_size > 0)
    {
        asmfile.writefln("    add sp, sp, #%s", frame_size);
    }
    asmfile.writefln("    ldmfd sp, {%sfp, sp, pc}",
                     preserved_regs);
}

private string
mult_as_shift(uint mult)
{
    string shift_operand;

    switch (mult)
    {
        case 4:
            shift_operand = ", lsl #2";
            break;
        case 2:
            shift_operand = ", lsl #1";
            break;
        case 1:
            shift_operand = "";
            break;
        default:
            assert(false, "unsupported mult value");
    }

    return shift_operand;
}

/**
 * Generate assembly to move an operand into specified register. The operand's
 * value can optionally be multiplied by an 2^x value.
 *
 * @param file where to write generated assembly
 * @param regiser register name where to store the operand
 * @param operand the operand to store in the register
 * @param mult if specified the value stored in register will be (operand * mult)
 *
 * @return the register were the operand have been moved, e.g. the value of
 *         register parameter
 */
private string
gen_move_to_reg(File asmfile,
                string register,
                ImlOperand *operand,
                uint mult = 1)
{
    if (iml_is_constant(operand))
    {
        ImlConstant *const_op = cast(ImlConstant *)operand;
        if (iml_operand_get_data_type(operand) == iml_data_type.ptr)
        {
            assert(mult == 1); /* can't multiply label constants */
            asmfile.writefln("    ldr %s, =%s",
                             register,
                             to!string(iml_constant_get_val_ptr(const_op)));
        }
        else
        {
            asmfile.writefln("    ldr %s, =%s",
                             register,
                             iml_constant_get_val_32b(const_op) * mult);
        }
    }
    else
    {
        assert(iml_is_variable(operand));

        string shift_operand = mult_as_shift(mult);
        ImlVariable *var = cast(ImlVariable *)operand;
        char *reg = iml_variable_get_register(var);

        if (reg != null)
        {
            asmfile.writefln("    mov %s, %s%s",
                             register, to!string(reg), shift_operand);
        }
        else
        {
            asmfile.writefln("    ldr %s, [fp, #%s]",
                             register, iml_variable_get_frame_offset(var));
            if (shift_operand != "")
            {
                asmfile.writefln("    mov %s, %s%s",
                                 register, register, shift_operand);
            }
        }
    }

    return register;
}

private void
gen_move_from_reg(File asmfile, string src_reg, ImlVariable *destination)
{
    char *dest_reg = iml_variable_get_register(destination);

    if (dest_reg != null)
    {
        asmfile.writefln("    mov %s, %s",
                         to!string(dest_reg),
                         src_reg);
    }
    else
    {
        asmfile.writefln("    str %s, [fp, #%s]",
                         src_reg,
                         iml_variable_get_frame_offset(destination));
    }
}

/**
 * Check if operand is stored in register. If not, generate assembly to
 * copy operand to provided register.
 *
 * @param asmfile the file where to write generated code
 * @param operand the operand to be copied to a register
 * @param register the register where operand can be stored
 *
 * @return the register where operand is stored
 */
private string
store_in_reg(File asmfile, ImlOperand *operand, string register)
{
    if (iml_is_constant(operand))
    {
        gen_move_to_reg(asmfile, register, operand);
    }
    else
    {
        assert(iml_is_variable(operand));
        char *reg = iml_variable_get_register(cast(ImlVariable*)operand);
        if (reg == null)
        {
            gen_move_to_reg(asmfile, register, operand);
        }
        else
        {
            register = to!string(reg);
        }
    }

    return register;
}

private void
compile_ret(File asmfile,
            iml_operation *op,
            string return_label,
            bool is_last_op)
{
    ImlOperand *ret_value = cast(ImlOperand *)iml_operation_get_operand(op, 1);

    if (ret_value != null)
    {
        gen_move_to_reg(asmfile, "r0", ret_value);
    }

    if (!is_last_op)
    {
        asmfile.writefln("    b %s", return_label);
    }
}

private void
compile_copy(File asmfile, iml_operation *op)
{
    ImlOperand *src = cast(ImlOperand *)iml_operation_get_operand(op, 1);
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 2);
    char *reg = iml_variable_get_register(dest);

    if (reg != null)
    {
        gen_move_to_reg(asmfile, to!string(reg), src);
    }
    else
    {
        gen_move_to_reg(asmfile, TEMP_REG1, src);
        asmfile.writefln("    str %s, [fp, #%s]",
                         TEMP_REG1,
                         iml_variable_get_frame_offset(dest));
    }
}

/**
 * Analyze the operands for an 3 operands instruction e.g.
 * 'instruction result, left, right', where result and left must be registers
 * and right can be either register or constant value.
 *
 * Pick registers for operands and generate code to load operands into
 * registers if needed.
 *
 * @param asmfile file where to write generated code
 * @param left left operand
 * @param right right operand
 * @param res result operand
 * @param mul_style if true,
 *        operands will be loaded suitable for mul instruction
 *        (all operand are loaded into registers,
 *         different register for result and left operand will be used)
 * @param left_reg register where left operand have been loaded
 * @param right_exp expression of right operand
 * @param dest_reg the register where result will be stored
 * @param store_res if true, code to store instruction result in res operand
 *                  must be generated
 */
private void
load_to_regs(File asmfile,
             ImlOperand *left,
             ImlOperand *right,
             ImlVariable *res,
             bool mul_style,
             out string left_reg,
             out string right_exp,
             out string dest_reg,
             out bool store_res)
{
    char *res_reg = iml_variable_get_register(res);

    string pick_left_reg()
    {
        if (mul_style)
        {
            if (dest_reg != TEMP_REG1)
            {
                return TEMP_REG1;
            }
            else
            {
                return TEMP_REG2;
            }
            assert(false);
        }
        else
        {
            return dest_reg;
        }
    }

    string pick_right_reg()
    {
        /* can we use destination register to store right operand ? */
        if (dest_reg == left_reg)
        {
            /*
             * no, left operand is already stored there,
             * use temporary register
             */
            return TEMP_REG2;
        }
        else
        {
            /* yes, we can use destination register for right operand */
            return dest_reg;
        }
    }

    /* pick the destination register */
    if (res_reg == null)
    {
        dest_reg = TEMP_REG1;
        store_res = true;
    }
    else
    {
        dest_reg = to!string(res_reg);
        store_res = false;
    }

    /* make sure left operand is loaded into a register */
    if (iml_is_constant(left))
    {
        left_reg = pick_left_reg();
        asmfile.writefln("    mov %s, #%s",
                         left_reg,
                         iml_constant_get_val_32b(cast(ImlConstant*)left));
    }
    else
    {
        assert(iml_is_variable(left));
        ImlVariable *var = cast(ImlVariable *)left;
        char *reg = iml_variable_get_register(var);

        if (reg == null)
        {
            left_reg = pick_left_reg();
            asmfile.writefln("    ldr %s, [fp, #%s]",
                             left_reg,
                             iml_variable_get_frame_offset(var));
        }
        else
        {
            left_reg = to!string(reg);
        }
    }

    /* figure out right operand expression */
    if (iml_is_constant(right))
    {
        assert(!iml_is_constant(left));

        if (mul_style)
        {
            right_exp = pick_right_reg();
            asmfile.writefln("    mov %s, #%s",
                             right_exp,
                             iml_constant_get_val_32b(
                                     cast(ImlConstant*)right));
        }
        else
        {
            right_exp =
                "#" ~ to!string(
                        iml_constant_get_val_32b(cast(ImlConstant*)right));
        }
    }
    else
    {
        /* right operand is a variable, make sure it's loaded into register */
        assert(iml_is_variable(right));
        ImlVariable *var = cast(ImlVariable *)right;
        char *reg = iml_variable_get_register(var);

        if (reg == null)
        {
            right_exp = pick_right_reg();
            asmfile.writefln("    ldr %s, [fp, #%s]",
                             right_exp,
                             iml_variable_get_frame_offset(var));

        }
        else
        {
            right_exp = to!string(reg);
        }
    }
}

private void
compile_binop(File asmfile, iml_operation *op)
{
    ImlOperand *left = cast(ImlOperand*)iml_operation_get_operand(op, 1);
    ImlOperand *right = cast(ImlOperand*)iml_operation_get_operand(op, 2);
    ImlVariable *res = cast(ImlVariable*)iml_operation_get_operand(op, 3);
    string inst;
    string dest_reg;
    string left_reg;
    string right_exp;
    bool mul_style = false;
    bool store_res;

    switch (iml_operation_get_opcode(op))
    {
        case iml_opcode.and:
            inst = "and";
            break;
        case iml_opcode.or:
            inst = "orr";
            break;
        case iml_opcode.add:
            inst = "add";
            break;
        case iml_opcode.sub:
            inst = "sub";
            break;
        case iml_opcode.smult:
        case iml_opcode.umult:
            inst = "mul";
            mul_style = true;
            break;
        default:
            assert(false,
                   "unexpectexted binary opcode '" ~
                   to!string(iml_operation_get_opcode(op)) ~ "'");
    }


    load_to_regs(asmfile,
                 left, right, res, mul_style,
                 left_reg, right_exp, dest_reg,
                 store_res);


    /* generate the operation */
    asmfile.writefln("    %s %s, %s, %s",
                     inst, dest_reg, left_reg, right_exp);

    /* move the result from register to result variable if needed */
    if (store_res)
    {
        gen_move_from_reg(asmfile, dest_reg, res);
    }
}

private void
compile_neg(File asmfile, iml_operation *op)
{
    ImlVariable *src = cast(ImlVariable *)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable *)iml_operation_get_operand(op, 2);
    string res_reg_name;

    string src_reg = store_in_reg(asmfile, cast(ImlOperand*)src, TEMP_REG1);

    char *reg = iml_variable_get_register(res);
    if (reg != null)
    {
        res_reg_name = to!string(reg);
    }
    else
    {
        res_reg_name =
            gen_move_to_reg(asmfile, TEMP_REG2, cast(ImlOperand*)res);
    }

    switch (iml_operation_get_opcode(op))
    {
        case iml_opcode.ineg:
            asmfile.writefln("    rsb %s, %s, #0", res_reg_name, src_reg);
            break;
        case iml_opcode.bneg:
            asmfile.writefln("    eor %s, %s, #1", res_reg_name, src_reg);
            break;
        default:
            assert(false, "unexpected opcode");
    }

    if (reg == null)
    {
        gen_move_from_reg(asmfile, res_reg_name, res);
    }
}

private void
compile_icmp(File asmfile, iml_operation *op)
{
    ImlOperand *left = cast(ImlOperand*)iml_operation_get_operand(op, 1);
    ImlOperand *right = cast(ImlOperand*)iml_operation_get_operand(op, 2);
    ImlVariable *res = cast(ImlVariable*)iml_operation_get_operand(op, 3);
    string dest_reg;
    string left_reg;
    string right_exp;
    bool store_res;
    string true_suffix;
    string false_suffix;

    switch (iml_operation_get_opcode(op))
    {
        case iml_opcode.sless:
            true_suffix = "lt";
            false_suffix = "ge";
            break;
        case iml_opcode.uless:
            true_suffix = "lo";
            false_suffix = "hs";
            break;
        case iml_opcode.slesseq:
            true_suffix = "le";
            false_suffix = "gt";
            break;
        case iml_opcode.ulesseq:
            true_suffix = "ls";
            false_suffix = "hi";
            break;
        case iml_opcode.equal:
            true_suffix = "eq";
            false_suffix = "ne";
            break;
        case iml_opcode.nequal:
            true_suffix = "ne";
            false_suffix = "eq";
            break;
        case iml_opcode.sgreatereq:
            true_suffix = "ge";
            false_suffix = "lt";
            break;
        case iml_opcode.ugreatereq:
            true_suffix = "hs";
            false_suffix = "lo";
            break;
        case iml_opcode.sgreater:
            true_suffix = "gt";
            false_suffix = "le";
            break;
        case iml_opcode.ugreater:
            true_suffix = "hi";
            false_suffix = "ls";
            break;
        default:
            assert(false,
                   "unexpectexted opcode '" ~
                   to!string(iml_operation_get_opcode(op)) ~ "'");
    }

    load_to_regs(asmfile,
                 left, right, res, false,
                 left_reg, right_exp, dest_reg,
                 store_res);


    /* generate the operation */
    asmfile.writefln("    cmp %s, %s\n"
                     "    mov%s %s, #0\n"
                     "    mov%s %s, #1",
                     left_reg, right_exp,
                     false_suffix, dest_reg,
                     true_suffix, dest_reg);

    if (store_res)
    {
        gen_move_from_reg(asmfile, dest_reg, res);
    }
}

private void
compile_jmpcond(File asmfile, iml_operation *op)
{
    ImlOperand *left = cast(ImlOperand*)iml_operation_get_operand(op, 1);
    ImlOperand *right = cast(ImlOperand*)iml_operation_get_operand(op, 2);
    string label = to!string(cast(char*)iml_operation_get_operand(op, 3));

    string left_reg = store_in_reg(asmfile, left, TEMP_REG1);

    string right_exp;
    if (iml_is_constant(right))
    {
        right_exp =
            "#" ~ to!string(iml_constant_get_val_32b(cast(ImlConstant*)right));
    }
    else
    {
        assert(iml_is_variable(right));
        right_exp = store_in_reg(asmfile, right, TEMP_REG1);
    }

    string cond;
    switch (iml_operation_get_opcode(op))
    {
        case iml_opcode.jmpneq:
            cond = "ne";
            break;
        case iml_opcode.jmpuless:
            cond = "lo";
            break;
        default:
            assert(false, "unexpected opcode");
    }

    asmfile.writefln("    cmp %s, %s\n"
                     "    b%s %s",
                     left_reg,
                     right_exp,
                     cond,
                     label);
}

/**
 * Generate the offset expression for ldr or str instruction.
 *
 * @param offset the IML offset value
 *
 * @return the offset expression or empty string if no offset or offset 0
 *         is specified
 */
private string
get_offset_expression(ImlConstant *offset)
{
    if (offset == null || iml_constant_get_val_32b(offset) == 0)
    {
        return "";
    }

    return ", #" ~ to!string(iml_constant_get_val_32b(offset));
}

private void
compile_set(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.set);
    ImlOperand *src = cast(ImlOperand *)iml_operation_get_operand(op, 1);
    ImlOperand *dest = cast(ImlOperand *)iml_operation_get_operand(op, 2);
    ImlConstant *offset = cast(ImlConstant *)iml_operation_get_operand(op, 3);

    assert(iml_operand_get_data_type(src) == iml_data_type._32b ||
           iml_operand_get_data_type(src) == iml_data_type.ptr,
           "only 32-bit set is implemented");
    assert(offset == null, "set offset not implemented");

    /* make sure source operand is placed in a register */
    string src_reg = store_in_reg(asmfile, src, TEMP_REG1);

    /* make sure destination operand is placed in a register */
    string dest_reg = store_in_reg(asmfile, dest, TEMP_REG2);

    asmfile.writefln("    str %s, [%s%s]",
                     src_reg,
                     dest_reg,
                     get_offset_expression(offset));
}

private void
compile_get(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.get);
    assert(iml_operation_get_operand(op, 2) == null,
           "get offset not implemented");

    /* make sure address value is stored in the register */
    string addr_reg =
        store_in_reg(asmfile,
                     cast(ImlOperand *)iml_operation_get_operand(op, 1),
                     TEMP_REG1);

    /* figure out the register where to load the value */
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 3);

    assert(iml_operand_get_data_type(cast(ImlOperand*)dest) ==
                                                    iml_data_type._32b ||
           iml_operand_get_data_type(cast(ImlOperand*)dest) ==
                                                    iml_data_type.ptr,
           "only 32-bit get is implemented");

    string dest_reg = to!string(iml_variable_get_register(dest));
    if (dest_reg == null)
    {
        dest_reg = TEMP_REG2;
    }

    ImlConstant *offset = cast(ImlConstant *)iml_operation_get_operand(op, 2);
    asmfile.writefln("    ldr %s, [%s%s]",
                     dest_reg,
                     addr_reg,
                     get_offset_expression(offset));

    /* move the loaded value from temp register if needed */
    if (dest_reg == TEMP_REG2)
    {
        gen_move_from_reg(asmfile, dest_reg, dest);
    }
}

/**
 * Generate an array element address expression, which can be used in
 * ldr and str instructions. Will generate assembly to preform address
 * calculations if needed.
 *
 * @param asmfile  the file were to write generated assembly if needed
 * @param base the array base variable, must be of blob or ptr type
 * @param index the array index operand
 * @param size array element size in bytes
 * @param addr_reg the register to use as a part of address expression,
 *                 for cases when part of address expression must be stored
 *                 in a register, e.g. "fp, r0"
 * @param temp_reg temporary register to use for address calculations if needed
 *
 * @return the derived address expression, for example "fp, #-52"
 */
private string
get_addr_exp(File asmfile,
             ImlVariable *base,
             ImlOperand *index,
             uint size,
             string addr_reg,
             string temp_reg)
{
    string addr_exp;

    if (iml_variable_get_data_type(base) == iml_data_type.blob)
    {
        int base_offset = iml_variable_get_frame_offset(base);

        if (iml_is_constant(index))
        {
            assert(iml_operand_get_data_type(index) == iml_data_type._32b);
            ImlConstant *const_idx = cast(ImlConstant *)index;
            addr_exp = "fp, #" ~
                to!string(base_offset +
                   cast(int)(iml_constant_get_val_32b(const_idx) * size));

        }
        else
        {
            assert(iml_is_variable(index));

            gen_move_to_reg(asmfile, addr_reg, index, size);
            asmfile.writefln("    add %s, %s, #%s",
                             addr_reg,
                             addr_reg,
                             base_offset);
            addr_exp = "fp, " ~ addr_reg;
        }
    }
    else
    {
        assert(iml_variable_get_data_type(base) == iml_data_type.ptr);

        string base_reg =
            store_in_reg(asmfile, cast(ImlOperand*)base, addr_reg);

        if (iml_is_constant(index))
        {
            ImlConstant *const_idx = cast(ImlConstant *)index;
            int offset = cast(int)iml_constant_get_val_32b(const_idx) * size;

            addr_exp = base_reg ~
                       (offset != 0 ? ", #" ~ to!string(offset) : "");
        }
        else
        {
            assert(iml_is_variable(index));

            ImlVariable *index_var = cast(ImlVariable *)index;
            string index_reg = store_in_reg(asmfile, index, temp_reg);

            asmfile.writefln("    add %s, %s, %s%s",
                             addr_reg,
                             base_reg,
                             index_reg,
                             mult_as_shift(size));
            addr_exp = addr_reg;
        }
    }

    return addr_exp;
}

private void
compile_setelm(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.setelm);

    ImlOperand *src = cast(ImlOperand *)iml_operation_get_operand(op, 1);
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 2);
    ImlOperand *index = cast(ImlOperand *)iml_operation_get_operand(op, 3);
    uint size = cast(uint)iml_operation_get_operand(op, 4);

    string op_suffix;

    switch (iml_operand_get_data_type(src))
    {
        case iml_data_type._32b:
        case iml_data_type.ptr:
            op_suffix = "";
            break;
        case iml_data_type._16b:
            op_suffix = "h";
            break;
        case iml_data_type._8b:
            op_suffix = "b";
            break;
        default:
            assert(false, "unexpected source data type");
    }

    string addr_exp =
        get_addr_exp(asmfile, dest, index, size, TEMP_REG1, TEMP_REG2);

    /* make sure source operand is placed in a register */
    string src_reg = store_in_reg(asmfile, src, TEMP_REG2);

    asmfile.writefln("    str%s %s, [%s]", op_suffix, src_reg, addr_exp);
}

private void
compile_getelm(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.getelm);

    ImlVariable *src = cast(ImlVariable *)iml_operation_get_operand(op, 1);
    ImlOperand *index = cast(ImlOperand *)iml_operation_get_operand(op, 2);
    uint size = cast(uint)iml_operation_get_operand(op, 3);
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 4);

    string op_suffix;

    switch (iml_variable_get_data_type(dest))
    {
        case iml_data_type._32b:
        case iml_data_type.ptr:
            op_suffix = "";
            break;
        case iml_data_type._16b:
            op_suffix = "h";
            break;
        case iml_data_type._8b:
            op_suffix = "b";
            break;
        default:
            assert(false, "unexpected destination data type");
    }

    string addr_exp =
        get_addr_exp(asmfile, src, index, size, TEMP_REG1, TEMP_REG2);

    char *dest_reg = iml_variable_get_register(dest);
    string reg;
    if (dest_reg != null)
    {
        reg = to!string(dest_reg);
    }
    else
    {
        reg = TEMP_REG1;
    }
    asmfile.writefln("    ldr%s %s, [%s]", op_suffix, reg, addr_exp);
    if (dest_reg == null)
    {
        gen_move_from_reg(asmfile, TEMP_REG1, dest);
    }
}

private void
compile_getaddr(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.getaddr);

    ImlVariable *var = cast(ImlVariable *)iml_operation_get_operand(op, 1);
    ImlVariable *addr = cast(ImlVariable *)iml_operation_get_operand(op, 2);

    assert(iml_variable_get_data_type(var) == iml_data_type.blob);
    assert(iml_variable_get_data_type(addr) == iml_data_type.ptr);

    char *addr_reg = iml_variable_get_register(addr);
    string reg = addr_reg != null ? to!string(addr_reg) : TEMP_REG1;

    asmfile.writefln("    add %s, fp, #%s",
                     reg, iml_variable_get_frame_offset(var));
    if (addr_reg == null)
    {
        gen_move_from_reg(asmfile, reg, addr);
    }
}

private void
compile_call(File asmfile, iml_operation *op)
{
    string func_name = to!string(cast(char*)iml_operation_get_operand(op, 1));
    GSList *args = cast(GSList *)iml_operation_get_operand(op, 2);
    ImlVariable *ret = cast(ImlVariable*)iml_operation_get_operand(op, 3);

    GSList *i = args;
    for (uint cntr = 0; i != null && cntr < 4; i = i.next(), cntr += 1)
    {
        gen_move_to_reg(asmfile,
                        "r" ~ to!string(cntr),
                        cast(ImlOperand*)i.data);
    }

    uint args_stack_size = 0; /* number of bytes on stack used for arguments */
    i = g_slist_reverse(g_slist_copy(i));
    for (; i != null; i = i.next())
    {
        asmfile.writefln("    push {%s}",
                         store_in_reg(asmfile,
                                      cast(ImlOperand*)i.data,
                                      TEMP_REG1));
        args_stack_size += 4;
    }
    g_slist_free(i);

    asmfile.writefln("    bl %s", func_name);

    /* remove arguments from stack if needed */
    if (args_stack_size > 0)
    {
        asmfile.writefln("    add sp, sp, #%s", args_stack_size);
    }


    if (ret != null)
    {
        gen_move_from_reg(asmfile, "r0", ret);
    }
}

private void
compile_mset(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.mset);

    ImlOperand *src = cast(ImlOperand *)iml_operation_get_operand(op, 1);
    uint num = cast(uint)iml_operation_get_operand(op, 2);
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 3);

    assert(iml_operand_get_data_type(src) == iml_data_type._32b,
           "only 32-bit mset implemented");
    assert(iml_variable_get_data_type(dest) == iml_data_type.blob,
           "only mset to blob variables implemented");

    /* store source value in register */
    string src_reg = store_in_reg(asmfile, src, TEMP_REG1);
    int dest_offset = iml_variable_get_frame_offset(dest);

    for (uint i = 0; i < num; i += 1)
    {
        asmfile.writefln("    str %s, [fp, #%s]",
                         src_reg,
                         (dest_offset + cast(int)(i * 4)));
    }
}
