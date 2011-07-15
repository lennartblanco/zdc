#ifndef IR_CONDITIONAL_INC_X
#define IR_CONDITIONAL_INC_X

#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_CONDITIONAL ir_conditional_get_type()

#define IR_CONDITIONAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_CONDITIONAL, IrConditional))

#define IR_CONDITIONAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_CONDITIONAL, IrConditionalClass))

#define IR_IS_CONDITIONAL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_CONDITIONAL))

#define IR_IS_CONDITIONAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_CONDITIONAL))

#define IR_CONDITIONAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_CONDITIONAL, IrConditionalClass))

typedef struct
{
    IrExpression parent;

    /* private */
    IrExpression *condition;
    IrExpression *true_exp;
    IrExpression *false_exp;
} IrConditional;

typedef struct
{
    IrExpressionClass parent_class;
} IrConditionalClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_conditional_get_type(void);

IrConditional *
ir_conditional_new(IrExpression *condition,
                   IrExpression *true_exp,
                   IrExpression *false_exp,
                   guint line_number);

IrExpression *
ir_conditional_get_cond(IrConditional *self);

IrExpression *
ir_conditional_get_true(IrConditional *self);

IrExpression *
ir_conditional_get_false(IrConditional *self);

/**
 * Set new conditional, true and false expressions.
 */
void
ir_conditional_set_expressions(IrConditional *self,
                               IrExpression *condition,
                               IrExpression *true_exp,
                               IrExpression *false_exp);

#endif /* IR_CONDITIONAL_INC_X */
