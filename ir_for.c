#include "ir_for.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_for_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrForClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrFor),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LOOP,
                                    "IrForType",
                                    &info, 0);
    }
    return type;
}

IrFor *
ir_for_new(IrCodeBlock *init,
           IrExpression *test,
           IrExpression *step,
           IrCodeBlock *body)
{
    assert(IR_IS_CODE_BLOCK(init));
    assert(IR_IS_EXPRESSION(test) || test == NULL);
    assert(IR_IS_EXPRESSION(step) || step == NULL);
    assert(IR_IS_CODE_BLOCK(body));

    IrFor *obj;

    obj = g_object_new(IR_TYPE_FOR, NULL);

    obj->init = init;
    obj->test = test;
    obj->step = step;
    obj->body = body;

    return obj;
}

IrCodeBlock *
ir_for_get_init(IrFor *self)
{
    assert(IR_IS_FOR(self));

    return self->init;
}

IrExpression *
ir_for_get_test(IrFor *self)
{
    assert(IR_IS_FOR(self));

    return self->test;
}


IrExpression *
ir_for_get_step(IrFor *self)
{
    assert(IR_IS_FOR(self));

    return self->step;
}


IrCodeBlock *
ir_for_get_body(IrFor *self)
{
    assert(IR_IS_FOR(self));

    return self->body;
}

