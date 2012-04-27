#include <stdbool.h>

#include "ir_expression.h"
#include "dt_basic.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_expression_class_init(gpointer klass, gpointer dummy);

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

IrExpression *
ir_expression(void *obj)
{
    return G_TYPE_CHECK_INSTANCE_CAST((obj), IR_TYPE_EXPRESSION, IrExpression);
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

bool
ir_expression_is_lvalue(IrExpression *self)
{
    assert(IR_IS_EXPRESSION(self));

    return IR_EXPRESSION_GET_CLASS(self)->do_is_lvalue(self);
}

bool
ir_expression_has_effect(IrExpression *self)
{
    assert(IR_IS_EXPRESSION(self));

    return IR_EXPRESSION_GET_CLASS(self)->has_effect(self);
}

UtRange *
ir_expression_get_value_range(IrExpression *self)
{
    assert(IR_IS_EXPRESSION(self));
    /* value ranges are only defined for expressions of basic type */
    assert(dt_is_basic(ir_expression_get_data_type(self)));
    assert(IR_EXPRESSION_GET_CLASS(self)->get_value_range);

    return IR_EXPRESSION_GET_CLASS(self)->get_value_range(self);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

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
static bool
ir_expression_do_is_constant(IrExpression *self)
{
    return false;
}

/**
 * Default implementation of 'is_lvalue()' method. By default
 * expression is not a lvalue.
 */
static bool
ir_expression_do_is_lvalue(IrExpression *self)
{
    return false;
}

/**
 * Default implementation of 'has_effect()' method. By default
 * expression has no effect.
 */
static bool
ir_expression_do_has_effect(IrExpression *self)
{
    return false;
}

static void
ir_expression_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_expression_do_get_data_type;

    IR_EXPRESSION_CLASS(klass)->do_is_constant =
        ir_expression_do_is_constant;

    IR_EXPRESSION_CLASS(klass)->do_is_lvalue =
        ir_expression_do_is_lvalue;

    IR_EXPRESSION_CLASS(klass)->has_effect =
        ir_expression_do_has_effect;
}
