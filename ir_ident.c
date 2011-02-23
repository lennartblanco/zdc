#include "ir_ident.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_ident_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrIdentClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrIdent),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrIdentType",
                                    &info, 0);
    }
    return type;
}

IrIdent *
ir_ident_new(const gchar *name, guint line_number)
{
    assert(name);

    IrIdent *obj;

    obj = g_object_new(IR_TYPE_IDENT,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->name = g_strdup(name);

    return obj;
}

const gchar *
ir_ident_get_name(IrIdent *self)
{
    assert(IR_IS_IDENT(self));

    return self->name;
}
