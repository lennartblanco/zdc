#ifndef AST_MODULE_INC_X
#define AST_MODULE_INC_X

#include "ast_types.h"
#include "ast_function_decl.h"
#include "ast_function_def.h"
#include "ast_enum.h"
#include "ast_alias.h"
#include "ast_import.h"

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
  AstNodeClass parent_class;
} AstModuleClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_module_get_type(void);

AstModule *
ast_module_new(void);

/**
 * Set the source file name from which this module was parsed.
 */
void
ast_module_set_source_file(AstModule *self, const char *source_file);

void
ast_module_set_package(AstModule *self, GSList *package_names);

GSList *
ast_module_get_package(AstModule *self);

void
ast_module_add_import(AstModule *self, AstImport *imported_module);

GSList *
ast_module_get_imports(AstModule *self);

void
ast_module_add_function_decl(AstModule *self,
                             AstFunctionDecl *function_decl);

GSList *
ast_module_get_function_decls(AstModule *self);

void
ast_module_add_function_def(AstModule *self,
                            AstFunctionDef *function_def);

GSList *
ast_module_get_function_defs(AstModule *self);

void
ast_module_add_enum_def(AstModule *self,
                        AstEnum *enum_def);

GSList *
ast_module_get_enum_defs(AstModule *self);

void
ast_module_add_alias(AstModule *self,
                     AstAlias *alias);

/**
 * Get all alias statments in this module.
 *
 * @return a list of AstAlias objects
 */
GSList *
ast_module_get_aliases(AstModule *self);

#endif /* AST_MODULE_INC_X */
