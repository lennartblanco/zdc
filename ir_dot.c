#include "ir_dot.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_dot_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrDotClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrDot),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrDotType",
                                    &info, 0);
    }
    return type;
}

IrDot *
ir_dot_new(IrExpression *left, IrExpression *right, guint line_number)
{
    assert(IR_IS_EXPRESSION(left));
    assert(IR_IS_EXPRESSION(right));

    IrDot *obj;

    obj = g_object_new(IR_TYPE_DOT,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->left = left;
    obj->right = right;

    return obj;
}

IrExpression *
ir_dot_get_left(IrDot *self)
{
    assert(IR_IS_DOT(self));

    return self->left;
}

IrExpression *
ir_dot_get_right(IrDot *self)
{
    assert(IR_IS_DOT(self));

    return self->right;
}

