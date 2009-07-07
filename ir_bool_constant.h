#ifndef IR_BOOL_CONSTANT_INC_X
#define IR_BOOL_CONSTANT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_BOOL_CONSTANT ir_bool_constant_get_type()

#define IR_BOOL_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_BOOL_CONSTANT, IrBoolConstant))

#define IR_BOOL_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_BOOL_CONSTANT, IrBoolConstantClass))

#define IR_IS_BOOL_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_BOOL_CONSTANT))

#define IR_IS_BOOL_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_BOOL_CONSTANT))

#define IR_BOOL_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_BOOL_CONSTANT, IrBoolConstantClass))

typedef struct
{
    IrExpression parent;
    /* private */
    gboolean value;
} IrBoolConstant;

typedef struct
{
    IrExpressionClass parent_class;
} IrBoolConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_bool_constant_get_type(void);

IrBoolConstant *
ir_bool_constant_new(gboolean value);

#endif /* IR_INT_CONSTANT_INC_X */
