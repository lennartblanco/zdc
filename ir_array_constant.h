#ifndef IR_ARRAY_CONSTANT_INC_X
#define IR_ARRAY_CONSTANT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_ARRAY_CONSTANT ir_array_constant_get_type()

#define IR_ARRAY_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_ARRAY_CONSTANT, IrArrayConstant))

#define IR_ARRAY_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_ARRAY_CONSTANT, IrArrayConstantClass))

#define IR_IS_ARRAY_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_ARRAY_CONSTANT))

#define IR_IS_ARRAY_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_ARRAY_CONSTANT))

#define IR_ARRAY_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_ARRAY_CONSTANT, IrArrayConstantClass))

typedef struct
{
    IrExpression parent;

    /* private */
    GSList *values;
} IrArrayConstant;

typedef struct
{
    IrExpressionClass parent_class;
} IrArrayConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_constant_get_type(void);

IrArrayConstant *
ir_array_constant_new();

void
ir_array_constant_add_value(IrArrayConstant *self, IrExpression *value);

#endif /* IR_ARRAY_CONSTANT_INC_X */
