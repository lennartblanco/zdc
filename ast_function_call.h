#ifndef AST_FUNCTION_CALL_INC_X
#define AST_FUNCTION_CALL_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_FUNCTION_CALL ast_function_call_get_type()

#define XDP_AST_FUNCTION_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_FUNCTION_CALL, AstFunctionCall))

#define XDP_AST_FUNCTION_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_FUNCTION_CALL, AstFunctionCallClass))

#define XDP_IS_AST_FUNCTION_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_FUNCTION_CALL))

#define XDP_IS_AST_FUNCTION_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_FUNCTION_CALL))

#define XDP_AST_FUNCTION_CALL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_FUNCTION_CALL, AstFunctionCallClass))

typedef struct
{
    AstExpression         parent;

    /* private */
    char                 *name;
    GSList               *arguments;
} AstFunctionCall;

typedef struct
{
    AstExpressionClass parent_class;
} AstFunctionCallClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_call_get_type(void);

AstFunctionCall *
ast_function_call_new(char *name, GSList *arguments);

#endif /* AST_FUNCTION_CALL_INC_X */
