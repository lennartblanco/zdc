#include "dt_enum.h"
#include "ir_module.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_enum_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_enum_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtEnumClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_enum_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtEnum),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_USER,
                                    "DtEnumType",
                                    &info, 0);
    }
    return type;
}

DtEnum *
dt_enum_new(gchar *name,
            DtDataType *base_type,
            IrModule *parent_module)
{
    assert(IR_IS_MODULE(parent_module));

    DtEnum *obj;

    obj = g_object_new(DT_TYPE_ENUM,
                       "dt-user-name", name,
                       "dt-user-parent-module", parent_module,
                       NULL);

    obj->base_type = base_type;
    obj->first_member = NULL;

    return obj;
}

void
dt_enum_set_first_member(DtEnum *self,
                         IrEnumMember *first_member)
{
    assert(DT_IS_ENUM(self));
    assert(IR_IS_ENUM_MEMBER(first_member));

    self->first_member = first_member;
}

DtDataType *
dt_enum_get_base_type(DtEnum *self)
{
    assert(DT_IS_ENUM(self));

    return self->base_type;
}

void
dt_enum_set_base_type(DtEnum *self, DtDataType *base_type)
{
    assert(DT_IS_ENUM(self));
    assert(DT_IS_DATA_TYPE(base_type));

    self->base_type = base_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_enum_get_mangled_prefix(DtUser *self)
{
    assert(DT_IS_ENUM(self));

    return "E";
}

static guint
dt_enum_get_size(DtDataType *self)
{
    assert(DT_IS_ENUM(self));
    assert(DT_IS_DATA_TYPE(DT_ENUM(self)->base_type));

    return dt_data_type_get_size(DT_ENUM(self)->base_type);
}

static IrExpression *
dt_enum_get_init(DtDataType *self)
{
    assert(DT_IS_ENUM(self));
    assert(IR_IS_EXPRESSION(DT_ENUM(self)->first_member));

    return IR_EXPRESSION(DT_ENUM(self)->first_member);
}

static bool
dt_enum_is_impl_conv(DtDataType *self, IrExpression *expression)
{
    assert(DT_IS_ENUM(self));
    assert(IR_IS_EXPRESSION(expression));

    return false;
}

static void
dt_enum_class_init(gpointer klass, gpointer dummy)
{
    DT_USER_CLASS(klass)->get_mangled_prefix = dt_enum_get_mangled_prefix;
    DT_DATA_TYPE_CLASS(klass)->get_size = dt_enum_get_size;
    DT_DATA_TYPE_CLASS(klass)->get_init = dt_enum_get_init;
    DT_DATA_TYPE_CLASS(klass)->is_impl_conv = dt_enum_is_impl_conv;
}
