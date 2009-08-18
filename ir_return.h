#ifndef IR_RETURN_INC_X
#define IR_RETURN_INC_X

#include "ir_statment.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_RETURN ir_return_get_type()

#define IR_RETURN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_RETURN, IrReturn))

#define IR_RETURN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_RETURN, IrReturnClass))

#define IR_IS_RETURN(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_RETURN))

#define IR_IS_RETURN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_RETURN))

#define IR_RETURN_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_RETURN, IrReturnClass))

typedef struct
{
    IrStatment          parent;

    /* private */
    IrExpression  *return_value;
} IrReturn;

typedef struct
{
    IrStatmentClass parent_class;
} IrReturnClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_return_get_type(void);

IrReturn *
ir_return_new(IrExpression *return_value);

void
ir_return_set_return_value(IrReturn *self, IrExpression *return_value);

IrExpression *
ir_return_get_return_value(IrReturn *self);

#endif /* IR_RETURN_INC_X */
