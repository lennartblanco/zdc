/**
 * @file java_trgt_int.h
 *
 * gen_java module's internal definitions
 */

#include <stdio.h>

#include "ast.h"
#include "sym_table.h"

#ifndef JAVA_TRGT_INT_INC_X
#define JAVA_TRGT_INT_INC_X

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#ifndef STATIC
#define STATIC static
#endif

#define MAX_JAVA_LABEL 16

typedef struct java_trgt_comp_params_s
{
    FILE *out;
    const char *class_name;
    char next_label[MAX_JAVA_LABEL];
} java_trgt_comp_params_t;

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static char *
java_trgt_data_type_to_str(ast_data_type_t t);

static void
java_trgt_handle_node(java_trgt_comp_params_t *params,
                     ast_node_t *node, sym_table_t *sym_table);

static void
java_trgt_handle_function_def(java_trgt_comp_params_t *params,
                             ir_function_def_t *func);

static void
java_trgt_handle_return_statment(java_trgt_comp_params_t *params, 
                                ast_node_t *node, 
                                sym_table_t *sym_table);

static void
java_trgt_handle_var_value(java_trgt_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
java_trgt_handle_assigment(java_trgt_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
java_trgt_handle_binary_op(java_trgt_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
java_trgt_handle_func_call(java_trgt_comp_params_t *params,
                          ast_node_t *node,
                          sym_table_t *sym_table);

static void
java_trgt_prelude(java_trgt_comp_params_t *params);

static void
java_trgt_epilog(java_trgt_comp_params_t *params);

/**
  * Generate the body for integer comparison operations,
  * e.g. <, >, !=, ==, etc
  */
static void
java_trgt_comp_ops_body(java_trgt_comp_params_t *params,
                       ast_oper_type_t type);

/**
 * Get next unique label
 *
 * @param label the generated label is into this buffer,
                this buffer must be at least MAX_JAVA_LABEL bytes long
 */
STATIC void
java_trgt_get_next_label(java_trgt_comp_params_t *params,
                        char *label);

#endif /* JAVA_TRGT_INT_INC_X */


