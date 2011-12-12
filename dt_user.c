#include <string.h>

#include "dt_user.h"
#include "ir_module.h"

#include <assert.h>

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

    return ir_symbol_get_name(ir_symbol(self));
}

static char *
dt_user_get_mangled(DtDataType *self)
{
    assert(DT_IS_USER(self));

    DtUser *user_type = DT_USER(self);

    if (user_type->mangled_name == NULL)
    {
        const char *name = dt_user_get_string(self);
        IrScope *scope = ir_symbol_get_scope(ir_symbol(self));

        user_type->mangled_name =
            g_strdup_printf(
                    "%s%s%zu%s",
                    dt_user_get_mangled_prefix(user_type),
                    ir_scope_get_mangle_prefix(scope),
                    strlen(name), name);
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

    return g_strcmp0(dt_user_get_string(self), dt_user_get_string(type)) == 0;
}

static void
dt_user_type_class_init(gpointer klass, gpointer dummy)
{
    /*
     * install default virtual methods implementations
     */
    DT_DATA_TYPE_CLASS(klass)->get_string = dt_user_get_string;
    DT_DATA_TYPE_CLASS(klass)->get_mangled = dt_user_get_mangled;
    DT_DATA_TYPE_CLASS(klass)->is_same = dt_user_is_same;
}
