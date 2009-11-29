#ifndef AST_EXPRESSION_INC_X
#define AST_EXPRESSION_INC_X

#include "ast_statment.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_EXPRESSION ast_expression_get_type()

#define AST_EXPRESSION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_EXPRESSION, AstExpression))

#define AST_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_EXPRESSION, AstExpressionClass))

#define AST_IS_EXPRESSION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_EXPRESSION))

#define AST_IS_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_EXPRESSION))

#define AST_EXPRESSION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_EXPRESSION, AstExpressionClass))

typedef struct
{
    AstStatment parent;
} AstExpression;

typedef struct
{
    AstStatmentClass parent_class;
} AstExpressionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_expression_get_type(void);

#endif /* AST_EXPRESSION_INC_X */
