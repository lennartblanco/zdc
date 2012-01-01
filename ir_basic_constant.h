#ifndef IR_BASIC_CONSTANT_INC_X
#define IR_BASIC_CONSTANT_INC_X

#include "ir_constant.h"
#include "dt_data_type.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_BASIC_CONSTANT ir_basic_constant_get_type()

#define IR_BASIC_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_BASIC_CONSTANT, IrBasicConstantClass))

#define IR_IS_BASIC_CONSTANT(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_BASIC_CONSTANT))

#define IR_IS_BASIC_CONSTANT_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_BASIC_CONSTANT))

#define IR_BASIC_CONSTANT_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_BASIC_CONSTANT, IrBasicConstantClass))

typedef struct
{
    IrConstant parent;
    /* private */
    DtDataType *type;
    UtRange *value_range;
    union {
        gint32 int_val;
        guint32 uint_val;
        gint16 short_val;
        guint16 ushort_val;
        gint8 byte_val;
        guint8 ubyte_val;
        guint8 char_val;
        bool bool_val;
    };
} IrBasicConstant;

typedef struct
{
    IrConstantClass parent_class;
} IrBasicConstantClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_basic_constant_get_type(void);

IrBasicConstant *
ir_basic_constant(void *obj);

IrBasicConstant *
ir_basic_constant_new_int(gint32 value, guint line_number);

IrBasicConstant *
ir_basic_constant_new_uint(guint32 value, guint line_number);

IrBasicConstant *
ir_basic_constant_new_short(gint16 value);

IrBasicConstant *
ir_basic_constant_new_ushort(guint16 value);

IrBasicConstant *
ir_basic_constant_new_byte(gint8 value);

IrBasicConstant *
ir_basic_constant_new_ubyte(guint8 value);

IrBasicConstant *
ir_basic_constant_new_char(guint8 value, bool immutable, guint line_number);

IrBasicConstant *
ir_basic_constant_new_bool(bool value, guint line_number);

gint32
ir_basic_constant_get_int(IrBasicConstant *self);

guint32
ir_basic_constant_get_uint(IrBasicConstant *self);

gint16
ir_basic_constant_get_short(IrBasicConstant *self);

guint16
ir_basic_constant_get_ushort(IrBasicConstant *self);

gint8
ir_basic_constant_get_byte(IrBasicConstant *self);

guint8
ir_basic_constant_get_ubyte(IrBasicConstant *self);

guint8
ir_basic_constant_get_char(IrBasicConstant *self);

bool
ir_basic_constant_get_bool(IrBasicConstant *self);

#endif /* IR_BASIC_CONSTANT_INC_X */
