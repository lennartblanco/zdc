#ifndef IML_REGISTER_INC_X
#define IML_REGISTER_INC_X

#include <stdio.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef struct iml_register_s iml_register_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * @param id backend specific unique number for the register
 * @param name pretty print name of the register
 */
iml_register_t *
iml_register_new(guint id, const gchar *name);

const gchar *
iml_register_get_name(iml_register_t *self);

void
iml_register_print(iml_register_t *self, FILE *out, int indention);

#endif /* IML_REGISTER_INC_X */
