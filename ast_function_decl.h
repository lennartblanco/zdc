#ifndef AST_FUNCTION_DECL_INC_X
#define AST_FUNCTION_DECL_INC_X

/**
 * Function declaration AST node. A function declaration is specification
 * of function signature and linkage attribute.
 */

#include "dt_data_type.h"
#include "ast_attributes.h"
#include "ast_declaration.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_FUNCTION_DECL ast_function_decl_get_type()

#define AST_FUNCTION_DECL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_FUNCTION_DECL, AstFunctionDecl))

#define AST_FUNCTION_DECL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_FUNCTION_DECL, AstFunctionDeclClass))

#define AST_IS_FUNCTION_DECL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_FUNCTION_DECL))

#define AST_IS_FUNCTION_DECL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_FUNCTION_DECL))

#define AST_FUNCTION_DECL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_FUNCTION_DECL, AstFunctionDeclClass))

typedef struct
{
    AstDeclaration      parent;

    /* private */
    AstAttributes *attrs;
    char          *name;
    GSList        *parameters;
    DtDataType    *return_type;
} AstFunctionDecl;

typedef struct
{
    AstDeclarationClass parent_class;
} AstFunctionDeclClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_decl_get_type(void);

AstFunctionDecl *
ast_function_decl_new(AstAttributes *attrs,
                      DtDataType *return_type,
                      char *name,
                      GSList *parameters,
                      guint line_number);

AstAttributes *
ast_function_decl_get_attributes(AstFunctionDecl *self);

char *
ast_function_decl_get_name(AstFunctionDecl *self);

GSList *
ast_function_decl_get_parameters(AstFunctionDecl *self);

DtDataType *
ast_function_decl_get_return_type(AstFunctionDecl *self);

#endif /* #define AST_FUNCTION_DECL_INC_X */
