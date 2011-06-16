import std.stdio;
import ir_module;

extern (C)
{
    void
    gen_data_section(FILE *output, IrModule *ir_module);
}
