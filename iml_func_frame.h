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
iml_func_frame_add_local(iml_func_frame_t *self, iml_variable_t *variable);

/**
 * add function parameter
 */
void
iml_func_frame_add_parameter(iml_func_frame_t *self, iml_variable_t *variable);

/**
 * pretty print this function frame
 */
void
iml_func_frame_print(iml_func_frame_t *self, FILE *out, int indention);

#endif /* IML_FUNC_FRAME_INC_X */
