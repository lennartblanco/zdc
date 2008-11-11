/**
 * @file gen_java_int.h
 *
 * gen_java module's internal definitions
 */

#include <stdio.h>

#include "ast.h"
#include "sym_table.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#ifndef STATIC
#define STATIC static
#endif

#define MAX_JAVA_LABEL 16

typedef struct gen_java_comp_params_s
{
    FILE *out;
    const char *class_name;
    char next_label[MAX_JAVA_LABEL];
} gen_java_comp_params_t;

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static char *
gen_java_data_type_to_str(ast_data_type_t t);

static void
gen_java_handle_node(gen_java_comp_params_t *params,
                     ast_node_t *node, sym_table_t *sym_table);

static void
gen_java_handle_function_def(gen_java_comp_params_t *params,
                             ir_function_def_t *func);

static void
gen_java_handle_return_statment(gen_java_comp_params_t *params, 
                                ast_node_t *node, 
                                sym_table_t *sym_table);

static void
gen_java_handle_var_value(gen_java_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
gen_java_handle_assigment(gen_java_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
gen_java_handle_binary_op(gen_java_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
gen_java_handle_func_call(gen_java_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
gen_java_prelude(gen_java_comp_params_t *params);

static void
gen_java_epilog(gen_java_comp_params_t *params);

/**
  * Generate the body for integer comparison operations,
  * e.g. <, >, !=, ==, etc
  */
static void
gen_java_comp_ops_body(gen_java_comp_params_t *params,
                       ast_oper_type_t type);

/**
 * Get next unique label
 *
 * @param label the generated label is into this buffer,
                this buffer must be at least MAX_JAVA_LABEL bytes long
 */
STATIC void
gen_java_get_next_label(gen_java_comp_params_t *params,
                        char *label);

