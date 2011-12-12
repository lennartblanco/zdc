#include <string.h>

#include "ir_scope.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static char *
package_names_prefix(GSList *names);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_scope_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrScopeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrScope),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(G_TYPE_OBJECT,
                                    "IrScopeType",
                                    &info, 0);
    }
    return type;
}

IrScope *
ir_scope_new_root(GSList *names)
{
    IrScope *obj;

    obj = g_object_new(IR_TYPE_SCOPE, NULL);

    obj->mangle_prefix = package_names_prefix(names);

    return obj;
}

IrScope *
ir_scope_new_sub(const char *name, IrScope *parent)
{
    assert(name);
    assert(IR_IS_SCOPE(parent));

    IrScope *obj;

    obj = g_object_new(IR_TYPE_SCOPE, NULL);

    obj->mangle_prefix =
            g_strdup_printf("%s%zu%s",
                            ir_scope_get_mangle_prefix(parent),
                            strlen(name),
                            name);

    return obj;
}

char *
ir_scope_get_mangle_prefix(IrScope *self)
{
    assert(IR_IS_SCOPE(self));

    return self->mangle_prefix;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static char *
package_names_prefix(GSList *names)
{
    GSList *i;
    GString *str = g_string_new(NULL);

    for (i = names; i != NULL; i = g_slist_next(i))
    {
       g_string_append_printf(str, "%zu%s", strlen(i->data), (char *)i->data);
    }

    return g_string_free(str, FALSE);
}
