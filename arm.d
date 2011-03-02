import std.stdio;
import std.algorithm;
import std.string;
import std.conv;
import std.c.stdio;
import std.c.string;
import auxil;
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
arm_init(arch_backend_s *backend)
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
assign_var_locations(iml_func_frame_t *frame, ir_linkage_type_t linkage)
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

        assert(g_slist_length(params) <= 4,
               "more then 4 function parameters not supported");
        for (i = params, cntr = 0;
             i != null && cntr < 4;
             i = i.next(), cntr += 1)
        {
            iml_variable_set_frame_offset(cast(ImlVariable*)i.data, offset);
            offset -= 4;
        }
    }

    void assign_offset(GSList *variable)
    {
        for (auto i = variable; i != null; i = i.next())
        {
            ImlVariable *var = cast(ImlVariable*)i.data;

            if (iml_variable_get_register(var) == null)
            {
                iml_variable_set_frame_offset(var, offset);
                offset -= 4;
            }
        }
    }

    offset = start_offset = get_start_offset();

    assign_params_offset(iml_func_frame_get_parameters(frame));
    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_32b));
    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_16b));
    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_8b));
    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_ptr));

    assert(iml_func_frame_get_locals(frame, iml_data_type_t.iml_blob) == null,
           "local blob variables not implemented");

    iml_func_frame_set_size(frame, start_offset - offset);
}

extern (C) void
gen_code(IrModule *ir_module, FILE *out_stream, const char *source_file)
{
    File f = File.wrapFile(out_stream);
    f.writefln("    .file \"%s\"\n", to!string(source_file));

    gen_text_prelude(f, ir_module_get_symbols(ir_module));

    GSList *funcs = ir_module_get_function_defs(ir_module);
    for (GSList *i = funcs; i != null; i = i.next())
    {
        compile_function_def(f, cast(IrFunctionDef*)i.data);
    }
}

