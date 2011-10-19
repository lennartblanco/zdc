#include "ast_address_of.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_address_of_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstAddressOfClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstAddressOf),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstAddressOfType",
                                    &info, 0);
    }
    return type;
}

AstAddressOf *
ast_address_of_new(AstExpression *expression, guint line_number)
{
    AstAddressOf *obj;

    obj = g_object_new(AST_TYPE_ADDRESS_OF,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->expression = expression;

    return obj;
}

AstExpression *
ast_address_of_get_expression(AstAddressOf *self)
{
    assert(AST_IS_ADDRESS_OF(self));

    return self->expression;
}
