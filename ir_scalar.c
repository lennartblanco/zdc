#include <stdbool.h>
#include <string.h>

#include "ir_scalar.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_scalar_class_init(gpointer klass, gpointer dummy);

static AstDataType *
ir_scalar_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_scalar_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrScalarClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_scalar_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrScalar),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_LVALUE,
                                    "IrScalarType",
                                    &info, 0);
    }
    return type;
}

IrScalar *
ir_scalar_new(char *variable_name)
{
    IrScalar *obj;

    obj = g_object_new(IR_TYPE_SCALAR,
                       "ir-lvalue-symbol-name", variable_name,
                       NULL);
    obj->variable = NULL;

    return obj;
}

char *
ir_scalar_get_variable_name(IrScalar *self)
{
    assert(IR_IS_SCALAR(self));

    return ir_lvalue_get_name(IR_LVALUE(self));
}

void
ir_scalar_set_variable(IrScalar *self,
                       IrVariable *variable)
{
    assert(IR_IS_SCALAR(self));
    assert(IR_IS_VARIABLE(variable));

    self->variable = variable;
}

IrVariable *
ir_scalar_get_variable(IrScalar *self)
{
    assert(IR_IS_SCALAR(self));
    assert(IR_IS_VARIABLE(self->variable));

    return self->variable;
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_scalar_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_scalar_do_get_data_type;
}

static AstDataType *
ir_scalar_do_get_data_type(IrExpression *self)
{
    assert(IR_IS_SCALAR(self));

    IrScalar *scalar = IR_SCALAR(self);

    assert(scalar->variable != NULL);

    return ir_expression_get_data_type(IR_EXPRESSION(scalar->variable));
}
