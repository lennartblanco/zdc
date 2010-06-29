module dbind.ir_module;

public import dbind.GSList;

struct IrModule;

extern (C)
{
  GSList *
  ir_module_get_function_defs(IrModule *self);
}

