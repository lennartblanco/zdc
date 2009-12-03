#ifndef IR_UINT_CONSTANT_INC_X
#define IR_UINT_CONSTANT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_UINT_CONSTANT ir_uint_constant_get_type()

#define IR_UINT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_UINT_CONSTANT, IrUintConstant))

#define IR_UINT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_UINT_CONSTANT, IrUintConstantClass))

#define IR_IS_UINT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_UINT_CONSTANT))

#define IR_IS_UINT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_UINT_CONSTANT))

#define IR_UINT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_UINT_CONSTANT, IrUintConstantClass))

typedef struct
{
    IrExpression parent;
    /* private */
    guint32 value;
} IrUintConstant;

typedef struct
{
    IrExpressionClass parent_class;
} IrUintConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_uint_constant_get_type(void);

IrUintConstant *
ir_uint_constant_new(guint32 value, guint line_number);

guint32
ir_uint_constant_get_value(IrUintConstant *self);

#endif /* IR_UINT_CONSTANT_INC_X */
