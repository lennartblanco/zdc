import std.stdio;
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
import iml_register;
import iml_variable;
import iml_constant;
import types;

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

    p_regs = g_slist_prepend(p_regs,
            iml_register_new(11, "v8"));
    p_regs = g_slist_prepend(p_regs,
            iml_register_new(10, "v7"));
    p_regs = g_slist_prepend(p_regs,
            iml_register_new(9, "v6"));
    p_regs = g_slist_prepend(p_regs,
            iml_register_new(8, "v5"));
    p_regs = g_slist_prepend(p_regs,
            iml_register_new(7, "v4"));
    p_regs = g_slist_prepend(p_regs,
            iml_register_new(6, "v3"));
    p_regs = g_slist_prepend(p_regs,
            iml_register_new(5, "v2"));
    p_regs = g_slist_prepend(p_regs,
            iml_register_new(4, "v1"));

    *scratch = null;
    *preserved = p_regs;
}

extern (C) void
assign_var_locations(iml_func_frame_t *frame)
{
    int start_offset;
    int offset;

    int get_start_offset()
    {
        uint num_of_used_regs;

        num_of_used_regs = g_slist_length(iml_func_frame_get_used_regs(frame));
        return (num_of_used_regs + 3) * -4;
    }

    void assign_offset(GSList *variable)
    {
        GSList *i;

        for (i = variable; i != null; i = g_slist_next(i))
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

    assert(iml_func_frame_get_parameters(frame) == null,
           "function parameters not implemented");

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
    for (GSList *i = funcs; i != null; i = g_slist_next(i))
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
    GSList *sorted_regs = null;
    string[uint] regs;

    /*
     * the registers must be specified in accending order,
     * otherwise assembler will complain
     */

    for (auto i = iml_func_frame_get_used_regs(func_frame);
         i != null;
         i = g_slist_next(i))
    {
        iml_register_t *reg = cast(iml_register_t *)i.data;
        regs[iml_register_get_id(reg)] = to!string(iml_register_get_name(reg));
    }

    foreach (i; 1..16)
    {
        if (i in regs)
        {
            preserved_regs ~= regs[i] ~ ", ";
        }
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

    for (i = ir_function_def_get_operations(func);
         i != null;
         i = g_slist_next(i))
    {
        iml_operation_t *op = cast(iml_operation_t *)i.data;

        switch (iml_operation_get_opcode(op))
        {
            case iml_opcode_t.ret:
                compile_ret(asmfile,
                            op,
                            return_label,
                            g_slist_next(i) == null);
                break;
            case iml_opcode_t.copy:
                compile_copy(asmfile, op);
                break;
            default:
                assert(false, "unexpected iml opcode");
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
        iml_register_t *reg = iml_variable_get_register(var);

        assert(reg != null, "variables not in register not supported");

        asmfile.writefln("    mov %s, %s",
                         register, to!string(iml_register_get_name(reg)));
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
    iml_register_t *reg = iml_variable_get_register(dest);

    assert(reg != null, "move to variables not in register not implemented");
    gen_move_to_reg(asmfile, to!string(iml_register_get_name(reg)), src);
}
