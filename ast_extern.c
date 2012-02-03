#include <string.h>

#include "ast_extern.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_extern_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstExternClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstExtern),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_ATTRIBUTE,
                                    "AstExternType",
                                    &info, 0);
    }
    return type;
}

AstExtern *
ast_extern_new(const char *linkage, guint line_number)
{
    AstExtern *obj;

    obj = g_object_new(AST_TYPE_EXTERN,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->linkage = g_strdup(linkage);

    return obj;
}

const char *
ast_extern_get_linkage(AstExtern *self)
{
    assert(AST_IS_EXTERN(self));

    return self->linkage;
}
