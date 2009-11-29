#ifndef AST_CONSTANT_INC_X
#define AST_CONSTANT_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_CONSTANT ast_constant_get_type()

#define AST_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_CONSTANT, AstConstant))

#define AST_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_CONSTANT, AstConstantClass))

#define AST_IS_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_CONSTANT))

#define AST_IS_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_CONSTANT))

#define AST_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_CONSTANT, AstConstantClass))

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
