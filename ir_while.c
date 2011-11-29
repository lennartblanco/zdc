#include "ir_while.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_while_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrWhileClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrWhile),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LOOP,
                                    "IrWhileType",
                                    &info, 0);
    }
    return type;
}

IrWhile *
ir_while_new(IrExpression *loop_condition, IrCodeBlock *body)
{
    IrWhile *obj;

    obj = g_object_new(IR_TYPE_WHILE, NULL);

    obj->loop_condition = loop_condition;
    obj->body = body;

    return obj;
}

void
ir_while_set_loop_condition(IrWhile *self, IrExpression *loop_condition)
{
    assert(IR_IS_WHILE(self));
    assert(IR_IS_EXPRESSION(loop_condition));

    self->loop_condition = loop_condition;
}

IrExpression *
ir_while_get_loop_condition(IrWhile *self)
{
    assert(IR_IS_WHILE(self));

    return self->loop_condition;
}

IrCodeBlock *
ir_while_get_body(IrWhile *self)
{
    assert(IR_IS_WHILE(self));

    return self->body;
}
