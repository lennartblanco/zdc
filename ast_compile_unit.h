#ifndef AST_COMPILE_UNIT_INC_X
#define AST_COMPILE_UNIT_INC_X

#include "ast_function_decl.h"
#include "ast_function_def.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_COMPILE_UNIT ast_compile_unit_get_type()

#define AST_COMPILE_UNIT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_COMPILE_UNIT, AstCompileUnit))

#define AST_COMPILE_UNIT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_COMPILE_UNIT, AstCompileUnitClass))

#define AST_IS_COMPILE_UNIT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_COMPILE_UNIT))

#define AST_IS_COMPILE_UNIT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_COMPILE_UNIT))

#define AST_COMPILE_UNIT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_COMPILE_UNIT, AstCompileUnitClass))

typedef struct 
{
  AstNode parent;
  /* private */
  GSList *function_defs;
  GSList *function_decls;
} AstCompileUnit;

typedef struct 
{
  AstNodeClass parent_class;
} AstCompileUnitClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType ast_compile_unit_get_type(void);

AstCompileUnit* ast_compile_unit_new(void);

void
ast_compile_unit_add_function_decl(AstCompileUnit *self,
                                   AstFunctionDecl *function_decl);

void
ast_compile_unit_add_function_def(AstCompileUnit *self,
                                  AstFunctionDef *function_def);

GSList *
ast_compile_unit_get_function_decls(AstCompileUnit *self);

GSList *
ast_compile_unit_get_function_defs(AstCompileUnit *self);

#endif /* AST_COMPILE_UNIT_INC_X */
