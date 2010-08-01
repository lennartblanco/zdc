#ifndef AST_RETURN_INC_X
#define AST_RETURN_INC_X

#include "ast_statment.h"
#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_RETURN ast_return_get_type()

#define AST_RETURN(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_RETURN, AstReturn))

#define AST_RETURN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_RETURN, AstReturnClass))

#define AST_IS_RETURN(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_RETURN))

#define AST_IS_RETURN_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_RETURN))

#define AST_RETURN_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_RETURN, AstReturnClass))

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
ast_return_new(AstExpression *return_value, guint line_number);

AstExpression *
ast_return_get_return_value(AstReturn *self);

#endif /* AST_RETURN_INC_X */
