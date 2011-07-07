#include "ir_break.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_break_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrBreakClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrBreak),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrBreakType",
                                    &info, 0);
    }
    return type;
}

IrBreak *
ir_break_new(guint line_number)
{
    IrBreak *obj;

    obj = g_object_new(IR_TYPE_BREAK,
                       "ir-node-line-number", line_number,
                       NULL);

    return obj;
}
