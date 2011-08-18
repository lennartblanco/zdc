#ifndef AST_CAST_INC_X
#define AST_CAST_INC_X

#include "ast_expression.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_CAST ast_cast_get_type()

#define AST_CAST(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_CAST, AstCast))

#define AST_CAST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_CAST, AstCastClass))

#define AST_IS_CAST(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_CAST))

#define AST_IS_CAST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_CAST))

#define AST_CAST_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_CAST, AstCastClass))

typedef struct
{
    AstExpression    parent;

    /* private */
    DtDataType *target_type;
    AstExpression *value;
} AstCast;

typedef struct
{
    AstExpressionClass parent_class;
} AstCastClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_cast_get_type(void);

AstCast *
ast_cast_new(DtDataType *target_type, AstExpression *value);

DtDataType *
ast_cast_get_target_type(AstCast *self);

AstExpression *
ast_cast_get_value(AstCast *self);

#endif /* AST_CAST_INC_X */
