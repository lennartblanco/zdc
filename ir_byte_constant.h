#ifndef IR_BYTE_CONSTANT_INC_X
#define IR_BYTE_CONSTANT_INC_X

#include "ir_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_BYTE_CONSTANT ir_byte_constant_get_type()

#define IR_BYTE_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_BYTE_CONSTANT, IrByteConstant))

#define IR_BYTE_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_BYTE_CONSTANT, IrByteConstantClass))

#define IR_IS_BYTE_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_BYTE_CONSTANT))

#define IR_IS_BYTE_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_BYTE_CONSTANT))

#define IR_BYTE_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_BYTE_CONSTANT, IrByteConstantClass))

typedef struct
{
    IrConstant parent;
    /* private */
    gint8 value;
} IrByteConstant;

typedef struct
{
    IrConstantClass parent_class;
} IrByteConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_byte_constant_get_type(void);

IrByteConstant *
ir_byte_constant_new(gint8 value);

gint8
ir_byte_constant_get_value(IrByteConstant *self);

#endif /* IR_BYTE_CONSTANT_INC_X */
