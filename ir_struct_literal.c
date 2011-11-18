#include "ir_struct_literal.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_struct_literal_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_struct_literal_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrStructLiteralClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_struct_literal_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrStructLiteral),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LITERAL,
                                    "IrStructLiteralType",
                                    &info, 0);
    }
    return type;
}

bool
ir_is_struct_literal(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_TYPE(obj, IR_TYPE_STRUCT_LITERAL);
}

IrStructLiteral *
ir_struct_literal_new(GSList *members)
{
    IrStructLiteral *obj;

    obj = g_object_new(IR_TYPE_STRUCT_LITERAL, NULL);

    obj->members = members;

    return obj;
}

GSList *
ir_struct_literal_get_members(IrStructLiteral *self)
{
    assert(ir_is_struct_literal(self));

    return self->members;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

bool
ir_struct_literal_is_constant(IrExpression *self)
{
    return true;
}

static void
ir_struct_literal_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_is_constant =
        ir_struct_literal_is_constant;
}
