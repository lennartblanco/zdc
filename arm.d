import std.stdio;
import std.c.stdio;
import std.c.string;
import dbind.ir_module;
import dbind.ir_function_def;


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

    gen_text_prelude(f, ir_module);

    GSList *funcs = ir_module_get_function_defs(ir_module);
    for (GSList *i = funcs; i != null; i = g_slist_next(i))
    {
      compile_function_def(f, cast(IrFunctionDef*)i.data);
    }
}

private void
gen_text_prelude(File asmfile, IrModule *ir_module)
{
    asmfile.writefln(
        "    .text\n"
        "    .align 2\n"
        "    .global main\n"
        "    .type main, %%function\n"
        "main:\n"
        "    stmfd sp!, {lr}\n"
        "    bl _D3wee4mainFZv\n"
        "    ldmfd sp, {pc}\n");
}

int dummy;

private void
compile_function_def(File asmfile, IrFunctionDef *func)
{
    string mangled_name;
    string name;

    name = d_str(ir_function_def_get_name(func));
    mangled_name = d_str(ir_function_def_get_mangled_name(func));

    asmfile.writefln("    .align 2\n"
                     "    .global %s\n"
                     "    .type %s, %%function\n"
                     "%s:\n"
                     "     mov r0, #%d\n"
                     "     mov pc, lr\n",
                     mangled_name, mangled_name, mangled_name, dummy++);
}
