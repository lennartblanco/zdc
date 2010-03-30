#include <stdbool.h>

#include "types.h"
#include "ir_uint_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_uint_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_uint_constant_do_get_data_type(IrExpression *self);

static void
ir_uint_constant_do_print(IrStatment *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_uint_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrUintConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_uint_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrUintConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrUintConstantType",
                                    &info, 0);
    }
    return type;
}

IrUintConstant *
ir_uint_constant_new(guint32 value, guint line_number)
{
    IrUintConstant *obj;

    obj = g_object_new(IR_TYPE_UINT_CONSTANT,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->value = value;

    return obj;
}

guint32
ir_uint_constant_get_value(IrUintConstant *self)
{
    assert(IR_IS_UINT_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_uint_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_uint_constant_do_get_data_type;
    ((IrStatmentClass *)klass)->do_print = ir_uint_constant_do_print;
}

static DtDataType *
ir_uint_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_UINT_CONSTANT(self));

    return types_get_uint_type();
}

static void
ir_uint_constant_do_print(IrStatment *self, FILE *out, int indention)
{
    assert(IR_IS_UINT_CONSTANT(self));
    assert(out);

    fprintf_indent(out, indention, "%u", IR_UINT_CONSTANT(self)->value);
}
