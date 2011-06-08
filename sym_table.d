public import ir_symbol;
public import GError;

struct sym_table;

extern (C)
{
    IrSymbol *
    sym_table_get_symbol(sym_table *table, const char *name, GError **error);
}
