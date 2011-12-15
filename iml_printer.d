import std.stdio;
import std.conv;
import ir;
import iml;

extern (C) void
iml_print_module(IrModule *mod)
{
    writefln("Module: '%s'",
             to!string(ir_scope_get_fqname(ir_module_get_scope(mod))));

    for (GSList *i = ir_module_get_functions(mod); i != null; i = i.next())
    {
        iml_function_print(cast_iml_function(i.data), stdout.getFP(), 2);
    }
}
