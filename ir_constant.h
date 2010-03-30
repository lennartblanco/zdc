#ifndef IR_CONSTANT_INC_X
#define IR_CONSTANT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_CONSTANT ir_constant_get_type()

#define IR_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_CONSTANT, IrConstant))

#define IR_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_CONSTANT, IrConstantClass))

#define IR_IS_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_CONSTANT))

#define IR_IS_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_CONSTANT))

#define IR_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_CONSTANT, IrConstantClass))

typedef struct 
{
  IrExpression parent;
} IrConstant;

typedef struct 
{
  IrExpressionClass parent_class;
} IrConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_constant_get_type(void);

#endif /* IR_CONSTANT_INC_X */
