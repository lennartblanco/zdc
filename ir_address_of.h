#ifndef IR_ADDRESS_OF_INC_X
#define IR_ADDRESS_OF_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ADDRESS_OF ir_address_of_get_type()

#define IR_ADDRESS_OF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ADDRESS_OF, IrAddressOf))

#define IR_ADDRESS_OF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ADDRESS_OF, IrAddressOfClass))

#define IR_IS_ADDRESS_OF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ADDRESS_OF))

#define IR_IS_ADDRESS_OF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ADDRESS_OF))

#define IR_ADDRESS_OF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ADDRESS_OF, IrAddressOfClass))

typedef struct
{
    IrExpression parent;
    /* private */
    IrExpression *expression;
    DtDataType   *type;
} IrAddressOf;

typedef struct
{
    IrExpressionClass parent_class;
} IrAddressOfClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_address_of_get_type(void);

IrAddressOf *
ir_address_of_new(IrExpression *expression, guint line_number);

IrExpression *
ir_address_of_get_expression(IrAddressOf *self);

void
ir_address_of_set_expression(IrAddressOf *self, IrExpression *expression);

#endif /* IR_ADDRESS_OF_INC_X */
