#ifndef SEM_ANALYZE_INC_X
#define SEM_ANALYZE_INC_X

#include "sym_table.h"

typedef struct ir_function_def_s
{
    sym_table_t *parameters;
    sym_table_t *local;
    ast_node_t *body;
} ir_function_def_t;

typedef struct ir_compile_unit_s
{
    sym_table_t *global_sym_table;
} ir_compile_unit_t;


/******************************
 * IR Compile Unit operations *
 ******************************/

/**
 * constructor
 */
ir_compile_unit_t*
new_ir_compile_init();

/**
 * add a function definition to this compile unit
 */
void
ir_compile_init_add_function(ir_compile_unit_t *compile_unit,
                             ir_function_def_t *function_def);

/*************************************
 * IR function definition operations *
 *************************************/

/**
 * constructor
 */
ir_function_def_t*
new_ir_function_def();


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
