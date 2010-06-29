module dbind.ir_module;

public import dbind.GSList;
public import dbind.sym_table;

struct IrModule;

extern (C)
{
  GSList *
  ir_module_get_function_defs(IrModule *self);

  sym_table_t *
  ir_module_get_symbols(IrModule *self);
}

