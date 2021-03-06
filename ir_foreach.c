#include "ir_foreach.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_foreach_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrForeachClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrForeach),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LOOP,
                                    "IrForeachType",
                                    &info, 0);
    }
    return type;
}

IrForeach *
ir_foreach(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IR_TYPE_FOREACH, IrForeach);
}

IrForeach *
ir_foreach_new(IrVariable *index,
               IrVariable *value,
               IrExpression *aggregate,
               IrCodeBlock *body,
               guint line_number)
{
    IrForeach *obj;

    obj = g_object_new(IR_TYPE_FOREACH,
                       "ir-node-line-number", line_number,
                       NULL);

    obj->index = index;
    obj->value = value;
    obj->aggregate = aggregate;
    obj->body = body;

    return obj;
}

void
ir_foreach_set_index(IrForeach *self, IrVariable *index)
{
    assert(IR_IS_FOREACH(self));
    assert(IR_IS_VARIABLE(index));

    self->index = index;
}

IrVariable *
ir_foreach_get_index(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

    return self->index;
}

IrVariable *
ir_foreach_get_value(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

   return self->value;
}

void
ir_foreach_set_aggregate(IrForeach *self, IrExpression *aggregate)
{
    assert(IR_IS_FOREACH(self));
    assert(IR_IS_EXPRESSION(aggregate));

    self->aggregate = aggregate;
}

IrExpression *
ir_foreach_get_aggregate(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

    return self->aggregate;
}

IrCodeBlock *
ir_foreach_get_body(IrForeach *self)
{
    assert(IR_IS_FOREACH(self));

    return self->body;
}
