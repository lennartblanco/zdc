#ifndef AST_IDENT_INC_X
#define AST_IDENT_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_IDENT ast_ident_get_type()

#define AST_IDENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_IDENT, AstIdent))

#define AST_IDENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_IDENT, AstIdentClass))

#define AST_IS_IDENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_IDENT))

#define AST_IS_IDENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_IDENT))

#define AST_IDENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_IDENT, AstIdentClass))

typedef struct
{
    AstExpression parent;
    /* private */
    char *name;
} AstIdent;

typedef struct
{
    AstExpressionClass parent_class;
} AstIdentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_ident_get_type(void);

AstIdent *
ast_ident_new(char *name, guint line_number);

char *
ast_ident_get_name(AstIdent *self);

#endif /* AST_IDENT_INC_X */
