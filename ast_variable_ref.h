#ifndef AST_VARIABLE_REF_INC_X
#define AST_VARIABLE_REF_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_VARIABLE_REF ast_variable_ref_get_type()

#define XDP_AST_VARIABLE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_VARIABLE_REF, AstVariableRef))

#define XDP_AST_VARIABLE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_VARIABLE_REF, AstVariableRefClass))

#define XDP_IS_AST_VARIABLE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_VARIABLE_REF))

#define XDP_IS_AST_VARIABLE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_VARIABLE_REF))

#define XDP_AST_VARIABLE_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_VARIABLE_REF, AstVariableRefClass))

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

char *
ast_variable_ref_get_name(AstVariableRef *self);

#endif /* AST_VARIABLE_REF_INC_X */
