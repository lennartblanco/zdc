#ifndef IR_CHAR_CONSTANT_INC_X
#define IR_CHAR_CONSTANT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_CHAR_CONSTANT ir_char_constant_get_type()

#define IR_CHAR_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_CHAR_CONSTANT, IrCharConstant))

#define IR_CHAR_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_CHAR_CONSTANT, IrCharConstantClass))

#define IR_IS_CHAR_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_CHAR_CONSTANT))

#define IR_IS_CHAR_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_CHAR_CONSTANT))

#define IR_CHAR_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_CHAR_CONSTANT, IrCharConstantClass))

typedef struct
{
    IrExpression parent;
    /* private */
    guint8 value;
} IrCharConstant;

typedef struct
{
    IrExpressionClass parent_class;
} IrCharConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_char_constant_get_type(void);

IrCharConstant *
ir_char_constant_new(guint8 value, guint line_number);

guint8
ir_char_constant_get_value(IrCharConstant *self);

#endif /* IR_CHAR_CONSTANT_INC_X */
