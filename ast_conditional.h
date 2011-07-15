#ifndef AST_CONDITIONAL_INC_X
#define AST_CONDITIONAL_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_CONDITIONAL ast_conditional_get_type()

#define AST_CONDITIONAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_CONDITIONAL, AstConditional))

#define AST_CONDITIONAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_CONDITIONAL, AstConditionalClass))

#define AST_IS_CONDITIONAL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_CONDITIONAL))

#define AST_IS_CONDITIONAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_CONDITIONAL))

#define AST_CONDITIONAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_CONDITIONAL, AstConditionalClass))


typedef struct
{
    AstExpression parent;

    /* private */
    AstExpression *condition;
    AstExpression *true_exp;
    AstExpression *false_exp;
} AstConditional;

typedef struct
{
    AstExpressionClass parent_class;
} AstConditionalClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_conditional_get_type(void);

AstConditional *
ast_conditional_new(AstExpression *condition,
                    AstExpression *true_exp,
                    AstExpression *false_exp,
                    guint line_number);

AstExpression *
ast_conditional_get_cond(AstConditional *self);

AstExpression *
ast_conditional_get_true(AstConditional *self);

AstExpression *
ast_conditional_get_false(AstConditional *self);

#endif /* AST_CONDITIONAL_INC_X */
