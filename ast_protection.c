#include "ast_protection.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_protection_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstProtectionClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstProtection),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_ATTRIBUTE,
                                    "AstProtectionType",
                                    &info, 0);
    }
    return type;
}

AstProtection *
ast_protection_new(bool is_private, guint line_number)
{
    AstProtection *obj;

    obj = g_object_new(AST_TYPE_PROTECTION,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->is_private = is_private;

    return obj;
}

bool
ast_protection_is_private(AstProtection *self)
{
    assert(AST_IS_PROTECTION(self));

    return self->is_private;
}
