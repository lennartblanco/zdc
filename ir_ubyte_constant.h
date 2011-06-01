#ifndef IR_UBYTE_CONSTANT_INC_X
#define IR_UBYTE_CONSTANT_INC_X

#include "ir_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_UBYTE_CONSTANT ir_ubyte_constant_get_type()

#define IR_UBYTE_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_UBYTE_CONSTANT, IrUbyteConstant))

#define IR_UBYTE_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_UBYTE_CONSTANT, IrUbyteConstantClass))

#define IR_IS_UBYTE_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_UBYTE_CONSTANT))

#define IR_IS_UBYTE_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_UBYTE_CONSTANT))

#define IR_UBYTE_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_UBYTE_CONSTANT, IrUbyteConstantClass))

typedef struct
{
    IrConstant parent;
    /* private */
    guint8 value;
} IrUbyteConstant;

typedef struct
{
    IrConstantClass parent_class;
} IrUbyteConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_ubyte_constant_get_type(void);

IrUbyteConstant *
ir_ubyte_constant_new(guint8 value);

guint8
ir_ubyte_constant_get_value(IrUbyteConstant *self);

#endif /* IR_UBYTE_CONSTANT_INC_X */