private void
gen_text_prelude(File asmfile, sym_table_t *sym_table)
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
        "    .text\n"
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
get_used_preserved_regs(iml_func_frame_t *func_frame)
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
    iml_func_frame_t *frame = ir_function_def_get_frame(func);
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
        iml_operation_t *op = cast(iml_operation_t *)i.data;
        iml_opcode_t opcode = iml_operation_get_opcode(op);

        /* annotate assembly file with compiled IML operations */
        if (opcode != iml_opcode_t.label)
        {
            asmfile.writef("\n    @ ");
            iml_operation_print(op, asmfile.getFP(), 0);
        }

        switch (opcode)
        {
            case iml_opcode_t.ret:
                compile_ret(asmfile,
                            op,
                            return_label,
                            i.next() == null);
                break;
            case iml_opcode_t.copy:
                /*
                 * currently variables of all supported iml types are
                 * stored as 32-bit integers,
                 * thus iml_cast is equivalent to iml_copy operation
                 */
            case iml_opcode_t.cast_op:
                compile_copy(asmfile, op);
                break;
            case iml_opcode_t.call_c:
            case iml_opcode_t.call:
                compile_call(asmfile, op);
                break;
            case iml_opcode_t.add:
            case iml_opcode_t.sub:
            case iml_opcode_t.smult:
            case iml_opcode_t.umult:
                compile_binop(asmfile, op);
                break;
            case iml_opcode_t.sless:
            case iml_opcode_t.uless:
            case iml_opcode_t.slesseq:
            case iml_opcode_t.ulesseq:
            case iml_opcode_t.equal:
            case iml_opcode_t.nequal:
            case iml_opcode_t.sgreatereq:
            case iml_opcode_t.ugreatereq:
            case iml_opcode_t.sgreater:
            case iml_opcode_t.ugreater:
                compile_icmp(asmfile, op);
                break;
            case iml_opcode_t.ineg:
                compile_ineg(asmfile, op);
                break;
            case iml_opcode_t.jmpneq:
                compile_jmpcond(asmfile, op);
                break;
            case iml_opcode_t.jmp:
                asmfile.writefln("    b %s",
                                 to!string(cast(char*)
                                           iml_operation_get_operand(op, 1)));
                break;
            case iml_opcode_t.label:
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

private void
gen_move_to_reg(File asmfile, string register, ImlOperand *operand)
{
    if (iml_is_constant(operand))
    {
        asmfile.writefln("    mov %s, #%s",
                         register,
                         iml_constant_get_val_32b(cast(ImlConstant *)operand));
    }
    else
    {
        assert(iml_is_variable(operand));

        ImlVariable *var = cast(ImlVariable *)operand;
        char *reg = iml_variable_get_register(var);

        if (reg != null)
        {
            asmfile.writefln("    mov %s, %s",
                             register, to!string(reg));
        }
        else
        {
            asmfile.writefln("    ldr %s, [fp, #%s]",
                             register, iml_variable_get_frame_offset(var));
        }
    }
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

private void
compile_ret(File asmfile,
            iml_operation_t *op,
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
compile_copy(File asmfile, iml_operation_t *op)
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
compile_binop(File asmfile, iml_operation_t *op)
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
        case iml_opcode_t.add:
            inst = "add";
            break;
        case iml_opcode_t.sub:
            inst = "sub";
            break;
        case iml_opcode_t.smult:
        case iml_opcode_t.umult:
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
compile_ineg(File asmfile, iml_operation_t *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode_t.ineg);

    ImlVariable *src = cast(ImlVariable *)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable *)iml_operation_get_operand(op, 2);
    string src_reg_name;
    string res_reg_name;
    bool store_res = false;

    char *reg = iml_variable_get_register(src);
    if (reg != null)
    {
        src_reg_name = to!string(reg);
    }
    else
    {
        gen_move_to_reg(asmfile, TEMP_REG1, cast(ImlOperand*)src);
        src_reg_name = TEMP_REG1;
    }

    reg = iml_variable_get_register(res);
    if (reg != null)
    {
        res_reg_name = to!string(reg);
    }
    else
    {
        gen_move_to_reg(asmfile, TEMP_REG2, cast(ImlOperand*)res);
        res_reg_name = TEMP_REG2;
        store_res = true;
    }

    asmfile.writefln("    rsb %s, %s, #0",
                     res_reg_name,
                     src_reg_name);
    if (store_res)
    {
        gen_move_from_reg(asmfile, res_reg_name, res);
    }
}

private void
compile_icmp(File asmfile, iml_operation_t *op)
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
        case iml_opcode_t.sless:
            true_suffix = "lt";
            false_suffix = "ge";
            break;
        case iml_opcode_t.uless:
            true_suffix = "lo";
            false_suffix = "hs";
            break;
        case iml_opcode_t.slesseq:
            true_suffix = "le";
            false_suffix = "gt";
            break;
        case iml_opcode_t.ulesseq:
            true_suffix = "ls";
            false_suffix = "hi";
            break;
        case iml_opcode_t.equal:
            true_suffix = "eq";
            false_suffix = "ne";
            break;
        case iml_opcode_t.nequal:
            true_suffix = "ne";
            false_suffix = "eq";
            break;
        case iml_opcode_t.sgreatereq:
            true_suffix = "ge";
            false_suffix = "lt";
            break;
        case iml_opcode_t.ugreatereq:
            true_suffix = "hs";
            false_suffix = "lo";
            break;
        case iml_opcode_t.sgreater:
            true_suffix = "gt";
            false_suffix = "le";
            break;
        case iml_opcode_t.ugreater:
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
compile_jmpcond(File asmfile, iml_operation_t *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode_t.jmpneq,
           "only jmpneq implemented");
    assert(iml_is_variable(iml_operation_get_operand(op, 1)),
           "only variable left operand supported");
    assert(iml_is_constant(iml_operation_get_operand(op, 2)),
           "only constant right operand supported");

    ImlVariable *left = cast(ImlVariable*)iml_operation_get_operand(op, 1);
    ImlConstant *right = cast(ImlConstant*)iml_operation_get_operand(op, 2);
    string label = to!string(cast(char*)iml_operation_get_operand(op, 3));
    string left_reg_name;
    char *reg = iml_variable_get_register(left);

    if (reg != null)
    {
        left_reg_name = to!string(reg);
    }
    else
    {
        gen_move_to_reg(asmfile, TEMP_REG1, cast(ImlOperand*)left);
        left_reg_name = TEMP_REG1;
    }

    asmfile.writefln("    cmp %s, #%s\n"
                     "    bne %s",
                     left_reg_name,
                     iml_constant_get_val_32b(right),
                     label);
}

private void
compile_call(File asmfile, iml_operation_t *op)
{
    string func_name = to!string(cast(char*)iml_operation_get_operand(op, 1));
    GSList *args = cast(GSList *)iml_operation_get_operand(op, 2);
    ImlVariable *ret = cast(ImlVariable*)iml_operation_get_operand(op, 3);

    assert(g_slist_length(args) <= 4,
           "max 4 args in function calls supported");

    GSList *i = args;
    for (uint cntr = 0; i != null && cntr < 4; i = i.next(), cntr += 1)
    {
        gen_move_to_reg(asmfile,
                        "r" ~ to!string(cntr),
                        cast(ImlOperand*)i.data);
    }

    asmfile.writefln("    bl %s", func_name);

    if (ret != null)
    {
        gen_move_from_reg(asmfile, "r0", ret);
    }
}
