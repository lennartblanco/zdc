#include "ir_assignment.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_assignment_class_init(gpointer klass, gpointer dummy);

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
        ir_assignment_class_init, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrAssignment),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrAssignmentType",
                                    &info, 0);
    }
    return type;
}

IrAssignment *
ir_assignment(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IR_TYPE_ASSIGNMENT, IrAssignment);
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

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_assignment_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ASSIGNMENT(self));

    return ir_expression_get_data_type(ir_assignment(self)->lvalue);
}

static bool
ir_assignment_do_has_effect(IrExpression *self)
{
    assert(IR_IS_ASSIGNMENT(self));

    return true;
}

static void
ir_assignment_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_assignment_do_get_data_type;
    IR_EXPRESSION_CLASS(klass)->has_effect = ir_assignment_do_has_effect;
}
