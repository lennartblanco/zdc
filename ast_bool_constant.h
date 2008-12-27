#ifndef AST_BOOL_CONSTANT_INC_X
#define AST_BOOL_CONSTANT_INC_X

#include <stdbool.h>
#include "ast_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_BOOL_CONSTANT ast_bool_constant_get_type()

#define XDP_AST_BOOL_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_BOOL_CONSTANT, AstBoolConstant))

#define XDP_AST_BOOL_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_BOOL_CONSTANT, AstBoolConstantClass))

#define XDP_IS_AST_BOOL_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_BOOL_CONSTANT))

#define XDP_IS_AST_BOOL_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_BOOL_CONSTANT))

#define XDP_AST_BOOL_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_BOOL_CONSTANT, AstBoolConstantClass))

typedef struct
{
    AstConstant parent;
    /* private */
    bool value;
} AstBoolConstant;

typedef struct
{
    AstConstantClass parent_class;
} AstBoolConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ast_bool_constant_get_type(void);

AstBoolConstant *
ast_bool_constant_new(bool value);

bool
ast_bool_constant_get_value(AstBoolConstant *self);

#endif /* AST_BOOL_CONSTANT_INC_X */
