#ifndef AST_FUNCTION_DECL_INC_X
#define AST_FUNCTION_DECL_INC_X

/**
 * Function declaration AST node. A function declaration is specification
 * of function signature and linkage attribute.
 */

#include "dt_data_type.h"
#include "ast_code_block.h"

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
    AstNode      parent;

    /* private */
    char         *linkage;      /* linkage attribute */
    char         *name;
    GSList       *parameters;
    DtDataType   *return_type;
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
                      DtDataType *return_type,
                      guint line_number);

/**
 * Set function's linkage type attribute
 */
void
ast_function_decl_set_linkage(AstFunctionDecl *self,
                              char *linkage);
/**
 * Get function's linkage type attribute
 */
char *
ast_function_decl_get_linkage(AstFunctionDecl *self);

char *
ast_function_decl_get_name(AstFunctionDecl *self);

GSList *
ast_function_decl_get_parameters(AstFunctionDecl *self);

DtDataType *
ast_function_decl_get_return_type(AstFunctionDecl *self);

#endif /* #define AST_FUNCTION_DECL_INC_X */
