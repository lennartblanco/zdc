#include "ast_class.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_class_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstClassClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstClass),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_USER_TYPE,
                                    "AstClassType",
                                    &info, 0);
    }
    return type;
}

AstClass *
ast_class_new(gchar *name, guint line_number)
{
    AstClass *obj;

    obj = g_object_new(AST_TYPE_CLASS,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->name = g_strdup(name);

    return obj;
}

gchar *
ast_class_get_name(AstClass *self)
{
    assert(AST_IS_CLASS(self));

    return self->name;
}
