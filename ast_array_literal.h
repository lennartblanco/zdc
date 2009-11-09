#ifndef AST_ARRAY_LITERAL_INC_X
#define AST_ARRAY_LITERAL_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_ARRAY_LITERAL ast_array_literal_get_type()

#define XDP_AST_ARRAY_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_ARRAY_LITERAL, AstArrayLiteral))

#define XDP_AST_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_ARRAY_LITERAL, AstArrayLiteralClass))

#define XDP_IS_AST_ARRAY_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_ARRAY_LITERAL))

#define XDP_IS_AST_ARRAY_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_ARRAY_LITERAL))

#define XDP_AST_ARRAY_LITERAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_ARRAY_LITERAL, AstArrayLiteralClass))

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
