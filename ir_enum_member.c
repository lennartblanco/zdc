#include "ir_enum_member.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_enum_member_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrEnumMemberClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, //ir_variable_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrEnumMember),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_SYMBOL,
                                    "IrEnumMemberType",
                                    &info, 0);
    }
    return type;
}

IrEnumMember *
ir_enum_member_new(gchar *name, IrExpression *value)
{
    IrEnumMember *obj;

    obj = g_object_new(IR_TYPE_ENUM_MEMBER,
                       "ir-symbol-name", name,
                       NULL);

    obj->value = value;

    return obj;
}

IrExpression *
ir_enum_member_get_value(IrEnumMember *self)
{
    assert(IR_IS_ENUM_MEMBER(self));

    return self->value;
}

void
ir_enum_member_set_value(IrEnumMember *self, IrExpression *value)
{
    assert(IR_IS_ENUM_MEMBER(self));
    assert(IR_IS_EXPRESSION(value));

    self->value = value;
}

