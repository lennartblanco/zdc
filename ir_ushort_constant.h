#ifndef IR_USHORT_CONSTANT_INC_X
#define IR_USHORT_CONSTANT_INC_X

#include "ir_constant.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_USHORT_CONSTANT ir_ushort_constant_get_type()

#define IR_USHORT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_USHORT_CONSTANT, IrUshortConstant))

#define IR_USHORT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_USHORT_CONSTANT, IrUshortConstantClass))

#define IR_IS_USHORT_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_USHORT_CONSTANT))

#define IR_IS_USHORT_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_USHORT_CONSTANT))

#define IR_USHORT_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_USHORT_CONSTANT, IrUshortConstantClass))

typedef struct
{
    IrConstant parent;
    /* private */
    guint16 value;
} IrUshortConstant;

typedef struct
{
    IrConstantClass parent_class;
} IrUshortConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_ushort_constant_get_type(void);

IrUshortConstant *
ir_ushort_constant_new(guint16 value);

guint16
ir_ushort_constant_get_value(IrUshortConstant *self);

#endif /* IR_USHORT_CONSTANT_INC_X */
