#include <stdbool.h>

#include "ast_basic_type.h"
#include "ir_bool_constant.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_bool_constant_class_init(gpointer klass, gpointer dummy);

static AstDataType *
ir_bool_constant_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_bool_constant_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrBoolConstantClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_bool_constant_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrBoolConstant),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrBoolConstantType",
                                    &info, 0);
    }
    return type;
}

IrBoolConstant *
ir_bool_constant_new(gint32 value)
{
    IrBoolConstant *obj;

    obj = g_object_new(IR_TYPE_BOOL_CONSTANT, NULL);
    obj->value = value;

    return obj;
}

gboolean
ir_bool_constant_get_value(IrBoolConstant *self)
{
    assert(self);
    assert(IR_IS_BOOL_CONSTANT(self));

    return self->value;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_bool_constant_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_bool_constant_do_get_data_type;
}

static AstDataType *
ir_bool_constant_do_get_data_type(IrExpression *self)
{
  static AstDataType *bool_data_type = NULL;

  if (bool_data_type == NULL) {
      bool_data_type = XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type));
  }
  return bool_data_type;
}
