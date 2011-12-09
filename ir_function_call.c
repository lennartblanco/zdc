#include <string.h>

#include "ir_function_call.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_function_call_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrFunctionCallClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrFunctionCall),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CALL,
                                    "IrFunctionCallType",
                                    &info, 0);
    }
    return type;
}

IrFunctionCall *
ir_function_call(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj),
                                      IR_TYPE_FUNCTION_CALL,
                                      IrFunctionCall);
}

IrFunctionCall *
ir_function_call_new(char *name,
                     GSList *arguments,
                     guint line_number)
{
    IrFunctionCall *obj;

    obj = g_object_new(IR_TYPE_FUNCTION_CALL,
                       "ir-node-line-number", line_number,
                       "ir-call-arguments", arguments,
                       NULL);

    obj->name = strdup(name);

    return obj;
}

ir_linkage_type_t
ir_function_call_get_linkage(IrFunctionCall *self)
{
    assert(IR_IS_FUNCTION_CALL(self));

    return ir_function_get_linkage(ir_call_get_function(IR_CALL(self)));
}

GSList *
ir_function_call_get_arguments(IrFunctionCall *self)
{
    return ir_call_get_arguments(IR_CALL(self));
}

char *
ir_function_call_get_name(IrFunctionCall *self)
{
    assert(IR_IS_FUNCTION_CALL(self));

    return self->name;
}
