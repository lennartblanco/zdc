#ifndef IR_FOREACH_INC_X
#define IR_FOREACH_INC_X

#include "ir_statment.h"
#include "ir_variable.h"
#include "ir_code_block.h"
#include "ir_array_slice.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_FOREACH ir_foreach_get_type()

#define IR_FOREACH(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_FOREACH, IrForeach))

#define IR_FOREACH_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_FOREACH, IrForeachClass))

#define IR_IS_FOREACH(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_FOREACH))

#define IR_IS_FOREACH_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_FOREACH))

#define IR_FOREACH_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_FOREACH, IrForeachClass))

typedef struct
{
    IrStatment          parent;

    /* private */
    IrVariable         *index;
    IrVariable         *value;
    IrArraySlice       *aggregate;
    IrCodeBlock        *body;
} IrForeach;

typedef struct
{
    IrStatmentClass parent_class;
} IrForeachClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_foreach_get_type(void);

IrForeach *
ir_foreach_new(IrVariable *index,
               IrVariable *value,
               IrArraySlice *aggregate,
               IrCodeBlock *body);


void
ir_foreach_set_index(IrForeach *self, IrVariable *index);

IrVariable *
ir_foreach_get_index(IrForeach *self);

IrVariable *
ir_foreach_get_value(IrForeach *self);

IrArraySlice *
ir_foreach_get_aggregate(IrForeach *self);

IrCodeBlock *
ir_foreach_get_body(IrForeach *self);

#endif /* IR_FOREACH_INC_X */
