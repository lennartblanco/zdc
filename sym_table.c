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
    GSList *imports;
    GHashTable *table;  
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static GQuark
sym_table_error_quark(void);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

sym_table_t*
sym_table_new(sym_table_t* parent)
{
    sym_table_t *tbl;

    tbl = g_malloc(sizeof(*tbl));

    tbl->parent = parent;
    tbl->imports = NULL;
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

void
sym_table_add_import(sym_table_t* table,
                     sym_table_t* imported_symbols)
{
    table->imports = g_slist_prepend(table->imports, imported_symbols);
}

int 
sym_table_add_symbol(sym_table_t* table, IrSymbol *symbol)
{
    assert(table);
    assert(IR_IS_SYMBOL(symbol));

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
sym_table_get_symbol(sym_table_t *table, const char *name, GError **error)
{
    assert(table);
    assert(name);

    gpointer p = NULL;
    GSList *i;

    /* look for the symbol in local scope */
    p = g_hash_table_lookup(table->table, name);

    /* if not found, look in parent scope(s) */
    if (p == NULL && table->parent != NULL)
    {
        GError *loc_err = NULL;
        p = sym_table_get_symbol(table->parent, name, &loc_err);
        if (p == NULL)
        {
            if (loc_err->code != SYM_TABLE_SYMBOL_NOT_FOUND_ERROR)
            {
                g_propagate_error(error, loc_err);
                return NULL;
            }
            g_error_free(loc_err);
        }
    }

    /* if not found, look in imported scopes */
    if (p == NULL)
    {
        GSList *syms_found = NULL;
        guint syms_found_cntr = 0;
        gpointer sym;

        for (i = table->imports; i != NULL; i = g_slist_next(i))
        {
            sym = sym_table_get_symbol(i->data, name, NULL);
            if (sym != NULL)
            {
                syms_found = g_slist_append(syms_found, sym);
                syms_found_cntr += 1;
            }
        }

        if (syms_found_cntr == 1)
        {
            p = syms_found->data;
        }
        else if (syms_found_cntr > 1)
        {
            GSList *i;
            GString *msg = g_string_new(NULL);
            

            for (i = syms_found; i != NULL; i = g_slist_next(i))
            {
                g_string_append_printf(msg, "%s%s",
                                       ir_symbol_get_fqname(i->data),
                                       g_slist_next(i) ? ", " : "");
            }

            g_set_error(error,
                        SYM_TABLE_ERROR,
                        SYM_TABLE_MULTIPLE_SYMBOLS_FOUND_ERROR,
                        "%s", msg->str);
            g_string_free(msg, TRUE);

        }
        else
        {
            g_set_error(error,
                        SYM_TABLE_ERROR,
                        SYM_TABLE_SYMBOL_NOT_FOUND_ERROR,
                        NULL);

        }
        g_slist_free(syms_found);
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
        ir_node_print(IR_NODE(i->data), out, 0);
        fprintf(out, "\n");
    }
    g_list_free(symbols);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static GQuark
sym_table_error_quark(void)
{
    return g_quark_from_static_string ("zdc-sym-table-error-quark");
}


