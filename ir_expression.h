#ifndef IR_EXPRESSION_INC_X
#define IR_EXPRESSION_INC_X

#include <glib-object.h>
#include <stdbool.h>

#include "ir_types.h"
#include "dt_types.h"
#include "ir_statment.h"
#include "ut_range.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_EXPRESSION ir_expression_get_type()

#define IR_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_EXPRESSION, IrExpressionClass))

#define IR_IS_EXPRESSION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_EXPRESSION))

#define IR_IS_EXPRESSION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_EXPRESSION))

#define IR_EXPRESSION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_EXPRESSION, IrExpressionClass))

struct _IrExpression
{
    IrStatment parent;
};

typedef struct 
{
    IrStatmentClass parent_class;
    /* public virtual methods */
    DtDataType * (*do_get_data_type) (IrExpression *self);
    bool (*do_is_constant) (IrExpression *self);
    bool (*do_is_lvalue) (IrExpression *self);
    bool (*has_effect) (IrExpression *self);
    UtRange * (*get_value_range) (IrExpression *self);
} IrExpressionClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_expression_get_type(void);

IrExpression *
ir_expression(void *obj);

DtDataType *
ir_expression_get_data_type(IrExpression *self);

/**
 * @return true if expression value is know at compile time, false otherwise
 */
bool
ir_expression_is_constant(IrExpression *self);

bool
ir_expression_is_lvalue(IrExpression *self);

/**
 * Checks if the expression has effect. For example:
 *
 * foo++; has effect
 * foo + 2 has not effect
 *
 * Expressions that have effect can be used as stand alone statements.
  */
bool
ir_expression_has_effect(IrExpression *self);

UtRange *
ir_expression_get_value_range(IrExpression *self);

#endif /* IR_EXPRESSION_INC_X */
