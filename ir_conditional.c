#include "ir_conditional.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_conditional_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_conditional_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_conditional_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrConditionalClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_conditional_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrConditional),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrConditionalType",
                                    &info, 0);
    }
    return type;
}

IrConditional *
ir_conditional_new(IrExpression *condition,
                   IrExpression *true_exp,
                   IrExpression *false_exp,
                   guint line_number)
{
    IrConditional *obj;

    obj = g_object_new(IR_TYPE_CONDITIONAL,
                       "ir-node-line-number", line_number,
                       NULL);

    obj->condition = condition;
    obj->true_exp = true_exp;
    obj->false_exp = false_exp;

    return obj;
}

IrExpression *
ir_conditional_get_cond(IrConditional *self)
{
    assert(IR_IS_CONDITIONAL(self));
    assert(IR_IS_EXPRESSION(self->condition));

    return self->condition;
}

IrExpression *
ir_conditional_get_true(IrConditional *self)
{
    assert(IR_IS_CONDITIONAL(self));
    assert(IR_IS_EXPRESSION(self->true_exp));

    return self->true_exp;
}

IrExpression *
ir_conditional_get_false(IrConditional *self)
{
    assert(IR_IS_CONDITIONAL(self));
    assert(IR_IS_EXPRESSION(self->false_exp));

    return self->false_exp;
}

void
ir_conditional_set_expressions(IrConditional *self,
                               IrExpression *condition,
                               IrExpression *true_exp,
                               IrExpression *false_exp)
{
    assert(IR_IS_CONDITIONAL(self));
    assert(IR_IS_EXPRESSION(condition));
    assert(IR_IS_EXPRESSION(true_exp));
    assert(IR_IS_EXPRESSION(false_exp));

    self->condition = condition;
    self->true_exp = true_exp;
    self->false_exp = false_exp;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_conditional_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_conditional_do_get_data_type;
}

static DtDataType *
ir_conditional_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_CONDITIONAL(self));

    return ir_expression_get_data_type(IR_CONDITIONAL(self)->true_exp);
}
