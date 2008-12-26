#ifndef AST_FUNCTION_INC_X
#define AST_FUNCTION_INC_X

#include "ast_node.h"
#include "ast_data_type.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_FUNCTION ast_function_get_type()

#define XDP_AST_FUNCTION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_FUNCTION, AstFunction))

#define XDP_AST_FUNCTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_FUNCTION, AstFunctionClass))

#define XDP_IS_AST_FUNCTION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_FUNCTION))

#define XDP_IS_AST_FUNCTION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_FUNCTION))

#define XDP_AST_FUNCTION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_FUNCTION, AstFunctionClass))

typedef struct
{
    AstNode      parent;

    /* private */
    char         *name;
    GSList       *parameters;
    AstDataType  *return_type;
    AstCodeBlock *body;
} AstFunction;

typedef struct
{
  AstNodeClass parent_class;
} AstFunctionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_get_type(void);

AstFunction *
ast_function_new(char *name,
                 GSList *parameters,
                 AstDataType *return_type,
                 AstCodeBlock *body);

#endif /* AST_FUNCTION_INC_X */
