#ifndef AST_DECLARATION_INC_X
#define AST_DECLARATION_INC_X

#include "ast_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_DECLARATION ast_declaration_get_type()

#define AST_DECLARATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_DECLARATION, AstDeclaration))

#define AST_DECLARATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_DECLARATION, AstDeclarationClass))

#define AST_IS_DECLARATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_DECLARATION))

#define AST_IS_DECLARATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_DECLARATION))

#define AST_DECLARATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_DECLARATION, AstDeclarationClass))

typedef struct
{
    AstNode parent;
} AstDeclaration;

typedef struct
{
    AstNodeClass parent_class;
} AstDeclarationClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_declaration_get_type(void);

#endif /* AST_DECLARATION_INC_X */
