#ifndef AST_FUNCTION_DECL_INC_X
#define AST_FUNCTION_DECL_INC_X

/**
 * Function declaration AST node. A function declaration is specification
 * of function signature and linkage attribute.
 */

#include "ast_node.h"
#include "ast_data_type.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_FUNCTION_DECL ast_function_decl_get_type()

#define XDP_AST_FUNCTION_DECL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_FUNCTION_DECL, AstFunctionDef))

#define XDP_AST_FUNCTION_DECL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_FUNCTION_DECL, AstFunctionDefClass))

#define XDP_IS_AST_FUNCTION_DECL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_FUNCTION_DECL))

#define XDP_IS_AST_FUNCTION_DECL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_FUNCTION_DECL))

#define XDP_AST_FUNCTION_DECL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_FUNCTION_DECL, AstFunctionDefClass))

typedef struct
{
    AstNode      parent;

    /* private */
    char         *linkage;      /* linkage attribute */
    char         *name;
    GSList       *parameters;
    AstDataType  *return_type;
} AstFunctionDecl;

typedef struct
{
  AstNodeClass parent_class;
} AstFunctionDeclClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_decl_get_type(void);

AstFunctionDecl *
ast_function_decl_new(char *name,
                      GSList *parameters,
                      AstDataType *return_type);

void
ast_function_decl_set_linkage(AstFunctionDecl *self,
                              char *linkage);

char *
ast_function_decl_get_name(AstFunctionDecl *self);

GSList *
ast_function_decl_get_parameters(AstFunctionDecl *self);

AstDataType *
ast_function_decl_get_return_type(AstFunctionDecl *self);

#endif /* #define AST_FUNCTION_DECL_INC_X */
