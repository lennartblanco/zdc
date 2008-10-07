#ifndef AST_DBG_DUMP_INC_X 
#define AST_DBG_DUMP_INC_X

#include <stdio.h>

#include "ast.h"

void
dump_ast(ast_node_t *root);

void
dump_ast_set_outstream(FILE *stream);


#endif /* AST_DBG_DUMP_INC_X */
