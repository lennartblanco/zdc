#ifndef AST_IF_ELSE_INC_X
#define AST_IF_ELSE_INC_X

#include "ast_statment.h"
#include "ast_if_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_IF_ELSE ast_if_else_get_type()

#define XDP_AST_IF_ELSE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_IF_ELSE, AstIfElse))

#define XDP_AST_IF_ELSE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_IF_ELSE, AstIfElseClass))

#define XDP_IS_AST_IF_ELSE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_IF_ELSE))

#define XDP_IS_AST_IF_ELSE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_IF_ELSE))

#define XDP_AST_IF_ELSE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_FUNCTION, AstIfElseClass))

typedef struct
{
    AstStatment      parent;

    /* private */
    GSList       *if_else_blocks;
    AstCodeBlock *else_block;
} AstIfElse;

typedef struct
{
  AstStatmentClass parent_class;
} AstIfElseClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_if_else_get_type(void);

AstIfElse *
ast_if_else_new();

void
ast_if_else_add_if_else_block(AstIfElse *self, AstIfBlock *if_else_block);

void
ast_if_else_set_else_block(AstIfElse *self, AstCodeBlock *else_block);

GSList *
ast_if_else_get_if_else_blocks(AstIfElse *self);

AstCodeBlock *
ast_if_else_get_else_block(AstIfElse *self);

#endif /* AST_IF_ELSE_INC_X */
