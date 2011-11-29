#ifndef SYM_TABLE_INC_X
#define SYM_TABLE_INC_X

#include "ir_symbol.h"
#include "sym_table_types.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

/** parser errors domain */
#define SYM_TABLE_ERROR sym_table_error_quark()

typedef enum
{
    SYM_TABLE_SYMBOL_NOT_FOUND_ERROR,
    SYM_TABLE_MULTIPLE_SYMBOLS_FOUND_ERROR
} sym_table_error_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Create a new symbol table.
 *
 * @param parent the symbol table at previous code scope, or NULL if this is
 *         the global symbol table
 */
sym_table_t*
sym_table_new(sym_table_t* parent);

/**
 * Deallocate all resources used by this symbol table.
 *
 * This will not delete symbols objects stored in the table nor will
 * it delete the parent symbol table.
 */
void
sym_table_delete(sym_table_t* table);

void
sym_table_add_import(sym_table_t* table,
                     sym_table_t* imported_symbols);

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
sym_table_get_symbol(sym_table_t *table, const char *name, GError **error);

/**
 * @return all symbols in this table as a list of IrSymbol pointers,
 *         the list should freed, when not needed anymore,
 *         by calling g_list_free() 
 */
GList *
sym_table_get_all_symbols(sym_table_t* table);

#endif /* SYM_TABLE_INC_X */
