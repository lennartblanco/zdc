#ifndef AST_POSTFIX_EXP_INC_X
#define AST_POSTFIX_EXP_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_POSTFIX_EXP ast_postfix_exp_get_type()

#define AST_POSTFIX_EXP(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_POSTFIX_EXP, AstPostfixExp))

#define AST_POSTFIX_EXP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_POSTFIX_EXP, AstPostfixExpClass))

#define AST_IS_POSTFIX_EXP(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_POSTFIX_EXP))

#define AST_IS_POSTFIX_EXP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_POSTFIX_EXP))

#define AST_POSTFIX_EXP_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_POSTFIX_EXP, AstPostfixExpClass))

typedef struct
{
    AstExpression parent;

    /* private */
    AstExpression *left;
    AstExpression *right;
} AstPostfixExp;

typedef struct
{
    AstExpressionClass parent_class;
} AstPostfixExpClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_postfix_exp_get_type(void);

AstPostfixExp *
ast_postfix_exp_new(AstExpression *left,
                    AstExpression *right,
                    guint line_number);

AstExpression *
ast_postfix_exp_get_left(AstPostfixExp *self);

AstExpression *
ast_postfix_exp_get_right(AstPostfixExp *self);

#endif /* AST_POSTFIX_EXP_INC_X */
