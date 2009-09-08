#include <stdbool.h>

#include "ir_array_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_array_constant_class_init(gpointer klass, gpointer dummy);

static AstDataType *
ir_array_constant_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_array_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrArrayConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_array_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrArrayConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrArrayConstantType",
                                    &info, 0);
    }
    return type;
}

IrArrayConstant *
ir_array_constant_new(gint32 value)
{
    IrArrayConstant *obj;

    obj = g_object_new(IR_TYPE_ARRAY_CONSTANT, NULL);
    obj->values = NULL;

    return obj;
}

void
ir_array_constant_add_value(IrArrayConstant *self, IrExpression *value)
{
    assert(IR_IS_ARRAY_CONSTANT(self));
    assert(IR_IS_EXPRESSION(value));

    self->values = g_slist_append(self->values, value);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_array_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_array_constant_do_get_data_type;
}

static AstDataType *
ir_array_constant_do_get_data_type(IrExpression *self)
{
  /* not implemented */
  assert(false);
  return NULL;
}
