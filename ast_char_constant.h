#ifndef AST_CHAR_CONSTANT_INC_X
#define AST_CHAR_CONSTANT_INC_X

#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_CHAR_CONSTANT ast_char_constant_get_type()

#define AST_CHAR_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_CHAR_CONSTANT, AstCharConstant))

#define AST_CHAR_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_CHAR_CONSTANT, AstCharConstantClass))

#define AST_IS_CHAR_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_CHAR_CONSTANT))

#define AST_IS_CHAR_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_CHAR_CONSTANT))

#define AST_CHAR_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_CHAR_CONSTANT, AstCharConstantClass))

typedef struct
{
    AstConstant parent;
    /* private */
    guint8 value;
} AstCharConstant;

typedef struct
{
    AstConstantClass parent_class;
} AstCharConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_char_constant_get_type(void);

AstCharConstant *
ast_char_constant_new(guint8 value, guint line_number);

guint8
ast_char_constant_get_value(AstCharConstant *self);

#endif /* AST_CHAR_CONSTANT_INC_X */
