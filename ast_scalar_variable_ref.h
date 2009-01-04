#ifndef AST_SCALAR_VARIABLE_REF_INC_X
#define AST_SCALAR_VARIABLE_REF_INC_X

#include "ast_variable_ref.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_SCALAR_VARIABLE_REF ast_scalar_variable_ref_get_type()

#define XDP_AST_SCALAR_VARIABLE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_SCALAR_VARIABLE_REF, AstScalarVariableRef))

#define XDP_AST_SCALAR_VARIABLE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_SCALAR_VARIABLE_REF, AstScalarVariableRefClass))

#define XDP_IS_AST_SCALAR_VARIABLE_REF(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_SCALAR_VARIABLE_REF))

#define XDP_IS_AST_SCALAR_VARIABLE_REF_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_SCALAR_VARIABLE_REF))

#define XDP_AST_SCALAR_VARIABLE_REF_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_SCALAR_VARIABLE_REF, AstScalarVariableRefClass))

typedef struct
{
    AstVariableRef      parent;
} AstScalarVariableRef;

typedef struct
{
  AstVariableRefClass parent_class;
} AstScalarVariableRefClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_scalar_variable_ref_get_type(void);

AstScalarVariableRef *
ast_scalar_variable_ref_new(char *name);

#endif /* AST_SCALAR_VARIABLE_REF_INC_X */
