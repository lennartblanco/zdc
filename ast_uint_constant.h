#ifndef AST_UINT_CONSTANT_INC_X
#define AST_UINT_CONSTANT_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_UINT_CONSTANT ast_uint_constant_get_type()

#define XDP_AST_UINT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_UINT_CONSTANT, AstUintConstant))

#define XDP_AST_UINT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_UINT_CONSTANT, AstUintConstantClass))

#define XDP_IS_AST_UINT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_UINT_CONSTANT))

#define XDP_IS_AST_UINT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_UINT_CONSTANT))

#define XDP_AST_UINT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_UINT_CONSTANT, AstUintConstantClass))

typedef struct
{
    AstConstant parent;
    /* private */
    guint32 value;
} AstUintConstant;

typedef struct
{
    AstConstantClass parent_class;
} AstUintConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_uint_constant_get_type(void);

AstUintConstant *
ast_uint_constant_new(guint32 value);

guint32
ast_uint_constant_get_value(AstUintConstant *self);

#endif /* AST_UINT_CONSTANT_INC_X */
