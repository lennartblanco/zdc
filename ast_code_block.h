#ifndef AST_CODE_BLOCK_INC_X
#define AST_CODE_BLOCK_INC_X

#include "ast_statment.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_CODE_BLOCK ast_code_block_get_type()

#define AST_CODE_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_CODE_BLOCK, AstCodeBlock))

#define AST_CODE_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_CODE_BLOCK, AstCodeBlockClass))

#define AST_IS_CODE_BLOCK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_CODE_BLOCK))

#define AST_IS_CODE_BLOCK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_CODE_BLOCK))

#define AST_CODE_BLOCK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_CODE_BLOCK, AstCodeBlockClass))

typedef struct
{
    AstStatment parent;
    /* private */
    GSList *statments;
} AstCodeBlock;

typedef struct
{
    AstStatmentClass parent_class;
} AstCodeBlockClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_code_block_get_type(void);

AstCodeBlock *
ast_code_block_new();

void
ast_code_block_add_statment(AstCodeBlock *self, AstStatment *statment);

GSList *
ast_code_block_get_statments(AstCodeBlock *self);

#endif /* AST_CODE_BLOCK_INC_X */
