#include "ast_property.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_property_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (AstPropertyClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstProperty),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstPropertyType",
                                    &info, 0);
    }
    return type;
}

AstProperty *
ast_property_new(AstExpression *exp,
                 const char *name)
{
    assert(AST_IS_EXPRESSION(exp));
    assert(name);

    AstProperty *obj;

    obj = g_object_new(AST_TYPE_PROPERTY, NULL);
    obj->exp = exp;
    obj->name = g_strdup(name);

    return obj;
}

AstExpression *
ast_property_get_expression(AstProperty *self)
{
    assert(AST_IS_PROPERTY(self));

    return self->exp;
}

const char *
ast_property_get_name(AstProperty *self)
{
    assert(AST_IS_PROPERTY(self));

    return self->name;
}
