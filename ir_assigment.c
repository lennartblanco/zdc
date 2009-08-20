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
ir_assigment_new(AstVariableRef *target_ref,
                 IrExpression *value)
{
    IrAssigment *obj;

    obj = g_object_new(IR_TYPE_ASSIGMENT, NULL);

    obj->target_ref = target_ref;
    obj->target = NULL;
    obj->value = value;

    return obj;
}

AstVariableRef *
ir_assigment_get_target_ref(IrAssigment *self)
{
    assert(self);
    assert(IR_IS_ASSIGMENT(self));

    return self->target_ref;
}

IrVariable *
ir_assigment_get_target(IrAssigment *self)
{
    assert(self);
    assert(IR_IS_ASSIGMENT(self));

    return self->target;
}

void
ir_assigment_set_target(IrAssigment *self, IrVariable *target)
{
    assert(self);
    assert(IR_IS_ASSIGMENT(self));
    assert(target);
    assert(IR_IS_VARIABLE(target));

    self->target = target;
}

IrExpression *
ir_assigment_get_value(IrAssigment *self)
{
    assert(self);
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
