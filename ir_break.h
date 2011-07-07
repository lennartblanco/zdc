#ifndef IR_BREAK_INC_X
#define IR_BREAK_INC_X

#include "ir_statment.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_BREAK ir_break_get_type()

#define IR_BREAK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_BREAK, IrBreak))

#define IR_BREAK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_BREAK, IrBreakClass))

#define IR_IS_BREAK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_BREAK))

#define IR_IS_BREAK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_BREAK))

#define IR_BREAK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_BREAK, IrReturnClass))

typedef struct
{
    IrStatment          parent;
} IrBreak;

typedef struct
{
    IrStatmentClass parent_class;
} IrBreakClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_break_get_type(void);

IrBreak *
ir_break_new(guint line_number);

#endif /* IR_BREAK_INC_X */
