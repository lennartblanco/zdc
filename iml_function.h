#ifndef IML_FUNCTION_INC_X
#define IML_FUNCTION_INC_X

#include <stdio.h>

#include "iml_operation.h"
#include "iml_variable.h"
#include "ir_function.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/


typedef struct iml_function_s iml_function_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_function_t *
iml_function_new(IrModule *parent_module, ir_linkage_type_t linkage);

IrModule *
iml_function_get_parent_module(iml_function_t *self);

ir_linkage_type_t
iml_function_get_linkage(iml_function_t *self);

void
iml_function_set_name(iml_function_t *self, const char *name);

const char *
iml_function_get_name(iml_function_t *self);

void
iml_function_add_operation(iml_function_t *self, iml_operation_t *operation);

GSList *
iml_function_get_operations(iml_function_t *self);

/**
 * add local variable to the frame
 */
void
iml_function_add_local(iml_function_t *self, ImlVariable *variable);

/**
 * get a list of local variables of given data type
 */
GSList *
iml_function_get_locals(iml_function_t *self);

/**
 * add function parameter
 */
void
iml_function_add_parameter(iml_function_t *self, ImlVariable *variable);

GSList *
iml_function_get_parameters(iml_function_t *self);

/**
 * Get a temporary variable in the function frame.
 * If datatype is iml_blob, the size of the blob must be specified as an
 * guint parameter after datatype.
 */
ImlVariable *
iml_function_get_temp(iml_function_t *self, iml_data_type_t datatype, ...);

/**
 * If specified operand is a temporary variable, it will be marked as
 * not used any more. Unused variables will be later reused by
 * iml_function_get_temp() if possible.
 *
 * If oper is not a temporary variable, nothing will be done.
 */
void
iml_function_unused_oper(iml_function_t *self, ImlOperand *oper);

/**
 * Set the size (in bytes) of this function's frame.
 */
void
iml_function_set_frame_size(iml_function_t *self, guint stack_size);

guint
iml_function_get_frame_size(iml_function_t *self);

/**
 * Stores the list of used preserved registers.
 */
void
iml_function_set_used_regs(iml_function_t *self, GSList *preserved_regs);

/**
 * Get the list of used preserved register, registers
 * which must be preserved across function calls.
 */
GSList *
iml_function_get_used_regs(iml_function_t *self);

/**
 * pretty print this function frame
 */
void
iml_function_print(iml_function_t *self, FILE *out, int indention);

#endif /* IML_FUNCTION_INC_X */
