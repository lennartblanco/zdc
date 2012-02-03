#ifndef AST_ATTRIBUTES_INC_X
#define AST_ATTRIBUTES_INC_X

#include "ast_declaration.h"
#include "ast_attribute.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_ATTRIBUTES_TYPE ast_attributes_get_type()

#define AST_ATTRIBUTES(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_ATTRIBUTES_TYPE, AstAttributes))

#define AST_ATTRIBUTES_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_ATTRIBUTES_TYPE, AstAttributesClass))

#define AST_IS_ATTRIBUTES(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_ATTRIBUTES_TYPE))

#define AST_IS_ATTRIBUTES_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_ATTRIBUTES_TYPE))

#define AST_ATTRIBUTES_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_ATTRIBUTES_TYPE, AstAttributesClass))

typedef struct
{
    AstDeclaration    parent;

    /* private */
    GSList           *attrs;
} AstAttributes;

typedef struct
{
    AstDeclarationClass parent_class;
} AstAttributesClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_attributes_get_type(void);

AstAttributes *
ast_attributes_new(AstAttribute *first_attr);

void
ast_attributes_add_attribute(AstAttributes *self, AstAttribute *attribute);

GSList *
ast_attributes_get_attributes(AstAttributes *self);

#endif /* AST_ATTRIBUTES_INC_X */
