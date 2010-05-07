#include "ast_string_literal.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_string_literal_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstStringLiteralClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstStringLiteral),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_CONSTANT,
                                    "AstStringLiteralType",
                                    &info, 0);
    }
    return type;
}

AstStringLiteral *
ast_string_literal_new(gchar *string, guint line_number)
{
    AstStringLiteral *obj;

    obj = g_object_new(AST_TYPE_STRING_LITERAL,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->string = g_strdup(string);

    return obj;
}

gchar *
ast_string_literal_get_str(AstStringLiteral *self)
{
    assert(AST_IS_STRING_LITERAL(self));

    return self->string;
}

