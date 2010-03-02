#ifndef IR_INT_CONSTANT_INC_X
#define IR_INT_CONSTANT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_INT_CONSTANT ir_int_constant_get_type()

#define IR_INT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_INT_CONSTANT, IrIntConstant))

#define IR_INT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_INT_CONSTANT, IrIntConstantClass))

#define IR_IS_INT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_INT_CONSTANT))

#define IR_IS_INT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_INT_CONSTANT))

#define IR_INT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_INT_CONSTANT, IrIntConstantClass))

typedef struct
{
    IrExpression parent;
    /* private */
    gint32 value;
} IrIntConstant;

typedef struct
{
    IrExpressionClass parent_class;
} IrIntConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_int_constant_get_type(void);

IrIntConstant *
ir_int_constant_new(gint32 value, guint line_number);

gint32
ir_int_constant_get_value(IrIntConstant *self);

#endif /* IR_INT_CONSTANT_INC_X */
