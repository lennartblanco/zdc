import std.string;
import std.conv;
import auxil;
import data_section;
import ir;
import dt_basic;
import arm_util;

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
            ImlVariable *var = cast(ImlVariable*)i.data;

            if (iml_variable_get_data_type(var) == iml_data_type.blob)
            {
                uint size = iml_variable_get_size(var);
                uint register_space_avail = (4 - cntr) * 4;
                if (size > register_space_avail)
                {
                    assert(false, "spliting blob params not implemented");
                }
                else
                {
                  iml_variable_set_frame_offset(var, offset - (size - 4));
                  offset -= size;
                }
            }
            else
            {
                iml_variable_set_frame_offset(var, offset);
                offset -= 4;
            }
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

    void assign_loc_vars_offset(GSList *variables)
    {
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
    assign_loc_vars_offset(iml_func_frame_get_locals(frame));

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
        dt_basic_is_int(ir_function_def_get_return_type(main_func));

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
         i = i.next())
    {
        ImlVariable *var = cast(ImlVariable*)i.data;

        if (iml_variable_get_data_type(var) == iml_data_type.blob)
        {
            asmfile.writefln("    add ip, fp, #%s\n"
                             "    stmia ip, {%s}",
                             iml_variable_get_frame_offset(var),
                             get_blob_arg_regs(cntr, var));
        }
        else
        {
            asmfile.writefln("    str r%s, [fp, #%s]",
                             cntr,
                             iml_variable_get_frame_offset(var));
            cntr += 1;
        }
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
            case iml_opcode.zpad:
                compile_copy(asmfile, op);
                break;
            case iml_opcode.trunc:
                compile_trunc(asmfile, op);
                break;
            case iml_opcode.sigext:
                compile_sigext(asmfile, op);
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
            case iml_opcode.sdiv:
            case iml_opcode.udiv:
            case iml_opcode.smod:
            case iml_opcode.umod:
                compile_divmod(asmfile, op);
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
            case iml_opcode.bconv:
                compile_bconv(asmfile, op);
                break;
            case iml_opcode.jmpneq:
            case iml_opcode.jmpuless:
            case iml_opcode.jmpugreatereq:
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
    /* dump contents of the literal pool */
    asmfile.writefln("    .pool\n");
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

    void blob_copy()
    {
        assert(iml_is_variable(src),
                "constant blobs not as source not supported");
        ImlVariable *src_var = cast(ImlVariable*)src;

        /* both variables should be stored in the function frame */
        assert(iml_variable_get_register(src_var) == null);
        assert(iml_variable_get_register(dest) == null);

        int src_offset = iml_variable_get_frame_offset(src_var);
        int dest_offset = iml_variable_get_frame_offset(dest);
        uint size = iml_variable_get_size(dest);

        assert(size == iml_variable_get_size(src_var));
        assert(size == 8); /* only blobs of 8 bytes supported for now */

        /* copy the 8-byte blob via r0+r1 registers */
        asmfile.writefln("    ldrd r0, [fp, #%s]\n"
                         "    strd r0, [fp, #%s]",
                         src_offset, dest_offset);
    }

    if (iml_operand_get_data_type(src) == iml_data_type.blob)
    {
       blob_copy();
    }
    else if (reg != null)
    {
        gen_move_to_reg(asmfile, to!string(reg), src);
    }
    else
    {
        assert(iml_variable_get_size(dest) <= 4,
               "only 1-4 bytes wide copy implemented");
        gen_move_to_reg(asmfile, TEMP_REG1, src);
        asmfile.writefln("    str %s, [fp, #%s]",
                         TEMP_REG1,
                         iml_variable_get_frame_offset(dest));
    }
}

private void
compile_trunc(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.trunc);
    assert(iml_is_variable(iml_operation_get_operand(op, 1)));
    assert(iml_is_variable(iml_operation_get_operand(op, 2)));

    ImlVariable *val = cast(ImlVariable *)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable *)iml_operation_get_operand(op, 2);

    string val_reg = store_in_reg(asmfile, cast(ImlOperand*)val, TEMP_REG1);

    string res_reg = to!string(iml_variable_get_register(res));
    if (res_reg == null)
    {
        res_reg = TEMP_REG2;
    }

    string bit_mask;
    switch (iml_variable_get_data_type(res))
    {
        case iml_data_type._8b:
            bit_mask = "#0xffffff00";
            break;
        case iml_data_type._16b:
            asmfile.writefln("    ldr %s, =0xffff0000", res_reg);
            bit_mask = res_reg;
            break;
        default:
            assert(false, "unexpected result data type");
    }

    asmfile.writefln("    bic %s, %s, %s",
                     res_reg, val_reg, bit_mask);
    if (iml_variable_get_register(res) == null)
    {
        gen_move_from_reg(asmfile, res_reg, res);
    }
}

