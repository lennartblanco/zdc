#include "ir_address_of.h"
#include "dt_pointer.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_address_of_class_init(gpointer klass, gpointer dummy);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_address_of_get_type(void)
{
    static GType type = 0;
    if (type == 0)
    {
      static const GTypeInfo info =
      {
        sizeof (IrAddressOfClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_address_of_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrAddressOf),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrAddressOfType",
                                    &info, 0);
    }
    return type;
}

IrAddressOf *
ir_address_of_new(IrExpression *expression, guint line_number)
{
    IrAddressOf *obj;

    assert(IR_IS_EXPRESSION(expression));

    obj = g_object_new(IR_TYPE_ADDRESS_OF,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->expression = expression;
    obj->type = NULL;

    return obj;
}

IrExpression *
ir_address_of_get_expression(IrAddressOf *self)
{
    assert(IR_IS_ADDRESS_OF(self));

    return self->expression;
}

void
ir_address_of_set_expression(IrAddressOf *self, IrExpression *expression)
{
    assert(IR_IS_ADDRESS_OF(self));
    assert(IR_IS_EXPRESSION(expression));

    self->expression = expression;
    self->type = NULL;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static DtDataType *
ir_address_of_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_ADDRESS_OF(self));
    IrAddressOf *addr_of = IR_ADDRESS_OF(self);

    if (addr_of->type == NULL)
    {
        addr_of->type =
           DT_DATA_TYPE(
             dt_pointer_new(ir_expression_get_data_type(addr_of->expression)));
    }
    return addr_of->type;
}

static void
ir_address_of_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_address_of_do_get_data_type;
}
