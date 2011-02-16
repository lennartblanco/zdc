#include "dt_struct.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_struct_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_struct_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtStructClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_struct_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtStruct),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_USER,
                                    "DtStructTypeType",
                                    &info, 0);
    }
    return type;
}

DtStruct *
dt_struct_new(gchar *name, IrModule *parent_module)
{
    DtStruct *obj;

    obj = g_object_new(DT_TYPE_STRUCT,
                       "dt-user-name", name,
                       "dt-user-parent-module", parent_module,
                       NULL);

    return obj;
}

void
dt_struct_set_member_types(DtStruct *self, GSList *member_types)
{
    assert(DT_IS_STRUCT(self));

    GSList *i;

    /* sum the byte size of all members, to get the size of this struct */
    for (i = member_types; i != NULL; i = g_slist_next(i))
    {
        self->size += dt_data_type_get_size(i->data);
    }
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static guint
dt_struct_get_size(DtDataType *self)
{
    assert(DT_IS_STRUCT(self));

    return DT_STRUCT(self)->size;
}

static gchar *
dt_struct_get_mangled_prefix(DtUser *self)
{
    assert(DT_IS_STRUCT(self));

    return "S";
}

static void
dt_struct_class_init(gpointer klass, gpointer dummy)
{
    ((DtUserClass *)klass)->get_mangled_prefix = dt_struct_get_mangled_prefix;
    ((DtDataTypeClass *)klass)->get_size = dt_struct_get_size;
}
