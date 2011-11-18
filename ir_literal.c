#include <string.h>

#include "ir_literal.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_literal_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrLiteralClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrLiteral),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrLiteralType",
                                    &info, 0);
    }
    return type;
}

IrLiteral *
ir_literal(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IR_TYPE_LITERAL, IrLiteral);
}

void
ir_literal_set_data_label(IrLiteral *self, char *label)
{
    assert(IR_IS_LITERAL(self));
    assert(label || strlen(label) > 0);

    self->data_label = g_strdup(label);
}

char *
ir_literal_get_data_label(IrLiteral *self)
{
    assert(IR_IS_LITERAL(self));

    return self->data_label;
}

