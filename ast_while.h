#ifndef AST_WHILE_INC_X
#define AST_WHILE_INC_X

#include "ast_statment.h"
#include "ast_expression.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_WHILE ast_while_get_type()

#define AST_WHILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_WHILE, AstWhile))

#define XDP_AST_WHILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_WHILE, AstWhileClass))

#define AST_IS_WHILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_WHILE))

#define AST_IS_WHILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_WHILE))

#define AST_WHILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_WHILE, AstForeachClass))

typedef struct
{
    AstStatment             parent;

    /* private */
    AstExpression          *loop_condition;
    AstCodeBlock           *body;

} AstWhile;

typedef struct
{
  AstStatmentClass parent_class;
} AstWhileClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_while_get_type(void);

/**
 * Create new AST while node
 *
 * @param loop_condition the expression to evaluate to deside if 
 *                       the loop should continue
 * @param body           the code to run for each loop iteration
 *
 * @return new object representing while-statment
 */
AstWhile * 
ast_while_new(AstExpression *loop_condition,
              AstCodeBlock *body);

AstExpression *
ast_while_get_loop_condition(AstWhile *self);

AstCodeBlock *
ast_while_get_body(AstWhile *self);

#endif /* AST_WHILE_INC_X */
