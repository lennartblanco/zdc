#include <stdbool.h>

#include "types.h"
#include "ir_byte_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_byte_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_byte_constant_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_byte_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrByteConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_byte_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrByteConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrByteConstantType",
                                    &info, 0);
    }
    return type;
}

IrByteConstant *
ir_byte_constant_new(gint8 value)
{
    IrByteConstant *obj;

    obj = g_object_new(IR_TYPE_BYTE_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

gint8
ir_byte_constant_get_value(IrByteConstant *self)
{
    assert(IR_IS_BYTE_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_byte_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_byte_constant_do_get_data_type;
}

static DtDataType *
ir_byte_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_BYTE_CONSTANT(self));

    return types_get_byte_type();
}
