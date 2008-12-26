#ifndef AST_INT_CONSTANT_INC_X
#define AST_INT_CONSTANT_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_INT_CONSTANT ast_int_constant_get_type()

#define XDP_AST_INT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_INT_CONSTANT, AstIntConstant))

#define XDP_AST_INT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_INT_CONSTANT, AstIntConstantClass))

#define XDP_IS_AST_INT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_INT_CONSTANT))

#define XDP_IS_AST_INT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_INT_CONSTANT))

#define XDP_AST_INT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_INT_CONSTANT, AstIntConstantClass))

typedef struct
{
    AstConstant parent;
    /* private */
    gint32 value;
} AstIntConstant;

typedef struct
{
    AstConstantClass parent_class;
} AstIntConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_int_constant_get_type(void);

AstIntConstant *
ast_int_constant_new(gint32 value);

#endif /* AST_INT_CONSTANT_INC_X */
