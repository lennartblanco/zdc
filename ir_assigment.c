#include "ir_assigment.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_assigment_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrAssigmentClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrAssigment),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrAssigmentType",
                                    &info, 0);
    }
    return type;
}

IrAssigment *
ir_assigment_new(IrLvalue *lvalue,
                 IrExpression *value)
{
    IrAssigment *obj;

    obj = g_object_new(IR_TYPE_ASSIGMENT, NULL);

    obj->lvalue = lvalue;
    obj->value = value;

    return obj;
}

IrLvalue *
ir_assigment_get_lvalue(IrAssigment *self)
{
    assert(IR_IS_ASSIGMENT(self));

    return self->lvalue;
}

void
ir_assigment_set_lvalue(IrAssigment *self, IrLvalue *lvalue)
{
    assert(IR_IS_ASSIGMENT(self));
    assert(IR_IS_LVALUE(lvalue));

    self->lvalue = lvalue;
}

IrExpression *
ir_assigment_get_value(IrAssigment *self)
{
    assert(IR_IS_ASSIGMENT(self));

    return self->value;
}

void
ir_assigment_set_value(IrAssigment *self, IrExpression *value)
{
    assert(self);
    assert(IR_IS_ASSIGMENT(self));

    self->value = value;
}
