#include <stdbool.h>

#include "types.h"
#include "ir_short_constant.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_short_constant_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_short_constant_do_get_data_type(IrExpression *self);

static void
ir_short_constant_do_print(IrNode *self, FILE *out, int indention);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_short_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrShortConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_short_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrShortConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CONSTANT,
                                    "IrShortConstantType",
                                    &info, 0);
    }
    return type;
}

IrShortConstant *
ir_short_constant_new(gint16 value)
{
    IrShortConstant *obj;

    obj = g_object_new(IR_TYPE_SHORT_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

gint16
ir_short_constant_get_value(IrShortConstant *self)
{
    assert(IR_IS_SHORT_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_short_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_short_constant_do_get_data_type;
    ((IrNodeClass *)klass)->do_print = ir_short_constant_do_print;
}

static DtDataType *
ir_short_constant_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_SHORT_CONSTANT(self));

    return types_get_short_type();
}

static void
ir_short_constant_do_print(IrNode *self, FILE *out, int indention)
{
    assert(IR_IS_SHORT_CONSTANT(self));
    assert(out);

    fprintf_indent(out, indention, "%d", IR_SHORT_CONSTANT(self)->value);
}
