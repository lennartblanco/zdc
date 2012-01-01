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
ir_basic_constant_new(bool immutable,
                      basic_data_type_t type,
                      void* value,
                      guint line_number);

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
ir_basic_constant(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj),
                                      IR_TYPE_BASIC_CONSTANT,
                                      IrBasicConstant);
}

IrBasicConstant *
ir_basic_constant_new_int(gint32 value, guint line_number)
{
    return ir_basic_constant_new(false, int_type, &value, line_number);
}

IrBasicConstant *
ir_basic_constant_new_uint(guint32 value, guint line_number)
{
    return ir_basic_constant_new(false, uint_type, &value, line_number);
}

IrBasicConstant *
ir_basic_constant_new_short(gint16 value)
{
    return ir_basic_constant_new(false, short_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_ushort(guint16 value)
{
    return ir_basic_constant_new(false, ushort_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_byte(gint8 value)
{
    return ir_basic_constant_new(false, byte_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_ubyte(guint8 value)
{
    return ir_basic_constant_new(false, ubyte_type, &value, 0);
}

IrBasicConstant *
ir_basic_constant_new_char(guint8 value, bool immutable, guint line_number)
{
    return ir_basic_constant_new(immutable, char_type, &value, line_number);
}

IrBasicConstant *
ir_basic_constant_new_bool(bool value, guint line_number)
{
    return ir_basic_constant_new(false, bool_type, &value, line_number);
}

gint32
ir_basic_constant_get_int(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_int(self->type));

    return self->int_val;
}

guint32
ir_basic_constant_get_uint(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_uint(self->type));

    return self->uint_val;
}

gint16
ir_basic_constant_get_short(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_short(self->type));

    return self->short_val;
}

guint16
ir_basic_constant_get_ushort(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_ushort(self->type));

    return self->ushort_val;
}

gint8
ir_basic_constant_get_byte(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_byte(self->type));

    return self->byte_val;
}

guint8
ir_basic_constant_get_ubyte(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_ubyte(self->type));

    return self->ubyte_val;
}

guint8
ir_basic_constant_get_char(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_char(self->type));

    return self->char_val;
}

bool
ir_basic_constant_get_bool(IrBasicConstant *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));
    assert(dt_basic_is_bool(self->type));

    return self->bool_val;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrBasicConstant *
ir_basic_constant_new(bool immutable,
                      basic_data_type_t type,
                      void* value,
                      guint line_number)
{
    IrBasicConstant *obj;

    obj = g_object_new(IR_TYPE_BASIC_CONSTANT,
                       "ir-node-line-number",
                       line_number, NULL);

    obj->type = DT_DATA_TYPE(dt_basic_new(type));
    dt_data_type_set_immutable(obj->type, immutable);
    obj->value_range = NULL;

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

static UtRange *
ir_basic_constant_get_value_range(IrExpression *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));

    if (ir_basic_constant(self)->value_range == NULL)
    {
        IrBasicConstant *bc = ir_basic_constant(self);
        gint64 val;
        switch (dt_basic_get_data_type(dt_basic(bc->type)))
        {
            case int_type:
                val = bc->int_val;
                break;
            case uint_type:
                val = bc->uint_val;
                break;
            case short_type:
                val = bc->short_val;
                break;
            case ushort_type:
                val = bc->ushort_val;
                break;
            case byte_type:
                val = bc->byte_val;
                break;
            case ubyte_type:
                val = bc->ubyte_val;
                break;
            case char_type:
                val = bc->char_val;
                break;
            case bool_type:
                val = bc->bool_val;
                break;
            default:
                assert(false); /* unexpected basic data type */
        }
        bc->value_range = ut_range_new(val, val);
    }

    return ir_basic_constant(self)->value_range;
}

static void
ir_basic_constant_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_basic_constant_do_get_data_type;
    IR_EXPRESSION_CLASS(klass)->get_value_range =
        ir_basic_constant_get_value_range;
}

static DtDataType *
ir_basic_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_BASIC_CONSTANT(self));

    return ir_basic_constant(self)->type;
}
