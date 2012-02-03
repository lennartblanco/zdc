#ifndef AST_USER_TYPE_INC_X
#define AST_USER_TYPE_INC_X

#include "ast_declaration.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_USER_TYPE ast_user_type_get_type()

#define AST_USER_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_USER_TYPE, AstUserType))

#define AST_USER_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_USER_TYPE, AstUserTypeClass))

#define AST_IS_USER_TYPE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_USER_TYPE))

#define AST_IS_USER_TYPE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_USER_TYPE))

#define AST_USER_TYPE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_USER_TYPE, AstUserTypeClass))

typedef struct
{
    AstDeclaration    parent;
} AstUserType;

typedef struct
{
    AstDeclarationClass parent_class;
} AstUserTypeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_user_type_get_type(void);

#endif /* AST_USER_TYPEINC_X */
