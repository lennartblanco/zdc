#include <string.h>

#include "ir_method_call.h"
#include "ir_address_of.h"
#include "dt_pointer.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_method_call_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_method_call_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrMethodCallClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_method_call_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrMethodCall),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_CALL,
                                    "IrMethodCallType",
                                    &info, 0);
    }
    return type;
}

IrMethodCall *
ir_method_call_new(IrExpression *this_exp,
                   const char *method_name,
                   GSList *arguments,
                   guint line_number)
{
    IrMethodCall *obj;

    obj = g_object_new(IR_TYPE_METHOD_CALL,
                       "ir-node-line-number", line_number,
                       "ir-call-arguments", arguments,
                       NULL);

    obj->this_exp = this_exp;
    obj->method_name = method_name;

    return obj;
}

IrExpression *
ir_method_call_get_this_exp(IrMethodCall *self)
{
    assert(IR_IS_METHOD_CALL(self));

    return self->this_exp;
}

void
ir_method_call_set_this_exp(IrMethodCall *self, IrExpression *this_exp)
{
    assert(IR_IS_METHOD_CALL(self));
    assert(IR_IS_EXPRESSION(this_exp));

    self->this_exp = this_exp;
}

const char *
ir_method_call_get_name(IrMethodCall *self)
{
    assert(IR_IS_METHOD_CALL(self));

    return self->method_name;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static IrExpression *
ir_method_get_this_arg(IrCall *self)
{
    assert(IR_IS_METHOD_CALL(self));

    IrExpression *this_arg = IR_METHOD_CALL(self)->this_exp;

    if (!DT_IS_POINTER(ir_expression_get_data_type(this_arg)))
    {
        this_arg = ir_expression(ir_address_of_new(this_arg, 0));
    }

    return this_arg;
}

static void
ir_method_call_class_init(gpointer klass, gpointer dummy)
{
    IR_CALL_CLASS(klass)->do_get_this_arg = ir_method_get_this_arg;
}
