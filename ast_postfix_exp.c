#include "ast_postfix_exp.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_postfix_exp_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info =
      {
        sizeof (AstPostfixExpClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (AstPostfixExp),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(AST_TYPE_EXPRESSION,
                                    "AstPostfixExpType",
                                    &info, 0);
    }
    return type;
}

AstPostfixExp *
ast_postfix_exp_new(AstExpression *left,
                    AstExpression *right,
                    guint line_number)
{
    assert(AST_IS_EXPRESSION(left));
    assert(AST_IS_EXPRESSION(right));

    AstPostfixExp *obj;

    obj = g_object_new(AST_TYPE_POSTFIX_EXP,
                       "ast-node-line-number", line_number,
                       NULL);

    obj->left = left;
    obj->right = right;

    return obj;
}

AstExpression *
ast_postfix_exp_get_left(AstPostfixExp *self)
{
    assert(AST_IS_POSTFIX_EXP(self));

    return self->left;
}

AstExpression *
ast_postfix_exp_get_right(AstPostfixExp *self)
{
    assert(AST_IS_POSTFIX_EXP(self));

    return self->right;
}
