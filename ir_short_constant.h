#ifndef IR_SHORT_CONSTANT_INC_X
#define IR_SHORT_CONSTANT_INC_X

#include "ir_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_SHORT_CONSTANT ir_short_constant_get_type()

#define IR_SHORT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_SHORT_CONSTANT, IrShortConstant))

#define IR_SHORT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_SHORT_CONSTANT, IrShortConstantClass))

#define IR_IS_SHORT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_SHORT_CONSTANT))

#define IR_IS_SHORT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_SHORT_CONSTANT))

#define IR_SHORT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_SHORT_CONSTANT, IrShortConstantClass))

typedef struct
{
    IrConstant parent;
    /* private */
    gint16 value;
} IrShortConstant;

typedef struct
{
    IrConstantClass parent_class;
} IrShortConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_short_constant_get_type(void);

IrShortConstant *
ir_short_constant_new(gint16 value);

gint16
ir_short_constant_get_value(IrShortConstant *self);

#endif /* IR_SHORT_CONSTANT_INC_X */
