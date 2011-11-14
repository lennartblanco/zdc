#ifndef IR_FOREACH_RANGE_INC_X
#define IR_FOREACH_RANGE_INC_X

#include "ir_loop.h"
#include "ir_code_block.h"
#include "ir_var_value.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FOREACH_RANGE ir_foreach_range_get_type()

#define IR_FOREACH_RANGE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FOREACH_RANGE, IrForeachRange))

#define IR_FOREACH_RANGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FOREACH_RANGE, IrForeachRangeClass))

#define IR_IS_FOREACH_RANGE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FOREACH_RANGE))

#define IR_IS_FOREACH_RANGE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FOREACH_RANGE))

#define IR_FOREACH_RANGE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FOR, IrForeachRangeClass))

typedef struct
{
    IrLoop           parent;

    /* private */
    IrVarValue      *index;
    IrExpression    *lower_exp;
    IrExpression    *upper_exp;
    IrCodeBlock     *body;
} IrForeachRange;

typedef struct
{
    IrLoopClass parent_class;
} IrForeachRangeClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_foreach_range_get_type(void);

IrForeachRange *
ir_foreach_range_new(IrVariable *index,
                     IrExpression *lower_exp,
                     IrExpression *upper_exp,
                     IrCodeBlock *body);

IrVarValue *
ir_foreach_range_get_index(IrForeachRange *self);

IrExpression *
ir_foreach_range_get_lower_exp(IrForeachRange *self);

IrExpression *
ir_foreach_range_get_upper_exp(IrForeachRange *self);

IrCodeBlock *
ir_foreach_range_get_body(IrForeachRange *self);

#endif /* IR_FOREACH_RANGE_INC_X */
