/**
 * @file java_trgt_int.h
 *
 * gen_java module's internal definitions
 */

#include <stdio.h>
#include <stdbool.h>

#include "sym_table.h"
#include "ast_return.h"
#include "ast_assigment.h"
#include "ast_unary_operation.h"
#include "ast_binary_operation.h"
#include "ast_scalar_variable_ref.h"
#include "ast_function_call.h"
#include "ast_if_else.h"

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
java_trgt_data_type_to_str(AstDataType *data_type);

static void
java_trgt_handle_node(java_trgt_comp_params_t *params,
                      AstNode *node, sym_table_t *sym_table);

static void
java_trgt_handle_expression(java_trgt_comp_params_t *params,
                            AstExpression *exp, 
                            sym_table_t *sym_table);

static void
java_trgt_handle_code_block(java_trgt_comp_params_t *params,
                            AstCodeBlock *code_block,
                            sym_table_t *sym_table);

static void
java_trgt_handle_function_def(java_trgt_comp_params_t *params,
                              ir_function_def_t *func);

static void
java_trgt_handle_if_else(java_trgt_comp_params_t *params, 
                         AstIfElse *node, 
                         sym_table_t *sym_table);
static void
java_trgt_handle_return_statment(java_trgt_comp_params_t *params, 
                                 AstReturn *ret, 
                                 sym_table_t *sym_table);

static void
java_trgt_handle_scalar_var_value(java_trgt_comp_params_t *params,
                                  AstScalarVariableRef *var_ref,
                                  sym_table_t *sym_table);

static void
java_trgt_handle_assigment(java_trgt_comp_params_t *params,
                           AstAssigment *node,
                           sym_table_t *sym_table);

static void
java_trgt_handle_unary_op(java_trgt_comp_params_t *params,
                          AstUnaryOperation *operation,
                          sym_table_t *sym_table);

static void
java_trgt_handle_binary_op(java_trgt_comp_params_t *params,
                           AstBinaryOperation *operation,
                           sym_table_t *sym_table);

static void
java_trgt_handle_func_call(java_trgt_comp_params_t *params,
                           AstFunctionCall *fun_call,
                           sym_table_t *sym_table,
                           bool pop_return_value);

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
                        ast_binary_op_type_t type);

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


