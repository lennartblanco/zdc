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
#include "ast_function_call.h"
#include "ast_if_else.h"
#include "ast_array_cell_ref.h"
#include "ast_variable_declaration.h"
#include "ir_function.h"
#include "ir_if_else.h"

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
                            IrCodeBlock *code_block);

static void
java_trgt_handle_function_def(java_trgt_comp_params_t *params,
                              IrFunction *func);

static void
java_trgt_handle_if_else(java_trgt_comp_params_t *params, 
                         IrIfElse *if_else, 
                         sym_table_t *sym_table);
static void
java_trgt_handle_return_statment(java_trgt_comp_params_t *params, 
                                 AstReturn *ret, 
                                 sym_table_t *sym_table);

static void
java_trgt_handle_var_value(java_trgt_comp_params_t *params,
                           AstVariableRef *var_ref,
                           sym_table_t *sym_table);
/**
 * generate the code to assign the result of un expression
 * to a local variable
 *
 * @param params compilation parameters
 * @param var_num the local variables number
 * @param exp the exprassion to evaluate and assign
 * @param sym_table the symbol table
 */
static void
java_trgt_handle_var_assigment(java_trgt_comp_params_t *params,
                               guint var_num,
                               AstExpression *exp,
                               sym_table_t *sym_table);
/**
 * generate code to assign the result of an expression
 * to an array cell
 *
 * @param params compilation parameters
 * @param var_num the number of the local variable that hold
 *                a reference to the array
 * @param index the expession to evalute to get the index of the array cell
 * @param exp the exprassion to evaluate and assign
 * @param sym_table the symbol table
 */ 
static void
java_trgt_handle_array_assigment(java_trgt_comp_params_t *params,
                                 guint var_num,
                                 AstExpression *index,
                                 AstExpression *exp,
                                 sym_table_t *sym_table);

static void
java_trgt_handle_array_slice_assigment(java_trgt_comp_params_t *params,
                                       guint var_num,
                                       AstExpression *start_exp,
                                       AstExpression *val,
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
java_trgt_handle_array_cell_ref(java_trgt_comp_params_t *params,
                                AstArrayCellRef *acell,
                                sym_table_t *sym_table);

static void
java_trgt_prelude(java_trgt_comp_params_t *params);

//static void
//java_trgt_epilog(java_trgt_comp_params_t *params);

/**
 * Generate code to load constant integer value on the stack.
 *
 * @param value the value that need to be loaded on the stack
 */
static void
java_trgt_const_int(java_trgt_comp_params_t *params, int value);

/**
 * Generate the body for integer comparison operations,
 * e.g. <, >, !=, ==, etc
 */
static void
java_trgt_comp_ops_body(java_trgt_comp_params_t *params,
                        ast_binary_op_type_t type);

/**
 * Assign numbers to local variables in this code block and any
 * children code blocks.
 *
 * @param first_num the first available empty number
 * @param code_block the code block to assign numbers to
 *
 * @param the number of allocated local variable slots
 */
static int
java_trgt_code_block_assign_addrs(int first_num,
                                  IrCodeBlock *code_block);

/**
 * Assign numbers to local variables in any code blocks and sub-blocks
 * in a if-else statment.
 *
 * @param first_num the first available empty number
 * @param if_else the if-else statment to assign numbers to
 *
 * @param the number of allocated local variable slots
 */
static int
java_trgt_if_else_assign_addrs(int first_num,
                               IrIfElse *if_else);

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


