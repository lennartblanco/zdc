#ifndef AST_FOR_INC_X
#define AST_FOR_INC_X

#include "ast_statment.h"
#include "ast_code_block.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_FOR ast_for_get_type()

#define AST_FOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_FOR, AstFor))

#define AST_FOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_FOR, AstForClass))

#define AST_IS_FOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_FOR))

#define AST_IS_FOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_FOR))

#define AST_FOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_FOR, AstForClass))

typedef struct
{
    AstStatment             parent;

    /* private */
    AstCodeBlock           *init;
    AstExpression          *test;
    AstExpression          *step;
    AstCodeBlock           *body;
} AstFor;

typedef struct
{
    AstStatmentClass parent_class;
} AstForClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_for_get_type(void);

AstFor *
ast_for_new(AstCodeBlock *init,
            AstExpression *test,
            AstExpression *step,
            AstCodeBlock *body);

AstCodeBlock *
ast_for_get_init(AstFor *self);

AstExpression *
ast_for_get_test(AstFor *self);

AstExpression *
ast_for_get_step(AstFor *self);

AstCodeBlock *
ast_for_get_body(AstFor *self);

#endif /* AST_FOR_INC_X */
