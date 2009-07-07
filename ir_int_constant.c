#include <stdbool.h>

#include "ast_basic_type.h"
#include "ir_int_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_int_constant_class_init(gpointer klass, gpointer dummy);

static AstDataType *
ir_int_constant_do_get_data_type(IrExpression *self);

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
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrIntConstantType",
                                    &info, 0);
    }
    return type;
}

IrIntConstant *
ir_int_constant_new(gint32 value)
{
    IrIntConstant *obj;

    obj = g_object_new(IR_TYPE_INT_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

gint32
ir_int_constant_get_value(IrIntConstant *self)
{
    assert(self);
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
}

static AstDataType *
ir_int_constant_do_get_data_type(IrExpression *self)
{
  static AstDataType *int_data_type = NULL;

  if (int_data_type == NULL) {
      int_data_type = XDP_AST_DATA_TYPE(ast_basic_type_new(int_type));
  }
  return int_data_type;
}
