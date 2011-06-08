struct IrFunction;

enum ir_linkage_type
{
  d_linkage,
  c_linkage
}

extern (C)
{
    char *
    ir_function_get_mangled_name(IrFunction *self);
}

