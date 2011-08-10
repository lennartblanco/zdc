#ifndef AST_FOREACH_RANGE_INC_X
#define AST_FOREACH_RANGE_INC_X

#include "ast_statment.h"
#include "ast_variable_declaration.h"
#include "ast_expression.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_FOREACH_RANGE ast_foreach_range_get_type()

#define AST_FOREACH_RANGE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_FOREACH_RANGE, AstForeachRange))

#define AST_FOREACH_RANGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_FOREACH_RANGE, AstForeachRangeClass))

#define AST_IS_FOREACH_RANGE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_FOREACH_RANGE))

#define AST_IS_FOREACH_RANGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_FOREACH_RANGE))

#define AST_FOREACH_RANGE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_FOREACH_RANGE, AstForeachRangeClass))

typedef struct
{
    AstStatment             parent;

    /* private */
    AstVariableDeclaration *index;
    AstExpression          *lower_exp;
    AstExpression          *upper_exp;
    AstCodeBlock           *body;
} AstForeachRange;

typedef struct
{
    AstStatmentClass parent_class;
} AstForeachRangeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_foreach_range_get_type(void);

AstForeachRange *
ast_foreach_range_new(AstVariableDeclaration *index,
                      AstExpression *lower_exp,
                      AstExpression *upper_exp,
                      AstCodeBlock  *body);

AstVariableDeclaration *
ast_foreach_range_get_index(AstForeachRange *self);

AstExpression *
ast_foreach_range_get_lower_exp(AstForeachRange *self);

AstExpression *
ast_foreach_range_get_upper_exp(AstForeachRange *self);

AstCodeBlock *
ast_foreach_range_get_body(AstForeachRange *self);

#endif /* AST_FOREACH_RANGE_INC_X */
