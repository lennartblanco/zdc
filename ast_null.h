#ifndef AST_NULL_INC_X
#define AST_NULL_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_NULL ast_null_get_type()

#define AST_NULL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_NULL, AstNull))

#define AST_NULL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_NULL, AstNullClass))

#define AST_IS_NULL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_NULL))

#define AST_IS_NULL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_NULL))

#define AST_NULL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_NULL, AstNullClass))

typedef struct
{
    AstExpression    parent;
} AstNull;

typedef struct
{
    AstExpressionClass parent_class;
} AstNullClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_null_get_type(void);

AstNull *
ast_null_new(guint line_number);

#endif /* AST_NULL_INC_X */
