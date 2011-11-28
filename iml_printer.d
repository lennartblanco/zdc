import std.stdio;
import std.conv;
import ir;

extern (C) void
iml_print_module(IrModule *mod)
{
    writefln("Module: '%s'", to!string(ir_module_get_fqname(mod)));

    for (GSList *i = ir_module_get_function_defs(mod);
         i != null;
         i = i.next())
    {
        print_function(ir_function_def(i.data));
    }
}

private:

void
print_function(IrFunctionDef *func)
{
    writefln("  function: '%s'\n"
             "    frame:",
            to!string(ir_function_def_get_name(func)));

    /* print frame */
    iml_func_frame_print(ir_function_def_get_frame(func), stdout.getFP(), 4);

    /* print function body's iml code */
    writefln("    body:");
    for (GSList *i = ir_function_def_get_operations(func);
         i != null;
         i = i.next())
    {
        iml_operation_print(cast_iml_operation(i.data), stdout.getFP(), 6);
    }
}

