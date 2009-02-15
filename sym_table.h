#ifndef SEM_TABLE_INC_X
#define SEM_TABLE_INC_X

#include "ir_symbol.h"
#include "sym_table_types.h"

/**
 * Create a new symbol table.
 *
 * @param parent the symbol table at previous code scope, or NULL if this is
 *         the global symbol table
 */
sym_table_t*
sym_table_new(sym_table_t* parent);

//void
//sym_table_foreach_sym(sym_table_t *table,
//                      void (*func) (ast_node_t *symp));

/**
 * @return the number of symbols in this table
 */
//guint 
//sym_table_get_size(sym_table_t *table);

/**
 * Deallocate all resources used by this symbol table
 */
void
sym_table_delete(sym_table_t* table);

/**
 * @return 0 - successfully added
 *         -1 - symbol already present in the table
 */
int 
sym_table_add_symbol(sym_table_t* table, IrSymbol *symbol);


/**
 * look for a symbol in the provided table
 *
 * @param table   the table where to look for the element
 * @param name    symbol's name
 *
 * @return The symbol of NULL of not such symbol is found.
 */
IrSymbol *
sym_table_get_symbol(sym_table_t *table, char *name);

/**
 * @return all symbols in this table as a list of IrSymbol pointers,
 *         the list should freed, when not needed anymore,
 *         by calling g_list_free() 
 */
GList *
sym_table_get_all_symbols(sym_table_t* table);

/**
 * debug function
 * print contents of the symbol table
 */
void
sym_table_dump(sym_table_t *table);

#endif /* SEM_TABLE_INC_X */
