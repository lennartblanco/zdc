#ifndef AST_DECLARATION_BLOCK_INC_X
#define AST_DECLARATION_BLOCK_INC_X

#include "ast_attributes.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_DECLARATION_BLOCK ast_declaration_block_get_type()

#define AST_DECLARATION_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_DECLARATION_BLOCK, AstDeclarationBlock))

#define AST_DECLARATION_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_DECLARATION_BLOCK, AstDeclarationBlockClass))

#define AST_IS_DECLARATION_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_DECLARATION_BLOCK))

#define AST_IS_DECLARATION_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_DECLARATION_BLOCK))

#define AST_DECLARATION_BLOCK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_DECLARATION_BLOCK, AstDeclarationBlockClass))

typedef struct
{
    AstDeclaration    parent;

    /* private */
    AstAttributes    *attrs;
    GSList           *decls;
} AstDeclarationBlock;

typedef struct
{
    AstDeclarationClass parent_class;
} AstDeclarationBlockClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_declaration_block_get_type(void);

AstDeclarationBlock *
ast_declaration_block_new(AstAttributes *attrs);

void
ast_declaration_block_add_declaration(AstDeclarationBlock *self,
                                      AstDeclaration *decl);

AstAttributes *
ast_declaration_block_get_attributes(AstDeclarationBlock *self);

GSList *
ast_declaration_block_get_declarations(AstDeclarationBlock *self);

#endif /* AST_DECLARATION_BLOCK_INC_X */
