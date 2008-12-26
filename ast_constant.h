#ifndef AST_CONSTANT_INC_X
#define AST_CONSTANT_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_CONSTANT ast_constant_get_type()

#define XDP_AST_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_CONSTANT, AstConstant))

#define XDP_AST_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_CONSTANT, AstConstantClass))

#define XDP_IS_AST_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_CONSTANT))

#define XDP_IS_AST_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_CONSTANT))

#define XDP_AST_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_CONSTANT, AstConstantClass))

typedef struct
{
    AstExpression parent;
} AstConstant;

typedef struct
{
    AstExpressionClass parent_class;
} AstConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_constant_get_type(void);

#endif /* AST_CONSTANT_INC_X */
