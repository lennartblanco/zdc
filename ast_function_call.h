#ifndef AST_FUNCTION_CALL_INC_X
#define AST_FUNCTION_CALL_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_FUNCTION_CALL ast_function_call_get_type()

#define AST_FUNCTION_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_FUNCTION_CALL, AstFunctionCall))

#define AST_FUNCTION_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_FUNCTION_CALL, AstFunctionCallClass))

#define AST_IS_FUNCTION_CALL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_FUNCTION_CALL))

#define AST_IS_FUNCTION_CALL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_FUNCTION_CALL))

#define AST_FUNCTION_CALL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_FUNCTION_CALL, AstFunctionCallClass))

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
ast_function_call_new(char *name,
                      GSList *arguments,
                      guint line_number);

char *
ast_function_call_get_name(AstFunctionCall *self);

GSList *
ast_function_call_get_arguments(AstFunctionCall *self);

#endif /* AST_FUNCTION_CALL_INC_X */
