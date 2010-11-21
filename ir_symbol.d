public import ir_module;

struct IrSymbol;

extern (C)
{
    IrModule *
    ir_symbol_get_parent_module(IrSymbol *self);
}
