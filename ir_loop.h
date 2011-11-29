#ifndef IR_LOOP_INC_X
#define IR_LOOP_INC_X

#include "ir_statment.h"
#include "iml_operation.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_LOOP ir_loop_get_type()

#define IR_LOOP(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_LOOP, IrLoop))

#define IR_LOOP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_LOOP, IrLoopClass))

#define IR_IS_LOOP(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_LOOP))

#define IR_IS_LOOP_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_LOOP))

#define IR_LOOP_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_LOOP, IrLoopClass))

typedef struct
{
    IrStatment parent;

    /* private */
    iml_operation_t *end_label;
} IrLoop;

typedef struct
{
    IrStatmentClass parent_class;
} IrLoopClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_loop_get_type(void);

/**
 * Set the IML exit label for this loop. The label to use for breaking out
 * of the loop.
 */
void
ir_loop_set_exit_label(IrLoop *self, iml_operation_t *label);

/**
 * Get this loop's exit label, see ir_while_set_exit_label().
 */
iml_operation_t *
ir_loop_get_exit_label(IrLoop *self);

#endif /* IR_LOOP_INC_X */
