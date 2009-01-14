#ifndef AST_ARRAY_CONSTANT_INC_X
#define AST_ARRAY_CONSTANT_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_ARRAY_CONSTANT ast_array_constant_get_type()

#define XDP_AST_ARRAY_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_ARRAY_CONSTANT, AstArrayConstant))

#define XDP_AST_ARRAY_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_ARRAY_CONSTANT, AstArrayConstantClass))

#define XDP_IS_AST_ARRAY_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_ARRAY_CONSTANT))

#define XDP_IS_AST_ARRAY_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_ARRAY_CONSTANT))

#define XDP_AST_ARRAY_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_ARRAY_CONSTANT, AstArrayConstantClass))

typedef struct
{
    AstConstant parent;
    /* private */
    int length;
    GSList *values;
} AstArrayConstant;

typedef struct
{
    AstConstantClass parent_class;
} AstArrayConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_array_constant_get_type(void);

AstArrayConstant *
ast_array_constant_new();

void
ast_array_constant_add_value(AstArrayConstant *self, AstExpression *value);

GSList *
ast_array_constant_get_values(AstArrayConstant *self);

#endif /* AST_ARRAY_CONSTANT_INC_X */
