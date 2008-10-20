#ifndef SEM_ANALYZE_INC_X
#define SEM_ANALYZE_INC_X

#include "sym_table.h"
#include "ir.h"


/********
 * MISC *
 ********/

/**
 * Preform semantic analysis of the provided compile unit in AST-form
 * and return IR representation of the compile unit analyzed.
 */
ir_compile_unit_t *
semantic_analyze(ast_node_t *node);

#endif /* SEM_ANALYZE_INC_X */
