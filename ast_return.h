#ifndef AST_RETURN_INC_X
#define AST_RETURN_INC_X

#include "ast_statment.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_RETURN ast_return_get_type()

#define XDP_AST_RETURN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_RETURN, AstFunction))

#define XDP_AST_RETURN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_RETURN, AstFunctionClass))

#define XDP_IS_AST_RETURN(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_RETURN))

#define XDP_IS_AST_RETURN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_RETURN))

#define XDP_AST_RETURN_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_RETURN, AstFunctionClass))

typedef struct
{
    AstStatment    parent;

    /* private */
    AstExpression  *return_value;
} AstReturn;

typedef struct
{
  AstStatmentClass parent_class;
} AstReturnClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_return_get_type(void);

AstReturn * 
ast_return_new(AstExpression *return_value);

#endif /* AST_RETURN_INC_X */
