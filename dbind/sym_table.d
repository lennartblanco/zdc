module dbind.sym_table;

public import dbind.ir_symbol;
public import dbind.GError;

struct sym_table_t;

extern (C)
{
  IrSymbol *
  sym_table_get_symbol(sym_table_t *table, const char *name, GError **error);
}
