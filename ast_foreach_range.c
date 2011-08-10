#include "ast_foreach_range.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_foreach_range_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstForeachRangeClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstForeachRange),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_STATMENT,
                                    "AstForeachRangeType",
                                    &info, 0);
    }
    return type;
}

AstForeachRange *
ast_foreach_range_new(AstVariableDeclaration *index,
                      AstExpression *lower_exp,
                      AstExpression *upper_exp,
                      AstCodeBlock  *body)
{
    assert(AST_IS_VARIABLE_DECLARATION(index));
    assert(AST_IS_EXPRESSION(lower_exp));
    assert(AST_IS_EXPRESSION(upper_exp));
    assert(AST_IS_CODE_BLOCK(body));


    AstForeachRange *obj;

    obj = g_object_new(AST_TYPE_FOREACH_RANGE, NULL);
    obj->index = index;
    obj->lower_exp = lower_exp;
    obj->upper_exp = upper_exp;
    obj->body = body;

    return obj;
}

AstVariableDeclaration *
ast_foreach_range_get_index(AstForeachRange *self)
{
    assert(AST_IS_FOREACH_RANGE(self));

    return self->index;
}

AstExpression *
ast_foreach_range_get_lower_exp(AstForeachRange *self)
{
    assert(AST_IS_FOREACH_RANGE(self));

    return self->lower_exp;
}

AstExpression *
ast_foreach_range_get_upper_exp(AstForeachRange *self)
{
    assert(AST_IS_FOREACH_RANGE(self));

    return self->upper_exp;
}

AstCodeBlock *
ast_foreach_range_get_body(AstForeachRange *self)
{
    assert(AST_IS_FOREACH_RANGE(self));

    return self->body;
}
