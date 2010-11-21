public import ir_symbol;
public import GError;

struct sym_table_t;

extern (C)
{
    IrSymbol *
    sym_table_get_symbol(sym_table_t *table, const char *name, GError **error);
}
