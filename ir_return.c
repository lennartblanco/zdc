#include "ir_return.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_return_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrReturnClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrReturn),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrReturnType",
                                    &info, 0);
    }
    return type;
}

IrReturn *
ir_return(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IR_TYPE_RETURN, IrReturn);
}

IrReturn *
ir_return_new(IrExpression *return_value, guint line_number)
{
    IrReturn *obj;

    obj = g_object_new(IR_TYPE_RETURN,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->return_value = return_value;

    return obj;
}

void
ir_return_set_return_value(IrReturn *self, IrExpression *return_value)
{
    assert(IR_IS_RETURN(self));

    self->return_value = return_value;
}

IrExpression *
ir_return_get_return_value(IrReturn *self)
{
    assert(IR_IS_RETURN(self));

    return self->return_value;
}
