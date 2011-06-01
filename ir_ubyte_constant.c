#include <stdbool.h>

#include "types.h"
#include "ir_ubyte_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_ubyte_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_ubyte_constant_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_ubyte_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrUbyteConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_ubyte_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrUbyteConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrUbyteConstantType",
                                    &info, 0);
    }
    return type;
}

IrUbyteConstant *
ir_ubyte_constant_new(guint8 value)
{
    IrUbyteConstant *obj;

    obj = g_object_new(IR_TYPE_UBYTE_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

guint8
ir_ubyte_constant_get_value(IrUbyteConstant *self)
{
    assert(IR_IS_UBYTE_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_ubyte_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_ubyte_constant_do_get_data_type;
}

static DtDataType *
ir_ubyte_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_UBYTE_CONSTANT(self));

    return types_get_ubyte_type();
}
