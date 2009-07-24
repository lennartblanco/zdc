#include <stdbool.h>

#include "ir_expression.h"

#include <assert.h>
/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_expression_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrExpressionClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        NULL,   /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrExpression),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrExpression",
                                    &info, 0);
    }
    return type;
}

AstDataType *
ir_expression_get_data_type(IrExpression *self)
{
    assert(self);
    assert(IR_IS_EXPRESSION(self));

    return IR_EXPRESSION_GET_CLASS(self)->do_get_data_type(self);
}

