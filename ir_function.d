module dbind.ir_function;

struct IrFunction;

extern (C)
{
    char *
    ir_function_get_mangled_name(IrFunction *self);
}

