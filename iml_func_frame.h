#ifndef IML_FUNC_FRAME_INC_X
#define IML_FUNC_FRAME_INC_X

#include <stdio.h>

#include "iml_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/


typedef struct iml_func_frame_s iml_func_frame_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_func_frame_t *
iml_func_frame_new(void);

/**
 * add local variable to the frame
 */
void
iml_func_frame_add_local(iml_func_frame_t *self, ImlVariable *variable);

/**
 * get a list of local variables of given data type
 */
GSList *
iml_func_frame_get_locals(iml_func_frame_t *self,
                          iml_data_type_t vars_type);

/**
 * add function parameter
 */
void
iml_func_frame_add_parameter(iml_func_frame_t *self, ImlVariable *variable);

GSList *
iml_func_frame_get_parameters(iml_func_frame_t *self);

/**
 * Get a temporary variable in the function frame.
 * If datatype is iml_blob, the size of the blob must be specified as an
 * guint parameter after datatype.
 */
ImlVariable *
iml_func_frame_get_temp(iml_func_frame_t *self, iml_data_type_t datatype, ...);

/**
 * If specified operand is a temporary variable, it will be marked as
 * not used any more. Unused variables will be later reused by
 * iml_func_frame_get_temp() if possible.
 *
 * If oper is not a temporary variable, nothing will be done.
 */
void
iml_func_frame_unsed_oper(iml_func_frame_t *self, ImlOperand *oper);

/**
 * Size in bytes that this function frames need to store all
 * it's variables.
 */
void
iml_func_frame_set_size(iml_func_frame_t *self, guint stack_size);

guint
iml_func_frame_get_size(iml_func_frame_t *self);

/**
 * Stores the list of used preserved registers.
 */
void
iml_func_frame_set_used_regs(iml_func_frame_t *self,
                             GSList *preserved_regs);

/**
 * Get the list of used preserved register, registers
 * which must be preserved across function calls.
 */
GSList *
iml_func_frame_get_used_regs(iml_func_frame_t *self);

/**
 * pretty print this function frame
 */
void
iml_func_frame_print(iml_func_frame_t *self, FILE *out, int indention);

#endif /* IML_FUNC_FRAME_INC_X */
