#include <stdbool.h>
#include <string.h>

#include "ir_scalar.h"

#include <assert.h>

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
        NULL, //ir_scalar_class_init, /* class_init */
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
ir_scalar_new(char *variable_name, guint line_number)
{
    IrScalar *obj;

    obj = g_object_new(IR_TYPE_SCALAR,
                       "ir-node-line-number", line_number,
                       "ir-lvalue-symbol-name", variable_name,
                       NULL);
    return obj;
}

void
ir_scalar_set_variable(IrScalar *self,
                       IrVariable *variable)
{
    assert(IR_IS_SCALAR(self));
    assert(IR_IS_VARIABLE(variable));

    ir_lvalue_set_variable(IR_LVALUE(self), variable);
}

char *
ir_scalar_get_variable_name(IrScalar *self)
{
    assert(IR_IS_SCALAR(self));

    return ir_lvalue_get_name(IR_LVALUE(self));
}

