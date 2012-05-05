#ifndef AST_NEW_INC_X
#define AST_NEW_INC_X

#include "ast_expression.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_NEW ast_new_get_type()

#define AST_NEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_NEW, AstNew))

#define AST_NEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_NEW, AstNewClass))

#define AST_IS_NEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_NEW))

#define AST_IS_NEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_NEW))

#define AST_NEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_NEW, AstNewClass))

typedef struct
{
    AstExpression parent;

    /* private */
    DtDataType *type;
} AstNew;

typedef struct
{
    AstExpressionClass parent_class;
} AstNewClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_new_get_type(void);

AstNew *
ast_new_new(DtDataType *type, guint line_number);

DtDataType *
ast_new_get_dt_type(AstNew *self);

#endif /* AST_NEW_INC_X */
