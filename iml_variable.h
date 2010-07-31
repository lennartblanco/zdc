#ifndef IML_VARIABLE_INC_X
#define IML_VARIABLE_INC_X

#include <stdio.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef struct iml_variable_s iml_variable_t;

typedef enum iml_data_type_e
{
    iml_8b,
    iml_16b,
    iml_32b
} iml_data_type_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

iml_variable_t *
iml_variable_new(iml_data_type_t data_type);

iml_data_type_t
iml_variable_get_data_type(iml_variable_t *self);

void
iml_variable_print(iml_variable_t *self, FILE *out, int indention);

#endif /* IML_VARIABLE_INC_X */
