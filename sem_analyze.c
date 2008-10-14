#include <stdio.h>

#include "sem_analyze.h"

void
sem_analyze_function(ast_node_t *node, void *comp_unit)
{
    printf("func %s %p\n", node->data.function_def.name, comp_unit);
}

ir_compile_unit_t*
new_ir_compile_init()
{
    ir_compile_unit_t *comp_unit;

    comp_unit = g_malloc(sizeof(*comp_unit));
    comp_unit->global_sym_table = sym_table_new(NULL);

    return comp_unit;
}


ir_compile_unit_t *
semantic_analyze(ast_node_t *node)
{
    ir_compile_unit_t *comp_unit;

    comp_unit = new_ir_compile_init();

    compile_unit_for_each_function(node, sem_analyze_function, comp_unit);

    return comp_unit;
}


ir_function_def_t*
new_ir_function_def()
{
    ir_function_def_t *func_def;

    func_def = g_malloc(sizeof(*func_def));

    func_def->parameters = sym_table_new(NULL);
    func_def->local = sym_table_new(NULL);
    func_def->body = NULL;

    return func_def;
}

