#ifndef IR_WHILE_INC_X
#define IR_WHILE_INC_X

#include "ir_loop.h"
#include "ir_code_block.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_WHILE ir_while_get_type()

#define IR_WHILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_WHILE, IrWhile))

#define IR_WHILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_WHILE, IrWhileClass))

#define IR_IS_WHILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_WHILE))

#define IR_IS_WHILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_WHILE))

#define IR_WHILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_WHILE, IrWhileClass))

typedef struct
{
    IrLoop           parent;
    /* private */
    IrExpression    *loop_condition;
    IrCodeBlock     *body;
    iml_operation_t *end_label;
} IrWhile;

typedef struct
{
    IrLoopClass parent_class;
} IrWhileClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_while_get_type(void);

IrWhile *
ir_while_new(IrExpression *loop_condition, IrCodeBlock *body);

IrExpression *
ir_while_get_loop_condition(IrWhile *self);

void
ir_while_set_loop_condition(IrWhile *self, IrExpression *loop_condition);

IrCodeBlock *
ir_while_get_body(IrWhile *self);

#endif /* IR_WHILE_INC_X */
