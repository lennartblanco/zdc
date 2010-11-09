#include "ir_null.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_null_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrNullClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrNull),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrNullType",
                                    &info, 0);
    }
    return type;
}

IrNull *
ir_null_new()
{
    return g_object_new(IR_TYPE_NULL, NULL);
}
