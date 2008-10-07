#include <stdio.h>

#include "sem_analyze.h"

void
analyze_function(ast_node_t *node)
{
    printf("w(e)[e] %p\n", node);
}

void
semantic_analyze(sym_table_t *global_sym_table)
{
    sym_table_foreach_sym(global_sym_table, analyze_function);
}
