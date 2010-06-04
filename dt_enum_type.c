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

static IrExpression *
dt_enum_type_get_init(DtDataType *self);

static bool
dt_enum_type_is_same(DtDataType *self, DtDataType *type);

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
dt_enum_type_new(gchar *name,
                 DtDataType *base_type)
{
    DtEnumType *obj;

    obj = g_object_new(DT_TYPE_ENUM_TYPE, NULL);

    obj->name = g_strdup(name);
    obj->base_type = base_type;
    obj->first_member = NULL;

    return obj;
}

void
dt_enum_type_set_first_member(DtEnumType *self,
                              IrEnumMember *first_member)
{
    assert(DT_IS_ENUM_TYPE(self));
    assert(IR_IS_ENUM_MEMBER(first_member));

    self->first_member = first_member;
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

static IrExpression *
dt_enum_type_get_init(DtDataType *self)
{
    assert(DT_IS_ENUM_TYPE(self));
    assert(IR_IS_EXPRESSION(DT_ENUM_TYPE(self)->first_member));

    return IR_EXPRESSION(DT_ENUM_TYPE(self)->first_member);
}

static bool
dt_enum_type_is_same(DtDataType *self, DtDataType *type)
{
    assert(DT_IS_ENUM_TYPE(self));

    if (!DT_IS_ENUM_TYPE(type))
    {
        return false;
    }

    DtEnumType *l = DT_ENUM_TYPE(self);
    DtEnumType *r = DT_ENUM_TYPE(type);

    return g_strcmp0(l->name, r->name) == 0;
}

static void
dt_enum_type_class_init(gpointer klass, gpointer dummy)
{
    ((DtDataTypeClass *)klass)->get_size = dt_enum_type_get_size;
    ((DtDataTypeClass *)klass)->get_string = dt_enum_type_get_string;
    ((DtDataTypeClass *)klass)->get_mangled = dt_enum_type_get_mangled;
    ((DtDataTypeClass *)klass)->get_init = dt_enum_type_get_init;
    ((DtDataTypeClass *)klass)->is_same = dt_enum_type_is_same;
}

