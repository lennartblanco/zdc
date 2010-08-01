#ifndef AST_IMPORT_INC_X
#define AST_IMPORT_INC_X

#include "ast_types.h"
#include "ast_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_IMPORT ast_import_get_type()

#define AST_IMPORT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_IMPORT, AstImport))

#define AST_IMPORT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_IMPORT, AstImportClass))

#define AST_IS_IMPORT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_IMPORT))

#define AST_IS_IMPORT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_IMPORT))

#define AST_IMPORT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_IMPORT, AstImportClass))

typedef struct
{
    AstNodeClass parent_class;
} AstImportClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_import_get_type(void);

/**
 * @param module_name the imported module's fully qualified name
 */
AstImport *
ast_import_new(GSList *module_name, guint line_number);

/**
 * Get the relative path to imported modules source file.
 * E.g. 'import root.sub.mdl;' will return 'root/sub/mdl.d'.
 *
 * @return the relative path as a newly allocated string. The string
 *         should be release with gfree() when no longer needed.
 */
char *
ast_import_get_path(AstImport *self);

void
ast_import_set_module(AstImport *self, AstModule *parsed_module);

/**
 * Get the imported module's AST.
 */
AstModule *
ast_import_get_module(AstImport *self);

#endif /* AST_IMPORT_INC_X */
