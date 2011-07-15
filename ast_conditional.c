#include "ast_conditional.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_conditional_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (AstConditionalClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstConditional),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstConditionalType",
                                    &info, 0);
    }
    return type;
}

AstConditional *
ast_conditional_new(AstExpression *condition,
                    AstExpression *true_exp,
                    AstExpression *false_exp,
                    guint line_number)
{
    assert(AST_IS_EXPRESSION(condition));
    assert(AST_IS_EXPRESSION(true_exp));
    assert(AST_IS_EXPRESSION(false_exp));

    AstConditional *obj;

    obj = g_object_new(AST_TYPE_CONDITIONAL,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->condition = condition;
    obj->true_exp = true_exp;
    obj->false_exp = false_exp;

    return obj;
}

AstExpression *
ast_conditional_get_cond(AstConditional *self)
{
    assert(AST_IS_CONDITIONAL(self));

    return self->condition;
}

AstExpression *
ast_conditional_get_true(AstConditional *self)
{
    assert(AST_IS_CONDITIONAL(self));

    return self->true_exp;
}

AstExpression *
ast_conditional_get_false(AstConditional *self)
{
    assert(AST_IS_CONDITIONAL(self));

    return self->false_exp;
}
