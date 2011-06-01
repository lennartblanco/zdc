#include <stdbool.h>

#include "types.h"
#include "ir_ushort_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_ushort_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_ushort_constant_do_get_data_type(IrExpression *self);

static void
ir_ushort_constant_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_ushort_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrUshortConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_ushort_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrUshortConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrUshortConstantType",
                                    &info, 0);
    }
    return type;
}

IrUshortConstant *
ir_ushort_constant_new(guint16 value)
{
    IrUshortConstant *obj;

    obj = g_object_new(IR_TYPE_USHORT_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

guint16
ir_ushort_constant_get_value(IrUshortConstant *self)
{
    assert(IR_IS_USHORT_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_ushort_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_ushort_constant_do_get_data_type;
    ((IrNodeClass *)klass)->do_print = ir_ushort_constant_do_print;
}

static DtDataType *
ir_ushort_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_USHORT_CONSTANT(self));

    return types_get_ushort_type();
}

static void
ir_ushort_constant_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_USHORT_CONSTANT(self));
    assert(out);

    fprintf_indent(out, indention, "%u", IR_USHORT_CONSTANT(self)->value);
}
