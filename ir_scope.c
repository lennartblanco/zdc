#include <string.h>

#include "ir_scope.h"

#include <assert.h>

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

    obj->names = g_slist_copy(names);
    obj->mangle_prefix = NULL;
    obj->fq_name = NULL;

    return obj;
}

IrScope *
ir_scope_new_sub(const char *name, IrScope *parent)
{
    return ir_scope_new_root(g_slist_append(g_slist_copy(parent->names),
                             (gpointer)name));
}

char *
ir_scope_get_mangle_prefix(IrScope *self)
{
    assert(IR_IS_SCOPE(self));

    if (self->mangle_prefix == NULL)
    {
        GSList *i;
        GString *str = g_string_new(NULL);

        for (i = self->names; i != NULL; i = g_slist_next(i))
        {
           g_string_append_printf(str, "%zu%s",
                                  strlen(i->data),
                                  (char *)i->data);
        }
        self->mangle_prefix = g_string_free(str, FALSE);
    }

    return self->mangle_prefix;
}

char *
ir_scope_get_fqname(IrScope *self)
{
    assert(IR_IS_SCOPE(self));

    if (self->fq_name == NULL)
    {
        GSList *i;
        GString *str = g_string_new(NULL);

        for (i = self->names; i != NULL; i = g_slist_next(i))
        {
           g_string_append_printf(str, "%s%s",
                                  (char *)i->data,
                                  g_slist_next(i) != NULL ? "." : "");
        }
        self->fq_name = g_string_free(str, FALSE);

    }
    return self->fq_name;
}
