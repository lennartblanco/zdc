#ifndef AST_STRING_LITERAL_INC_X
#define AST_STRING_LITERAL_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_STRING_LITERAL ast_string_literal_get_type()

#define AST_STRING_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_STRING_LITERAL, AstStringLiteral))

#define AST_STRING_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_STRING_LITERAL, AstStringLiteralClass))

#define AST_IS_STRING_LITERAL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_STRING_LITERAL))

#define AST_IS_STRING_LITERAL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_STRING_LITERAL))

#define AST_STRING_LITERAL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_STRING_LITERAL, AstStringLiteralClass))

typedef struct
{
    AstConstant parent;
    /* private */
    gchar *string;
} AstStringLiteral;

typedef struct
{
    AstConstantClass parent_class;
} AstStringLiteralClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_string_literal_get_type(void);

AstStringLiteral *
ast_string_literal_new(gchar *string, guint line_number);

gchar *
ast_string_literal_get_str(AstStringLiteral *self);

#endif /* AST_STRING_LITERAL_INC_X */
