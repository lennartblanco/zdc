#ifndef AST_ARRAY_LITERAL_INC_X
#define AST_ARRAY_LITERAL_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ARRAY_LITERAL ast_array_literal_get_type()

#define AST_ARRAY_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ARRAY_LITERAL, AstArrayLiteral))

#define AST_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ARRAY_LITERAL, AstArrayLiteralClass))

#define AST_IS_ARRAY_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ARRAY_LITERAL))

#define AST_IS_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ARRAY_LITERAL))

#define AST_ARRAY_LITERAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ARRAY_LITERAL, AstArrayLiteralClass))

typedef struct
{
    AstConstant parent;
    /* private */
    GSList *values;
} AstArrayLiteral;

typedef struct
{
    AstConstantClass parent_class;
} AstArrayLiteralClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_array_literal_get_type(void);

AstArrayLiteral *
ast_array_literal_new(guint line_number);

void
ast_array_literal_add_value(AstArrayLiteral *self, AstExpression *value);

GSList *
ast_array_literal_get_values(AstArrayLiteral *self);

#endif /* AST_ARRAY_LITERAL_INC_X */
