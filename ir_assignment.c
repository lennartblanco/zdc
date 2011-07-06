#include "ir_assignment.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_assignment_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (IrAssignmentClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrAssignment),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrAssignmentType",
                                    &info, 0);
    }
    return type;
}

IrAssignment *
ir_assignment_new(IrExpression *lvalue,
                  IrExpression *value,
                  guint line_number)
{
    IrAssignment *obj;

    assert(IR_IS_EXPRESSION(lvalue));
    assert(IR_IS_EXPRESSION(value));

    obj = g_object_new(IR_TYPE_ASSIGNMENT,
                       "ir-node-line-number", line_number,
                       NULL);

    obj->lvalue = lvalue;
    obj->value = value;

    return obj;
}

IrExpression *
ir_assignment_get_lvalue(IrAssignment *self)
{
    assert(IR_IS_ASSIGNMENT(self));

    return self->lvalue;
}

void
ir_assignment_set_lvalue(IrAssignment *self, IrExpression *lvalue)
{
    assert(IR_IS_ASSIGNMENT(self));
    assert(IR_IS_EXPRESSION(lvalue));
    assert(ir_expression_is_lvalue(lvalue));

    self->lvalue = lvalue;
}

IrExpression *
ir_assignment_get_value(IrAssignment *self)
{
    assert(IR_IS_ASSIGNMENT(self));

    return self->value;
}

void
ir_assignment_set_value(IrAssignment *self, IrExpression *value)
{
    assert(IR_IS_ASSIGNMENT(self));

    self->value = value;
}
