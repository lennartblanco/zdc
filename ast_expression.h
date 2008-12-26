#ifndef AST_EXPRESSION_INC_X
#define AST_EXPRESSION_INC_X

#include "ast_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_EXPRESSION ast_expression_get_type()

#define XDP_AST_EXPRESSION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_EXPRESSION, AstExpression))

#define XDP_AST_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_EXPRESSION, AstExpressionClass))

#define XDP_IS_AST_EXPRESSION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_EXPRESSION))

#define XDP_IS_AST_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_EXPRESSION))

#define XDP_AST_EXPRESSION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_EXPRESSION, AstExpressionClass))

typedef struct
{
    AstNode parent;
} AstExpression;

typedef struct
{
    AstNodeClass parent_class;
} AstExpressionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_expression_get_type(void);

#endif /* AST_EXPRESSION_INC_X */
