#ifndef AST_FOREACH_INC_X
#define AST_FOREACH_INC_X

#include "ast_statment.h"
#include "ast_variable_declaration.h"
#include "ast_array_slice_ref.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_FOREACH ast_foreach_get_type()

#define XDP_AST_FOREACH(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_FOREACH, AstForeach))

#define XDP_AST_FOREACH_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_FOREACH, AstForeachClass))

#define XDP_IS_AST_FOREACH(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_FOREACH))

#define XDP_IS_AST_FOREACH_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_FOREACH))

#define XDP_AST_FOREACH_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_FOREACH, AstForeachClass))

typedef struct
{
    AstStatment    parent;

    /* private */
    AstVariableDeclaration *index;
    AstVariableDeclaration *value;
    AstArraySliceRef       *aggregate;
    AstCodeBlock           *body;

} AstForeach;

typedef struct
{
  AstStatmentClass parent_class;
} AstForeachClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_foreach_get_type(void);

/**
 * Create new AST Foreach node
 * 
 * @param index index variable node, or null if index is variable is not 
 *              declared
 * @param value value variable node
 * @param aggregate the aggregate expression to iterate over
 * @param body the loop's body node
 */
AstForeach * 
ast_foreach_new(AstVariableDeclaration *index,
                AstVariableDeclaration *value,
                AstArraySliceRef *aggregate,
                AstCodeBlock *body);

#endif /* AST_FOREACH_INC_X */
