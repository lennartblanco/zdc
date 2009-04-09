#include <stdlib.h>
#include <stdio.h>

#include "sym_table.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct sym_table_s
{
    sym_table_t *parent;
    GHashTable *table;  
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

sym_table_t*
sym_table_new(sym_table_t* parent)
{
    sym_table_t *tbl;

    tbl = g_malloc(sizeof(*tbl));

    tbl->parent = parent;
    tbl->table = g_hash_table_new(g_str_hash, g_str_equal);

    return tbl;
}

void
sym_table_delete(sym_table_t* self)
{
    g_hash_table_unref(self->table);
    self->table = NULL;
    g_free(self);
}

int 
sym_table_add_symbol(sym_table_t* table, IrSymbol *symbol)
{
    assert(table);
    assert(symbol);

    gpointer p;

    p = g_hash_table_lookup(table->table, ir_symbol_get_name(symbol));
    if (p != NULL)
    {
        return -1;
    }

    g_hash_table_insert(table->table,
                        ir_symbol_get_name(symbol),
                        symbol);
    return 0;
}

IrSymbol *
sym_table_get_symbol(sym_table_t *table, char *name)
{
    assert(table);
    assert(name);

    gpointer p;

    p = g_hash_table_lookup(table->table, name);
    if (p == NULL)
    {
        if (table->parent == NULL) {
            return NULL;
        }
        return sym_table_get_symbol(table->parent, name);
    }

    return p;
}

GList *
sym_table_get_all_symbols(sym_table_t* table)
{
    return g_hash_table_get_values(table->table);
}

void
sym_table_print(sym_table_t *table, FILE *out)
{
    GList *symbols;
    GList *i;
    fprintf(out, "symbol table [%p]:\n", table);

    if (table->parent != NULL)
    {
        fprintf(out, "parent symbols\n");
        sym_table_print(table->parent, out);
    }

    printf("local symbols:\n");
    symbols = sym_table_get_all_symbols(table);
    for (i = symbols; i != NULL; i = g_list_next(i))
    {
        ir_symbol_print(IR_SYMBOL(i->data), out, 0);
        fprintf(out, "\n");
    }
    g_list_free(symbols);
}

