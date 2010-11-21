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

void arm_init(arch_backend_s *backend)
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
    int offset = -12;
    void assign_offset(GSList *variable)
    {
        GSList *i;

        for (i = variable; i != null; i = g_slist_next(i))
        {
            ImlVariable *var = cast(ImlVariable*)i.data;
            writefln("variable %s", i.data);
            if (iml_variable_get_register(var) == null)
            {
                iml_variable_set_frame_offset(var, offset);
                offset -= 4;
            }
        }
    }

    assert(iml_func_frame_get_parameters(frame) == null,
           "function parameters not implemented");

    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_32b));
    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_16b));
    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_8b));
    assign_offset(iml_func_frame_get_locals(frame, iml_data_type_t.iml_ptr));

    assert(iml_func_frame_get_locals(frame, iml_data_type_t.iml_blob) == null,
           "local blob variables not implemented");

    iml_func_frame_set_size(frame, -12 - offset);
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

private void
compile_function_def(File asmfile, IrFunctionDef *func)
{
    IrModule *parent_module;
    string mangled_name;
    string return_label;
    iml_func_frame_t *frame = ir_function_def_get_frame(func);
    uint frame_size;
    GSList *i;

    mangled_name =
        to!string(ir_function_get_mangled_name(cast(IrFunction *) func));

    frame_size = iml_func_frame_get_size(frame);

    /* generate function return label */
    parent_module = ir_symbol_get_parent_module(cast(IrSymbol *)func);
    return_label = to!string(ir_module_gen_label(parent_module));

    /* generate function prelude */
    asmfile.writefln("    .align 2\n"
                     "    .global %s\n"
                     "    .type %s, %%function\n"
                     "%s:\n"
                     "    mov ip, sp\n"
                     "    stmfd sp!, {fp, ip, lr}\n"
                     "    sub fp, ip, #4\n"
                     "    sub sp, sp, #%s",
                     mangled_name, mangled_name, mangled_name,
                     frame_size);

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
            default:
                assert(false, "unexpected iml opcode");
        }
    }

    /* generate function return code */
    asmfile.writefln("%s:\n"
                     "    add sp, sp, #%s\n"
                     "    ldmfd sp, {fp, sp, pc}",
                     return_label, frame_size);
}

private void
gen_move_to_reg(File asmfile, string register, ImlOperand *operand)
{
    assert(iml_is_constant(operand), "only constants implemented for now");
    asmfile.writefln("    mov %s, #%s",
                     register,
                     iml_constant_get_val_32b(cast(ImlConstant *)operand));
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
