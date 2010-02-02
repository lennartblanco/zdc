#ifndef AST_PROPERTY_INC_X
#define AST_PROPERTY_INC_X

#include "ast_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_PROPERTY ast_property_get_type()

#define AST_PROPERTY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_PROPERTY, AstProperty))

#define AST_PROPERTY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_PROPERTY, AstPropertyClass))

#define AST_IS_PROPERTY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_PROPERTY))

#define AST_IS_PROPERTY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_PROPERTY))

#define AST_PROPERTY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_PROPERTY, AstPropertyClass))

typedef struct
{
    AstExpression parent;

    /* private */
    AstExpression *exp;
    const char *name;
} AstProperty;

typedef struct
{
    AstExpressionClass parent_class;
} AstPropertyClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_property_get_type(void);

/**
 * @param name property name
 */
AstProperty *
ast_property_new(AstExpression *exp,
                 const char *name);

AstExpression *
ast_property_get_expression(AstProperty *self);

const char *
ast_property_get_name(AstProperty *self);

#endif /* AST_PROPERTY_INC_X */
