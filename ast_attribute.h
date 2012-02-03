#ifndef AST_ATTRIBUTE_INC_X
#define AST_ATTRIBUTE_INC_X

#include "ast_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_ATTRIBUTE ast_attribute_get_type()

#define AST_ATTRIBUTE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_ATTRIBUTE, AstAttribute))

#define AST_ATTRIBUTE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_ATTRIBUTE, AstAttributeClass))

#define AST_IS_ATTRIBUTE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_ATTRIBUTE))

#define AST_IS_ATTRIBUTE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_ATTRIBUTE))

#define AST_ATTRIBUTE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_ATTRIBUTE, AstAttributeClass))

typedef struct
{
    AstNode parent;
} AstAttribute;

typedef struct
{
    AstNodeClass parent_class;
} AstAttributeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_attribute_get_type(void);

#endif /* AST_ATTRIBUTE_INC_X */
