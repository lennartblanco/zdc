#ifndef AST_MODULE_INC_X
#define AST_MODULE_INC_X

#include "ast_function_decl.h"
#include "ast_function_def.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_MODULE ast_module_get_type()

#define AST_MODULE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_MODULE, AstModule))

#define AST_MODULE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_MODULE, AstModuleClass))

#define AST_IS_MODULE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_MODULE))

#define AST_IS_MODULE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_MODULE))

#define AST_MODULE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_MODULE, AstModuleClass))

typedef struct 
{
  AstNode parent;
  /* private */
  GSList *package;
  GSList *function_defs;
  GSList *function_decls;
} AstModule;

typedef struct 
{
  AstNodeClass parent_class;
} AstModuleClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_module_get_type(void);

AstModule *
ast_module_new(void);

void
ast_module_set_package(AstModule *self, GSList *package_names);

void
ast_module_add_function_decl(AstModule *self,
                             AstFunctionDecl *function_decl);

void
ast_module_add_function_def(AstModule *self,
                            AstFunctionDef *function_def);

GSList *
ast_module_get_function_decls(AstModule *self);

GSList *
ast_module_get_function_defs(AstModule *self);

#endif /* AST_MODULE_INC_X */
