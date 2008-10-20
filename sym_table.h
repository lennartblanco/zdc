#ifndef SEM_TABLE_INC_X
#define SEM_TABLE_INC_X

#include "ast.h"
#include "ir.h"

typedef struct sym_table_s sym_table_t;


/**
 * Create a new symbol table.
 *
 * @param parent the symbol table at previous code scope, or NULL if this is
 *         the global symbol table
 */
sym_table_t*
sym_table_new(sym_table_t* parent);

void
sym_table_foreach_sym(sym_table_t *table,
                      void (*func) (ast_node_t *symp));

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
sym_table_add_symbol(sym_table_t* table,
                     char *name,
                     ir_symbol_t *symbol);

/**
 * debug function
 * print contents of the symbol table
 */
void
sym_table_dump(sym_table_t *table);

/**
 * look for a symbol in the provided table
 *
 * @param table   the table where to look for the element
 * @param name    symbol's name
 * @param symbol  if this pointer is not NULL, the found element returned here
 *
 * @return  0 if element is found in the table
 * @return -1 if no such element is present in the table
 */
int
sym_table_find_symbol(sym_table_t *table, char *name, ir_symbol_t **symbol);

#endif /* SEM_TABLE_INC_X */
