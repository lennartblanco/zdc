#ifndef IR_STATMENT_INC_X
#define IR_STATMENT_INC_X

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
} IrStatmentClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_statment_get_type(void);

#endif /* IR_STATMENT_INC_X */
