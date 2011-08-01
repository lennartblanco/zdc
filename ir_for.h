#ifndef IR_FOR_INC_X
#define IR_FOR_INC_X

#include "ir_loop.h"
#include "ir_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FOR ir_for_get_type()

#define IR_FOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FOR, IrFor))

#define IR_FOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FOR, IrForClass))

#define IR_IS_FOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FOR))

#define IR_IS_FOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FOR))

#define IR_FOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FOR, IrForClass))

typedef struct
{
    IrLoop           parent;

    /* private */
    IrCodeBlock     *init;
    IrExpression    *test;
    IrExpression    *step;
    IrCodeBlock     *body;
} IrFor;

typedef struct
{
    IrLoopClass parent_class;
} IrForClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_for_get_type(void);

IrFor *
ir_for_new(IrCodeBlock *init,
           IrExpression *test,
           IrExpression *step,
           IrCodeBlock *body);

IrCodeBlock *
ir_for_get_init(IrFor *self);

IrExpression *
ir_for_get_test(IrFor *self);

IrExpression *
ir_for_get_step(IrFor *self);

IrCodeBlock *
ir_for_get_body(IrFor *self);

#endif /* IR_FOR_INC_X */
