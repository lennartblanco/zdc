#ifndef IR_SCALAR_INC_X
#define IR_SCALAR_INC_X

/**
 * A reference to variable of basic data type.
 */

#include "ir_lvalue.h"
#include "ir_variable.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_SCALAR ir_scalar_get_type()

#define IR_SCALAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_SCALAR, IrScalar))

#define IR_SCALAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_SCALAR, IrScalarClass))

#define IR_IS_SCALAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_SCALAR))

#define IR_IS_SCALAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_SCALAR))

#define IR_SCALAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_SCALAR, IrScalarClass))

typedef struct
{
    IrLvalue parent;

    /* private */
    IrVariable *variable;
} IrScalar;

typedef struct
{
    IrLvalueClass parent_class;
} IrScalarClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_scalar_get_type(void);

IrScalar *
ir_scalar_new(char *variable_name);

char *
ir_scalar_get_variable_name(IrScalar *self);

void
ir_scalar_set_variable(IrScalar *self,
                       IrVariable *variable);

IrVariable *
ir_scalar_get_variable(IrScalar *self);

#endif /* IR_SCALAR_INC_X */
