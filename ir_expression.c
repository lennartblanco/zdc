#include <stdbool.h>

#include "ir_expression.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_expression_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_expression_do_get_data_type(IrExpression *self);

bool
ir_expression_do_is_constant(IrExpression *self);

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
        ir_expression_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrExpression),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_STATMENT,
                                    "IrExpressionType",
                                    &info, 0);
    }
    return type;
}

DtDataType *
ir_expression_get_data_type(IrExpression *self)
{
    assert(IR_IS_EXPRESSION(self));

    return IR_EXPRESSION_GET_CLASS(self)->do_get_data_type(self);
}

bool
ir_expression_is_constant(IrExpression *self)
{
    assert(IR_IS_EXPRESSION(self));

    return IR_EXPRESSION_GET_CLASS(self)->do_is_constant(self);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_expression_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_expression_do_get_data_type;

    ((IrExpressionClass *)klass)->do_is_constant =
        ir_expression_do_is_constant;
}

static DtDataType *
ir_expression_do_get_data_type(IrExpression *self)
{
    /* no implementation for get_data_type() method provided */
    assert(false);
}

/**
 * Default implementation of 'is_constant()' method. By default
 * expression is not constant.
 */
bool
ir_expression_do_is_constant(IrExpression *self)
{
    return false;
}
