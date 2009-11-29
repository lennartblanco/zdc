#ifndef AST_FUNCTION_DEF_INC_X
#define AST_FUNCTION_DEF_INC_X

/**
 * Function definition AST subtree. A function definition is specification
 * of function signature plus the full AST for function body's code.
 */

#include "dt_data_type.h"
#include "ast_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_FUNCTION_DEF ast_function_def_get_type()

#define AST_FUNCTION_DEF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_FUNCTION_DEF, AstFunctionDef))

#define AST_FUNCTION_DEF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_FUNCTION_DEF, AstFunctionDefClass))

#define AST_IS_FUNCTION_DEF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_FUNCTION_DEF))

#define AST_IS_FUNCTION_DEF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_FUNCTION_DEF))

#define AST_FUNCTION_DEF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_FUNCTION_DEF, AstFunctionDefClass))

typedef struct
{
    AstNode      parent;

    /* private */
    char         *name;
    GSList       *parameters;
    DtDataType   *return_type;
    AstCodeBlock *body;
} AstFunctionDef;

typedef struct
{
  AstNodeClass parent_class;
} AstFunctionDefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_function_def_get_type(void);

AstFunctionDef *
ast_function_def_new(char *name,
                     GSList *parameters,
                     DtDataType *return_type,
                     AstCodeBlock *body,
                     guint line_number);

char *
ast_function_def_get_name(AstFunctionDef *self);

GSList *
ast_function_def_get_parameters(AstFunctionDef *self);

DtDataType *
ast_function_def_get_return_type(AstFunctionDef *self);

AstCodeBlock *
ast_function_def_get_body(AstFunctionDef *self);

#endif /* AST_FUNCTION_DEF_INC_X */