private void
compile_sigext(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.sigext);
    assert(iml_is_variable(iml_operation_get_operand(op, 1)));
    assert(iml_is_variable(iml_operation_get_operand(op, 2)));

    ImlVariable *val = cast(ImlVariable *)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable *)iml_operation_get_operand(op, 2);

    /* make sure val operand is stored in a register */
    string val_reg = store_in_reg(asmfile, cast(ImlOperand*)val, TEMP_REG1);

    /* if result is stored in frame offset, use temporary register */
    string res_reg = to!string(iml_variable_get_register(res));
    if (res_reg == null)
    {
        res_reg = TEMP_REG2;
    }

    /* pick instraction to use for sign extension */
    string inst;
    if (iml_variable_get_data_type(val) == iml_data_type._8b)
    {
        if (iml_variable_get_data_type(res) == iml_data_type._16b)
        {
            /*
             * the val register bits 23:16 should be zero, so
             * the result register bits 31:16 will be set to zero thus
             * the 8-bit value will be correctly sign extended to 16-bit
             */
            inst = "sxtb16";
        }
        else
        {
            assert(iml_variable_get_data_type(res) == iml_data_type._32b ||
                   iml_variable_get_data_type(res) == iml_data_type.ptr);
            inst = "sxtb";
        }
    }
    else
    {
        assert(iml_variable_get_data_type(val) == iml_data_type._16b);
        assert(iml_variable_get_data_type(res) == iml_data_type._32b ||
                iml_variable_get_data_type(res) == iml_data_type.ptr);
        inst = "sxth";
    }

    asmfile.writefln("    %s %s, %s",
                     inst, res_reg, val_reg);

    /*
     * if temporary register for result was used, make sure the result
     * is stored in the frame offset
     */
    if (iml_variable_get_register(res) == null)
    {
        gen_move_from_reg(asmfile, res_reg, res);
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
    if (iml_variable_get_register(res) == null)
    {
        dest_reg = TEMP_REG1;
        store_res = true;
    }
    else
    {
        dest_reg = to!string(iml_variable_get_register(res));
        store_res = false;
    }

    /* make sure left operand is loaded into a register */
    bool force = false;
    if (mul_style && iml_is_variable(left))
    {
        /*
         * for mul-style operation, we must make sure that
         * destination register and left register are different
         */
        force =
          dest_reg == to!string(
                        iml_variable_get_register(cast(ImlVariable*)left));
    }
    left_reg = store_in_reg(asmfile, left, pick_left_reg(), force);

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
compile_divmod(File asmfile, iml_operation *op)
{
    ImlOperand *left = cast(ImlOperand*)iml_operation_get_operand(op, 1);
    ImlOperand *right = cast(ImlOperand*)iml_operation_get_operand(op, 2);
    ImlVariable *res = cast(ImlVariable*)iml_operation_get_operand(op, 3);

    string func;
    string res_reg;
    switch (iml_operation_get_opcode(op))
    {
        case iml_opcode.sdiv:
            func = "__aeabi_idiv";
            res_reg = "r0";
            break;
        case iml_opcode.udiv:
            func = "__aeabi_uidiv";
            res_reg = "r0";
            break;
        case iml_opcode.smod:
            func = "__aeabi_idivmod";
            res_reg = "r1";
            break;
        case iml_opcode.umod:
            func = "__aeabi_uidivmod";
            res_reg = "r1";
            break;
        default:
                assert(false,
                       "unexpected iml opcode '" ~
                          to!string(iml_operation_get_opcode(op)) ~ "'");
    }

    gen_move_to_reg(asmfile, "r0", left);
    gen_move_to_reg(asmfile, "r1", right);

    asmfile.writefln("    bl %s", func);

    gen_move_from_reg(asmfile, res_reg, res);
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
compile_bconv(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.bconv);

    ImlOperand *val = cast(ImlOperand *)iml_operation_get_operand(op, 1);
    ImlVariable *res = cast(ImlVariable *)iml_operation_get_operand(op, 2);

    string val_reg = store_in_reg(asmfile, val, TEMP_REG1);
    string res_reg = store_in_reg(asmfile, cast(ImlOperand *)res, TEMP_REG2);

    asmfile.writefln("    cmp %s, #0\n"
                     "    moveq %s, #0\n"
                     "    movne %s, #1",
                     val_reg, res_reg, res_reg);

    if (iml_variable_get_register(res) == null)
    {
        asmfile.writefln("    str %s, [fp, #%s]",
                         res_reg, iml_variable_get_frame_offset(res));
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
        case iml_opcode.jmpugreatereq:
            cond = "hs";
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

private void
compile_set(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.set);
    ImlOperand *src = cast(ImlOperand *)iml_operation_get_operand(op, 1);
    ImlOperand *dest = cast(ImlOperand *)iml_operation_get_operand(op, 2);
    ImlOperand *offset = cast(ImlOperand *)iml_operation_get_operand(op, 3);

    /* make sure source operand is placed in a register */
    string src_reg = store_in_reg(asmfile, src, TEMP_REG1);

    /* make sure destination operand is placed in a register */
    string dest_reg = store_in_reg(asmfile, dest, TEMP_REG2);

    asmfile.writefln("    str%s %s, [%s%s]",
                     access_operation_suffix(src),
                     src_reg,
                     dest_reg,
                     get_offset_expression(offset));
}

private void
compile_get(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.get);

    /* make sure address value is stored in the register */
    string addr_reg =
        store_in_reg(asmfile,
                     cast(ImlOperand *)iml_operation_get_operand(op, 1),
                     TEMP_REG1);

    /* figure out the register where to load the value */
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 3);

    string dest_reg = to!string(iml_variable_get_register(dest));
    if (dest_reg == null)
    {
        dest_reg = TEMP_REG2;
    }

    ImlOperand *offset = cast(ImlOperand *)iml_operation_get_operand(op, 2);
    asmfile.writefln("    ldr%s %s, [%s%s]",
                     access_operation_suffix(cast(ImlOperand*)dest),
                     dest_reg,
                     addr_reg,
                     get_offset_expression(offset));

    /* move the loaded value from temp register if needed */
    if (dest_reg == TEMP_REG2)
    {
        gen_move_from_reg(asmfile, dest_reg, dest);
    }
}

