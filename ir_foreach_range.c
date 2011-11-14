#include "ir_foreach_range.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_foreach_range_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrForeachRangeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrForeachRange),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LOOP,
                                    "IrForeachRangeType",
                                    &info, 0);
    }
    return type;
}

IrForeachRange *
ir_foreach_range_new(IrVariable *index,
                     IrExpression *lower_exp,
                     IrExpression *upper_exp,
                     IrCodeBlock *body)
{
    assert(IR_IS_VARIABLE(index));
    assert(IR_IS_EXPRESSION(lower_exp));
    assert(IR_IS_EXPRESSION(upper_exp));
    assert(IR_IS_CODE_BLOCK(body));

    IrForeachRange *obj;

    obj = g_object_new(IR_TYPE_FOREACH_RANGE, NULL);

    obj->index = ir_var_value_new(index, ir_node_get_line_num(index));
    obj->lower_exp = lower_exp;
    obj->upper_exp = upper_exp;
    obj->body = body;

    return obj;
}

IrVarValue *
ir_foreach_range_get_index(IrForeachRange *self)
{
    assert(IR_IS_FOREACH_RANGE(self));

    return self->index;
}

IrExpression *
ir_foreach_range_get_lower_exp(IrForeachRange *self)
{
    assert(IR_IS_FOREACH_RANGE(self));

    return self->lower_exp;
}

IrExpression *
ir_foreach_range_get_upper_exp(IrForeachRange *self)
{
    assert(IR_IS_FOREACH_RANGE(self));

    return self->upper_exp;
}

IrCodeBlock *
ir_foreach_range_get_body(IrForeachRange *self)
{
    assert(IR_IS_FOREACH_RANGE(self));

    return self->body;
}
