#ifndef AST_INT_CONSTANT_INC_X
#define AST_INT_CONSTANT_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_INT_CONSTANT ast_int_constant_get_type()

#define AST_INT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_INT_CONSTANT, AstIntConstant))

#define AST_INT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_INT_CONSTANT, AstIntConstantClass))

#define AST_IS_INT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_INT_CONSTANT))

#define AST_IS_INT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_INT_CONSTANT))

#define AST_INT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_INT_CONSTANT, AstIntConstantClass))

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
ast_int_constant_new(gint32 value, guint line_number);

gint32
ast_int_constant_get_value(AstIntConstant *self);

#endif /* AST_INT_CONSTANT_INC_X */
