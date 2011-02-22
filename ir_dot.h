#ifndef IR_DOT_INC_X
#define IR_DOT_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_DOT ir_dot_get_type()

#define IR_DOT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_DOT, IrDot))

#define IR_DOT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_DOT, IrDotClass))

#define IR_IS_DOT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_DOT))

#define IR_IS_DOT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_DOT))

#define IR_DOT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_DOT, IrDotClass))

typedef struct
{
    IrExpression parent;

    /* private */
    IrExpression *left;
    IrExpression *right;
} IrDot;

typedef struct
{
    IrExpressionClass parent_class;
} IrDotClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_dot_get_type(void);

IrDot *
ir_dot_new(IrExpression *left, IrExpression *right, guint line_number);

IrExpression *
ir_dot_get_left(IrDot *self);

IrExpression *
ir_dot_get_right(IrDot *self);

#endif /* IR_DOT_INC_X */
