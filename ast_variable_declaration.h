#ifndef AST_VARIABLE_DECLARATION_INC_X
#define AST_VARIABLE_DECLARATION_INC_X

#include "ast_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_VARIABLE_DECLARATION ast_variable_declaration_get_type()

#define XDP_AST_VARIABLE_DECLARATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_VARIABLE_DECLARATION, AstVariableDeclaration))

#define XDP_AST_VARIABLE_DECLARATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_VARIABLE_DECLARATION, AstVariableDeclarationClass))

#define XDP_IS_AST_VARIABLE_DECLARATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_VARIABLE_DECLARATION))

#define XDP_IS_AST_VARIABLE_DECLARATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_VARIABLE_DECLARATION))

#define XDP_AST_VARIABLE_DECLARATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_VARIABLE_DECLARATION, AstVariableDeclarationClass))

typedef struct
{
    AstNode      parent;

    /* private */
    AstDataType  *type;
    char         *name;
} AstVariableDeclaration;

typedef struct
{
  AstNodeClass parent_class;
} AstVariableDeclarationClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_variable_declaration_get_type(void);

AstVariableDeclaration *
ast_variable_declaration_new(AstDataType *type, char *name);

AstDataType *
ast_variable_declaration_get_data_type(AstVariableDeclaration *self);

char *
ast_variable_declaration_get_name(AstVariableDeclaration *self);

#endif /* AST_VARIABLE_DECLARATION_INC_X */
