#ifndef AST_NODE_INC_X
#define AST_NODE_INC_X

#include <glib-object.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define AST_TYPE_NODE ast_node_get_type()

#define AST_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), AST_TYPE_NODE, AstNode))

#define AST_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), AST_TYPE_NODE, AstNodeClass))

#define AST_IS_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), AST_TYPE_NODE))

#define AST_IS_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), AST_TYPE_NODE))

#define AST_NODE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), AST_TYPE_NODE, AstNodeClass))

typedef struct 
{
  GObject parent;
  /* private */
  guint line_number;
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

/**
 * Get source file line number where this node is defined.
 */
guint
ast_node_get_line_num(void *self);

void
ast_node_print(AstNode *self, FILE *out);

#endif /* AST_NODE_INC_X */
