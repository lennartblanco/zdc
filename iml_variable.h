#ifndef IML_VARIABLE_INC_X
#define IML_VARIABLE_INC_X

#include "iml_operand.h"
#include "iml_register.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IML_TYPE_VARIABLE iml_variable_get_type()

#define IML_VARIABLE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IML_TYPE_VARIABLE, ImlVariable))

#define IML_VARIABLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IML_TYPE_VARIABLE, ImlVariableClass))

#define IML_IS_VARIABLE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IML_TYPE_VARIABLE))

#define IML_IS_VARIABLE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IML_TYPE_VARIABLE))

#define IML_VARIABLE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IML_TYPE_VARIABLE, ImlVariableClass))

typedef struct 
{
    ImlOperand parent;

    /* private */

    gchar *name;
    iml_data_type_t datatype;
    /*
     * the register allocation, or NULL if no register is allocated
     * for this variable
     */
    iml_register_t *reg;
} ImlVariable;

typedef struct 
{
    ImlOperandClass parent_class;
} ImlVariableClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
iml_variable_get_type(void);

/**
 * @param name variable name to use when pretty printing,
 *        should be NULL for temporary variables
 */
ImlVariable *
iml_variable_new(iml_data_type_t data_type, const gchar *name);

iml_data_type_t
iml_variable_get_data_type(ImlVariable *self);

void
iml_variable_set_register(ImlVariable *self, iml_register_t *reg);

#endif /* IML_VARIABLE_INC_X */
