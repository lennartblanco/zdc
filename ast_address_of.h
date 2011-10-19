#ifndef AST_ADDRESS_OF_INC_X
#define AST_ADDRESS_OF_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ADDRESS_OF ast_address_of_get_type()

#define AST_ADDRESS_OF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ADDRESS_OF, AstAddressOf))

#define AST_ADDRESS_OF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ADDRESS_OF, AstAddressOfClass))

#define AST_IS_ADDRESS_OF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ADDRESS_OF))

#define AST_IS_ADDRESS_OF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ADDRESS_OF))

#define AST_ADDRESS_OF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ADDRESS_OF, AstAddressOfClass))

typedef struct
{
    AstExpression parent;
    /* private */
    AstExpression *expression;
} AstAddressOf;

typedef struct
{
    AstExpressionClass parent_class;
} AstAddressOfClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_address_of_get_type(void);

AstAddressOf *
ast_address_of_new(AstExpression *expression, guint line_number);

AstExpression *
ast_address_of_get_expression(AstAddressOf *self);

#endif /* AST_ADDRESS_OF_INC_X */
