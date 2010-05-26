#include <stdbool.h>

#include "types.h"
#include "ir_int_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_int_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_int_constant_do_get_data_type(IrExpression *self);

static void
ir_int_constant_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_int_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrIntConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_int_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrIntConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrIntConstantType",
                                    &info, 0);
    }
    return type;
}

IrIntConstant *
ir_int_constant_new(gint32 value, guint line_number)
{
    IrIntConstant *obj;

    obj = g_object_new(IR_TYPE_INT_CONSTANT,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->value = value;

    return obj;
}

gint32
ir_int_constant_get_value(IrIntConstant *self)
{
    assert(IR_IS_INT_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_int_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_int_constant_do_get_data_type;
    ((IrNodeClass *)klass)->do_print = ir_int_constant_do_print;
}

static DtDataType *
ir_int_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_INT_CONSTANT(self));

    return types_get_int_type();
}

static void
ir_int_constant_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_INT_CONSTANT(self));
    assert(out);

    fprintf_indent(out, indention, "%d", IR_INT_CONSTANT(self)->value);
}
