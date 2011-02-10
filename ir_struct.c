#include "ir_struct.h"
#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_struct_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrStructClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL, /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrStruct),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_NODE,
                                    "IrStructType",
                                    &info, 0);
    }
    return type;
}

IrStruct *
ir_struct_new(gchar *name, GSList *members, sym_table_t *symbols)
{
    IrStruct *obj;

    obj = g_object_new(IR_TYPE_STRUCT, NULL);

    obj->data_type = dt_struct_new();
    obj->name = name;
    obj->members = members;
    obj->symbols = symbols;

    return obj;
}

void
ir_struct_set_members(IrStruct *self, GSList *members)
{
    assert(IR_IS_STRUCT(self));

    GSList *member_types = NULL;
    GSList *i;

    for (i = self->members; i != NULL; i = g_slist_next(i))
    {
        member_types =
                g_slist_prepend(member_types,
                                ir_variable_get_data_type(
                                    IR_VARIABLE(i->data)));
    }

    dt_struct_set_member_types(self->data_type, g_slist_reverse(member_types));

    self->members = members;
}

GSList *
ir_struct_get_members(IrStruct *self)
{
    assert(IR_IS_STRUCT(self));

    return self->members;
}

gchar *
ir_struct_get_name(IrStruct *self)
{
    assert(IR_IS_STRUCT(self));

    return self->name;
}

DtStruct *
ir_struct_get_data_type(IrStruct *self)
{
    assert(IR_IS_STRUCT(self));

    return self->data_type;
}
