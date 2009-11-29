#ifndef AST_IF_BLOCK_INC_X
#define AST_IF_BLOCK_INC_X

#include "ast_expression.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_IF_BLOCK ast_if_block_get_type()

#define AST_IF_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_IF_BLOCK, AstIfBlock))

#define AST_IF_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_IF_BLOCK, AstIfBlockClass))

#define AST_IS_IF_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_IF_BLOCK))

#define AST_IS_IF_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_IF_BLOCK))

#define AST_IF_BLOCK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_IF_BLOCK, AstIfBlockClass))

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