private void
compile_setelm(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.setelm);

    ImlOperand *src = cast(ImlOperand *)iml_operation_get_operand(op, 1);
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 2);
    ImlOperand *index = cast(ImlOperand *)iml_operation_get_operand(op, 3);
    uint size = cast(uint)iml_operation_get_operand(op, 4);

    string addr_exp =
        get_addr_exp(asmfile, dest, index, size, TEMP_REG1, TEMP_REG2);

    /* make sure source operand is placed in a register */
    string src_reg = store_in_reg(asmfile, src, TEMP_REG2);

    asmfile.writefln("    str%s %s, [%s]",
                     access_operation_suffix(src),
                     src_reg,
                     addr_exp);
}

private void
compile_getelm(File asmfile, iml_operation *op)
{
    assert(iml_operation_get_opcode(op) == iml_opcode.getelm);

    ImlVariable *src = cast(ImlVariable *)iml_operation_get_operand(op, 1);
    ImlOperand *index = cast(ImlOperand *)iml_operation_get_operand(op, 2);
    uint size = cast(uint)iml_operation_get_operand(op, 3);
    ImlVariable *dest = cast(ImlVariable *)iml_operation_get_operand(op, 4);

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
    asmfile.writefln("    ldr%s %s, [%s]",
                     access_operation_suffix(cast(ImlOperand*)dest),
                     reg,
                     addr_exp);
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

    assert(iml_variable_is_mem_pinned(var));
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
    for (uint cntr = 0; i != null && cntr < 4; i = i.next())
    {
        ImlOperand *oper = cast(ImlOperand*)i.data;

        if (iml_operand_get_data_type(oper) == iml_data_type.blob)
        {
            /*
             * generate code to load blob variable into rX registers
             */

            /* only blob variable arguments expected */
            assert(iml_is_variable(oper));
            ImlVariable *var = cast(ImlVariable*)oper;

            asmfile.writefln("    add ip, fp, #%s\n"
                             "    ldmia ip, {%s}",
                             iml_variable_get_frame_offset(var),
                             get_blob_arg_regs(cntr, var));
        }
        else
        {
            /* a 32-bit value, load it to a rX register */
            gen_move_to_reg(asmfile, "r" ~ to!string(cntr), oper);
            cntr += 1;
        }
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

    assert(iml_variable_get_data_type(dest) == iml_data_type.blob,
           "only mset to blob variables implemented");

    uint size;
    string suffix;
    switch (iml_operand_get_data_type(src))
    {
        case iml_data_type._32b:
            size = 4;
            suffix = "";
            break;
        case iml_data_type._16b:
            size = 2;
            suffix = "h";
            break;
        case iml_data_type._8b:
            size = 1;
            suffix = "b";
            break;
        default:
            assert(false, "unsupported mset data type");
    }

    /* store source value in register */
    string src_reg = store_in_reg(asmfile, src, TEMP_REG1);
    int dest_offset = iml_variable_get_frame_offset(dest);

    for (uint i = 0; i < num; i += 1)
    {
        asmfile.writefln("    str%s %s, [fp, #%s]",
                         suffix,
                         src_reg,
                         (dest_offset + cast(int)(i * size)));
    }
}
