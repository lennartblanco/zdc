#ifndef AST_VARIABLE_REF_INC_X
#define AST_VARIABLE_REF_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_VARIABLE_REF ast_variable_ref_get_type()

#define AST_VARIABLE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_VARIABLE_REF, AstVariableRef))

#define AST_VARIABLE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_VARIABLE_REF, AstVariableRefClass))

#define AST_IS_VARIABLE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_VARIABLE_REF))

#define AST_IS_VARIABLE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_VARIABLE_REF))

#define AST_VARIABLE_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_VARIABLE_REF, AstVariableRefClass))

typedef struct
{
    AstExpression parent;
    /* private */
    char *name;
} AstVariableRef;

typedef struct
{
    AstExpressionClass parent_class;
} AstVariableRefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_variable_ref_get_type(void);

AstVariableRef *
ast_variable_ref_new(char *name, guint line_number);

char *
ast_variable_ref_get_name(AstVariableRef *self);

#endif /* AST_VARIABLE_REF_INC_X */
