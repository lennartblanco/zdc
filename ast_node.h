#ifndef AST_NODE_INC_X
#define AST_NODE_INC_X

#include <glib-object.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define XDP_TYPE_AST_NODE ast_node_get_type()

#define XDP_AST_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), XDP_TYPE_AST_NODE, AstNode))

#define XDP_AST_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), XDP_TYPE_AST_NODE, AstNodeClass))

#define XDP_IS_AST_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XDP_TYPE_AST_NODE))

#define XDP_IS_AST_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), XDP_TYPE_AST_NODE))

#define XDP_AST_NODE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), XDP_TYPE_AST_NODE, AstNodeClass))

typedef struct 
{
  GObject parent;
} AstNode;

typedef struct 
{
  GObjectClass parent_class;
  /* public virtual methods */
  void (*do_print) (AstNode *self, FILE *out);
} AstNodeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ast_node_get_type(void);

AstNode*
ast_node_new(void);

void
ast_node_print(AstNode *self, FILE *out);

#endif /* AST_NODE_INC_X */
