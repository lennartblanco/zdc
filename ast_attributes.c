#include "ast_attributes.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_attributes_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstAttributesClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstAttributes),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_DECLARATION,
                                    "AstAttributesType",
                                    &info, 0);
    }
    return type;
}

AstAttributes *
ast_attributes_new(AstAttribute *first_attr)
{
    assert(AST_IS_ATTRIBUTE(first_attr));

    AstAttributes *obj;

    obj = g_object_new(AST_ATTRIBUTES_TYPE, NULL);

    obj->attrs = g_slist_prepend(NULL, first_attr);

    return obj;
}

void
ast_attributes_add_attribute(AstAttributes *self, AstAttribute *attribute)
{
    assert(AST_IS_ATTRIBUTES(self));
    assert(AST_IS_ATTRIBUTE(attribute));

    self->attrs = g_slist_append(self->attrs, attribute);
}

GSList *
ast_attributes_get_attributes(AstAttributes *self)
{
    assert(AST_IS_ATTRIBUTES(self));

    return self->attrs;
}
