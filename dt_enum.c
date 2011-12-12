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

bool
dt_is_enum(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_TYPE((obj), DT_TYPE_ENUM);
}

DtEnum *
dt_enum(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), DT_TYPE_ENUM, DtEnum);
}

DtEnum *
dt_enum_new(gchar *name,
            DtDataType *base_type,
            IrModule *parent_module,
            guint line_number)
{
    assert(IR_IS_MODULE(parent_module));

    DtEnum *obj;

    obj = g_object_new(DT_TYPE_ENUM,
                       "ir-symbol-name", name,
                       "ir-symbol-parent-module", parent_module,
                       "ir-symbol-scope", ir_module_get_scope(parent_module),
                       "ir-node-line-number", line_number,
                       NULL);

    obj->base_type = base_type;
    obj->members = NULL;

    return obj;
}

gchar *
dt_enum_get_tag(DtEnum *self)
{
    assert(dt_is_enum(self));

    return ir_symbol_get_name(ir_symbol(self));
}

bool
dt_enum_is_anonymous(DtEnum *self)
{
    assert(dt_is_enum(self));

    return dt_enum_get_tag(self) == NULL;
}

void
dt_enum_set_members(DtEnum *self, GSList *members)
{
    assert(dt_is_enum(self));
    assert(members);

    self->members = members;
}

GSList *
dt_enum_get_members(DtEnum *self)
{
    assert(dt_is_enum(self));

    return self->members;
}

IrEnumMember *
dt_enum_get_member(DtEnum *self, const gchar *enum_member_name)
{
    assert(dt_is_enum(self));
    assert(enum_member_name);
    GSList *i;

    for (i = self->members; i != NULL; i = g_slist_next(i))
    {
        if (g_strcmp0(ir_symbol_get_name(i->data), enum_member_name) == 0)
        {
            return i->data;
        }
    }

    return NULL;
}

DtDataType *
dt_enum_get_base_type(DtEnum *self)
{
    assert(dt_is_enum(self));

    return self->base_type;
}

void
dt_enum_set_base_type(DtEnum *self, DtDataType *base_type)
{
    assert(dt_is_enum(self));
    assert(DT_IS_DATA_TYPE(base_type));

    self->base_type = base_type;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_enum_get_mangled_prefix(DtUser *self)
{
    assert(dt_is_enum(self));

    return "E";
}

static guint
dt_enum_get_size(DtDataType *self)
{
    assert(dt_is_enum(self));
    assert(DT_IS_DATA_TYPE(dt_enum(self)->base_type));

    return dt_data_type_get_size(dt_enum(self)->base_type);
}

static IrExpression *
dt_enum_get_init(DtDataType *self)
{
    assert(dt_is_enum(self));
    assert(IR_IS_EXPRESSION(dt_enum(self)->members->data));

    /* init expression is first member */
    return ir_expression(dt_enum(self)->members->data);
}

static bool
dt_enum_is_impl_conv(DtDataType *self, IrExpression *expression)
{
    assert(dt_is_enum(self));
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
