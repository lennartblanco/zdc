#include "ast_postfix_exp.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ast_postfix_exp_do_print(AstNode *self, FILE *out, int indention);

static void
ast_postfix_exp_class_init(gpointer klass, gpointer dummy);

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
        ast_postfix_exp_class_init,   /* class_init */
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
ast_postfix_exp_new(AstExpression *exp,
                    const char *name,
                    guint line_number)
{
    assert(AST_IS_EXPRESSION(exp));
    assert(name);

    AstPostfixExp *obj;

    obj = g_object_new(AST_TYPE_POSTFIX_EXP,
                       "ast-node-line-number", line_number,
                       NULL);
    obj->exp = exp;
    obj->name = g_strdup(name);

    return obj;
}

AstExpression *
ast_postfix_exp_get_expression(AstPostfixExp *self)
{
    assert(AST_IS_POSTFIX_EXP(self));

    return self->exp;
}

const char *
ast_postfix_exp_get_name(AstPostfixExp *self)
{
    assert(AST_IS_POSTFIX_EXP(self));

    return self->name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ast_postfix_exp_do_print(AstNode *self, FILE *out, int indention)
{
    assert(AST_IS_POSTFIX_EXP(self));

    AstPostfixExp *postfix = AST_POSTFIX_EXP(self);

    fprintf(out,
            "postfix expression (");
    ast_node_print(AST_NODE(postfix->exp), out, indention);
    fprintf(out, " . %s)", postfix->name);
}

static void
ast_postfix_exp_class_init(gpointer klass, gpointer dummy)
{
    ((AstNodeClass *)klass)->do_print = ast_postfix_exp_do_print;
}

