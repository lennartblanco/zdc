#ifndef IR_NODE_INC_X
#define IR_NODE_INC_X

#include <glib-object.h>
#include <stdio.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_NODE ir_node_get_type()

#define IR_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_NODE, IrNode))

#define IR_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_NODE, IrNodeClass))

#define IR_IS_NODE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_NODE))

#define IR_IS_NODE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_NODE))

#define IR_NODE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_NODE, IrNodeClass))

typedef struct
{
  GObject parent;
  /* private */
  guint line_number;
} IrNode;

typedef struct
{
  GObjectClass parent_class;
  /* public virtual methods */
  void (*do_print) (IrNode *self, FILE *out, int indention);
} IrNodeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_node_get_type(void);

/**
 * Get source file line number where this node is defined.
 */
guint
ir_node_get_line_num(void *self);

/**
 * Pretty print this node.
 *
 * @param self      the statment to print
 * @param out       the stream to print to
 * @param indention the indention level to use while printing
 */
void
ir_node_print(IrNode *self, FILE *out, int indention);

#endif /* IR_NODE_INC_X */
