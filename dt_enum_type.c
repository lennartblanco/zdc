#include "dt_enum_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static guint
dt_enum_type_get_size(DtDataType *self);

static char *
dt_enum_type_get_string(DtDataType *self);

static char *
dt_enum_type_get_mangled(DtDataType *self);

static void
dt_enum_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_enum_type_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (DtEnumTypeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_enum_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtEnumType),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtEnumTypeType",
                                    &info, 0);
    }
    return type;
}

DtEnumType *
dt_enum_type_new(gchar *name, DtDataType *base_type)
{
    DtEnumType *obj;

    obj = g_object_new(DT_TYPE_ENUM_TYPE, NULL);

    obj->name = g_strdup(name);
    obj->base_type = base_type;

    return obj;
}

DtDataType *
dt_enum_type_get_base_type(DtEnumType *self)
{
    assert(DT_IS_ENUM_TYPE(self));

    return self->base_type;
}

void
dt_enum_type_set_base_type(DtEnumType *self, DtDataType *base_type)
{
    assert(DT_IS_ENUM_TYPE(self));
    assert(DT_IS_DATA_TYPE(base_type));

    self->base_type = base_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_enum_type_get_string(DtDataType *self)
{
    assert(DT_IS_ENUM_TYPE(self));

    return DT_ENUM_TYPE(self)->name;
}

static char *
dt_enum_type_get_mangled(DtDataType *self)
{
    assert(DT_IS_ENUM_TYPE(self));

    /* not implemented */
    assert(false);
}

static guint
dt_enum_type_get_size(DtDataType *self)
{
    assert(DT_IS_ENUM_TYPE(self));
    assert(DT_IS_DATA_TYPE(DT_ENUM_TYPE(self)->base_type));

    return dt_data_type_get_size(DT_ENUM_TYPE(self)->base_type);
}

static void
dt_enum_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->get_size = dt_enum_type_get_size;
    ((DtDataTypeClass *)klass)->get_string = dt_enum_type_get_string;
    ((DtDataTypeClass *)klass)->get_mangled = dt_enum_type_get_mangled;
}

