public import GSList;
public import sym_table;

struct IrModule;

extern (C)
{
  GSList *
  ir_module_get_function_defs(IrModule *self);

  sym_table_t *
  ir_module_get_symbols(IrModule *self);

  char *
  ir_module_gen_label(IrModule *self);
}