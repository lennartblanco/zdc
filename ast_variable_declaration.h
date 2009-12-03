#ifndef AST_VARIABLE_DECLARATION_INC_X
#define AST_VARIABLE_DECLARATION_INC_X

#include "ast_node.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_VARIABLE_DECLARATION ast_variable_declaration_get_type()

#define AST_VARIABLE_DECLARATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_VARIABLE_DECLARATION, AstVariableDeclaration))

#define AST_VARIABLE_DECLARATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_VARIABLE_DECLARATION, AstVariableDeclarationClass))

#define AST_IS_VARIABLE_DECLARATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_VARIABLE_DECLARATION))

#define AST_IS_VARIABLE_DECLARATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_VARIABLE_DECLARATION))

#define AST_VARIABLE_DECLARATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_VARIABLE_DECLARATION, AstVariableDeclarationClass))

typedef struct
{
    AstNode      parent;

    /* private */
    DtDataType  *type;
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
ast_variable_declaration_new(DtDataType *type,
                             char *name,
                             guint line_number);

DtDataType *
ast_variable_declaration_get_data_type(AstVariableDeclaration *self);

char *
ast_variable_declaration_get_name(AstVariableDeclaration *self);

#endif /* AST_VARIABLE_DECLARATION_INC_X */
