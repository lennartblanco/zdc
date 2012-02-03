#ifndef AST_MODULE_INC_X
#define AST_MODULE_INC_X

#include "ast_types.h"
#include "ast_function_decl.h"
#include "ast_function_def.h"
#include "ast_user_type.h"
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

const char *
ast_module_get_source_file(AstModule *self);

void
ast_module_set_package(AstModule *self, GSList *package_names);

GSList *
ast_module_get_package(AstModule *self);

void
ast_module_add_imports(AstModule *self, GSList *imports);

GSList *
ast_module_get_imports(AstModule *self);

void
ast_module_add_declaration(AstModule *self, AstDeclaration *decl);

GSList *
ast_module_get_declarations(AstModule *self);

#endif /* AST_MODULE_INC_X */
