#include <string.h>

#include "dt_user.h"
#include "ir_module.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

enum
{
    DT_USER_NAME = 1,
    DT_USER_PARENT_MODULE
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
dt_user_type_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
dt_user_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (DtUserClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        dt_user_type_class_init,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (DtUser),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(DT_TYPE_DATA_TYPE,
                                    "DtUserType",
                                    &info, 0);
    }
    return type;
}

gchar *
dt_user_get_mangled_prefix(DtUser *self)
{
    assert(DT_IS_USER(self));
    assert(DT_USER_GET_CLASS(self)->get_mangled_prefix);

    return DT_USER_GET_CLASS(self)->get_mangled_prefix(self);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
dt_user_get_string(DtDataType *self)
{
    assert(DT_IS_USER(self));

    return DT_USER(self)->name;
}

static char *
dt_user_get_mangled(DtDataType *self)
{
    assert(DT_IS_USER(self));

    DtUser *user_type = DT_USER(self);

    assert(IR_IS_MODULE(user_type->parent_module));

    if (user_type->mangled_name == NULL)
    {
        user_type->mangled_name =
            g_strdup_printf(
                    "%s%s%d%s",
                    dt_user_get_mangled_prefix(user_type),
                    ir_module_get_mangled_name(user_type->parent_module),
                    strlen(user_type->name), user_type->name);
    }

    return user_type->mangled_name;
}

static bool
dt_user_is_same(DtDataType *self, DtDataType *type)
{
    assert(DT_IS_USER(self));

    if (!g_type_is_a(G_TYPE_FROM_INSTANCE(self), G_TYPE_FROM_INSTANCE(type)))
    {
        /* must be of same class */
        return false;
    }

    /* todo: we must take into account the parent module here as well,
     * e.g. a struct foo in module A is not same data type as
     *        struct foo in module B
     */

    DtUser *l = DT_USER(self);
    DtUser *r = DT_USER(type);

    return g_strcmp0(l->name, r->name) == 0;
}

static void
dt_user_set_property(GObject *object,
                     guint property_id,
                     const GValue *value,
                     GParamSpec *pspec)
{
    switch (property_id)
    {
        case DT_USER_NAME:
            DT_USER(object)->name = g_value_dup_string(value);
            break;
        case DT_USER_PARENT_MODULE:
            DT_USER(object)->parent_module = g_value_get_object(value);
            break;
        default:
            /* unexpected property id */
            assert(false);
    }
}

static void
dt_user_get_property(GObject *object,
                     guint property_id,
                     GValue *value,
                     GParamSpec *pspec)
{
    /* not implemented */
    assert(false);
}

static void
dt_user_type_class_init(gpointer klass, gpointer dummy)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS(klass);
    GParamSpec *pspec;

    /*
     * setup this structure for setting and getting properties
     */
    gobject_class->set_property = dt_user_set_property;
    gobject_class->get_property = dt_user_get_property;

    /*
     * install 'name' property
     */
    pspec = g_param_spec_string("dt-user-name",
                                "dt user name",
                                "the name of the user type",
                                "no-name-set" /* default value */,
                                G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    DT_USER_NAME,
                                    pspec);

    /*
     * install 'parent module' property
     */
    pspec = g_param_spec_object("dt-user-parent-module",
                                "dt user parent module",
                                "the parent module of the user type",
                                IR_TYPE_MODULE,
                                G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

    g_object_class_install_property(gobject_class,
                                    DT_USER_PARENT_MODULE,
                                    pspec);

    /*
     * install default virtual methods implementations
     */
     ((DtDataTypeClass *)klass)->get_string = dt_user_get_string;
     ((DtDataTypeClass *)klass)->get_mangled = dt_user_get_mangled;
     ((DtDataTypeClass *)klass)->is_same = dt_user_is_same;
}
