#ifndef IR_STATMENT_INC_X
#define IR_STATMENT_INC_X

#include <stdio.h>

#include "ir_node.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_STATMENT ir_statment_get_type()

#define IR_STATMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_STATMENT, IrStatment))

#define IR_STATMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_STATMENT, IrStatmentClass))

#define IR_IS_STATMENT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_STATMENT))

#define IR_IS_STATMENT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_STATMENT))

#define IR_STATMENT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_STATMENT, IrStatmentClass))

typedef struct 
{
  IrNode parent;
} IrStatment;

typedef struct 
{
  IrNodeClass parent_class;
  /* public virtual methods */
  void (*do_print) (IrStatment *self, FILE *out, int indention); /* @todo move this method to IrNode class */
} IrStatmentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_statment_get_type(void);

/**
 * Pretty print this statment.
 *
 * @param self      the statment to print
 * @param out       the stream to print to
 * @param indention the indention level to use while printing
 */
void
ir_statment_print(IrStatment *self, FILE *out, int indention);

#endif /* IR_STATMENT_INC_X */
