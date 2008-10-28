#include <stdlib.h>
#include <stdio.h>

#include "sym_table.h"
#include "ast.h"

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

int 
sym_table_add_symbol(sym_table_t* table, 
                     char *name, 
                     ir_symbol_t *symbol)
{
    assert(table);

    gpointer p;

    p = g_hash_table_lookup(table->table, name);
    if (p != NULL)
    {
        return -1;
    }

    g_hash_table_insert(table->table, name, symbol);
    return 0;
}

int
sym_table_get_symbol(sym_table_t *table, char *name, ir_symbol_t **symbol)
{
    assert(table);
    assert(name);
    assert(symbol);

    gpointer p;

    p = g_hash_table_lookup(table->table, name);
    if (p == NULL)
    {
        return -1;
    }

    *symbol = p;
    return 0;
}


void
sym_table_foreach_sym(sym_table_t *table, void (*func) (ast_node_t *))
{
    GList *keys = g_hash_table_get_keys(table->table);
    GList *p = keys;
    ast_node_t *symb;
    
    while (p != NULL)
    {
        symb = g_hash_table_lookup(table->table, p->data);
        func(symb);
        p =  p->next;
    }

    g_list_free(p);
}

GList *
sym_table_get_all_symbols(sym_table_t* table)
{
    return g_hash_table_get_values(table->table);
}

