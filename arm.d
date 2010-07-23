import std.stdio;
import std.string;
import std.c.stdio;
import std.c.string;
import dbind.ir_module;
import dbind.ir_function_def;
import dbind.types;

string
d_str(const char *c_str)
{
    char[] str = new char[strlen(c_str)];

    memcpy(str.ptr, c_str, str.length);

    return cast(string)str;
}


GSList *
g_slist_next(GSList *l)
{
  return l.next;
}

extern (C) void
arm_gen_code(IrModule *ir_module, FILE *out_stream, const char *source_file)
{
    File f = File.wrapFile(out_stream);
    f.writefln("    .file \"%s\"\n", d_str(source_file));

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
        d_str(ir_function_def_get_mangled_name(main_func));
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
    string mangled_name;
    string name;
    static int dummy;

    name = d_str(ir_function_def_get_name(func));
    mangled_name = d_str(ir_function_def_get_mangled_name(func));

    /* just generate a dummy function code which returns some number */
    asmfile.writefln("    .align 2\n"
                     "    .global %s\n"
                     "    .type %s, %%function\n"
                     "%s:\n"
                     "     mov r0, #%d\n"
                     "     mov pc, lr\n",
                     mangled_name, mangled_name, mangled_name, dummy++);
}