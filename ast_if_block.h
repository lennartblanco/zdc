#ifndef AST_IF_BLOCK_INC_X
#define AST_IF_BLOCK_INC_X

#include "ast_expression.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_IF_BLOCK ast_if_block_get_type()

#define XDP_AST_IF_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_IF_BLOCK, AstIfBlock))

#define XDP_AST_IF_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_IF_BLOCK, AstIfBlockClass))

#define XDP_IS_AST_IF_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_IF_BLOCK))

#define XDP_IS_AST_IF_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_IF_BLOCK))

#define XDP_AST_IF_BLOCK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_IF_BLOCK, AstIfBlockClass))

typedef struct
{
    AstNode      parent;

    /* private */
    AstExpression *condition;
    AstCodeBlock  *body;
} AstIfBlock;

typedef struct
{
  AstNodeClass parent_class;
} AstIfBlockClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_if_block_get_type(void);

AstIfBlock *
ast_if_block_new(AstExpression *condition, AstCodeBlock *body);

AstExpression *
ast_if_block_get_condition(AstIfBlock *self);

AstCodeBlock *
ast_if_block_get_body(AstIfBlock *self);

#endif /* AST_IF_BLOCK_INC_X */
