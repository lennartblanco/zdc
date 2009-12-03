#ifndef AST_UINT_CONSTANT_INC_X
#define AST_UINT_CONSTANT_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_UINT_CONSTANT ast_uint_constant_get_type()

#define AST_UINT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_UINT_CONSTANT, AstUintConstant))

#define AST_UINT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_UINT_CONSTANT, AstUintConstantClass))

#define AST_IS_UINT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_UINT_CONSTANT))

#define AST_IS_UINT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_UINT_CONSTANT))

#define AST_UINT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_UINT_CONSTANT, AstUintConstantClass))

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
ast_uint_constant_new(guint32 value, guint line_number);

guint32
ast_uint_constant_get_value(AstUintConstant *self);

#endif /* AST_UINT_CONSTANT_INC_X */
