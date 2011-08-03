#include <stdbool.h>

#include "types.h"
#include "ir_basic_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_basic_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_basic_constant_do_get_data_type(IrExpression *self);

static IrBasicConstant *
ir_basic_constant_new(basic_data_type_t type, void* value, guint line_number);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_basic_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrBasicConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_basic_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrBasicConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrBasicConstantType",
                                    &info, 0);
    }
    return type;
}

IrBasicConstant *
ir_basic_constant_new_int(gint32 value, guint line_number)
{
    return ir_basic_constant_new(int_type, &value, line_number);
}

IrBasicConstant *
ir_basic_constant_new_uint(guint32 value, guint line_number)
{
    return ir_basic_constant_new(uint_type, &value, line_number);
}

IrBasicConstant *
ir_basic_constant_new_short(gint16 value)
{
    return ir_basic_constant_new(short_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_ushort(guint16 value)
{
    return ir_basic_constant_new(ushort_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_byte(gint8 value)
{
    return ir_basic_constant_new(byte_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_ubyte(guint8 value)
{
    return ir_basic_constant_new(ubyte_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_char(guint8 value, guint line_number)
{
    return ir_basic_constant_new(char_type, &value, line_number);
}

IrBasicConstant *
ir_basic_constant_new_bool(bool value, guint line_number)
{
    return ir_basic_constant_new(bool_type, &value, line_number);
}

gint32
ir_basic_constant_get_int(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == int_type);

    return self->int_val;
}

guint32
ir_basic_constant_get_uint(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == uint_type);

    return self->uint_val;
}

gint16
ir_basic_constant_get_short(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == short_type);

    return self->short_val;
}

guint16
ir_basic_constant_get_ushort(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == ushort_type);

    return self->ushort_val;
}

gint8
ir_basic_constant_get_byte(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == byte_type);

    return self->byte_val;
}

guint8
ir_basic_constant_get_ubyte(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == ubyte_type);

    return self->ubyte_val;
}

guint8
ir_basic_constant_get_char(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == char_type);

    return self->char_val;
}

bool
ir_basic_constant_get_bool(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(self->type == bool_type);

    return self->bool_val;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrBasicConstant *
ir_basic_constant_new(basic_data_type_t type, void* value, guint line_number)
{
    IrBasicConstant *obj;

    obj = g_object_new(IR_TYPE_BASIC_CONSTANT,
                       "ir-node-line-number",
                       line_number, NULL);

    obj->type = type;
    switch (type)
    {
        case int_type:
            obj->int_val = *((gint32*)value);
            break;
        case uint_type:
            obj->uint_val = *((guint32*)value);
            break;
        case short_type:
            obj->short_val = *((gint16*)value);
            break;
        case ushort_type:
            obj->ushort_val = *((guint16*)value);
            break;
        case byte_type:
            obj->byte_val = *((gint8*)value);
            break;
        case ubyte_type:
            obj->ubyte_val = *((guint8*)value);
            break;
        case char_type:
            obj->char_val = *((guint8*)value);
            break;
        case bool_type:
            obj->bool_val = *((bool*)value);
            break;
        default:
            assert(false); /* unexpected basic data type */
    }

    return obj;
}

static void
ir_basic_constant_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_basic_constant_do_get_data_type;
}

static DtDataType *
ir_basic_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));

    switch (IR_BASIC_CONSTANT(self)->type)
    {
        case int_type:
            return types_get_int_type();
        case uint_type:
            return types_get_uint_type();
        case short_type:
            return types_get_short_type();
        case ushort_type:
            return types_get_ushort_type();
        case byte_type:
            return types_get_byte_type();
        case ubyte_type:
            return types_get_ubyte_type();
        case char_type:
            return types_get_char_type();
        case bool_type:
            return types_get_bool_type();
        default:
            assert(false); /* unexpected data type */
    }
}
