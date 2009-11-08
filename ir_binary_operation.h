#ifndef IR_BINARY_OPERATION_INC_X
#define IR_BINARY_OPERATION_INC_X

#include <stdbool.h>

#include "ast_binary_operation.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_BINARY_OPERATION ir_binary_operation_get_type()

#define IR_BINARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_BINARY_OPERATION, IrBinaryOperation))

#define IR_BINARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_BINARY_OPERATION, IrBinaryOperationClass))

#define IR_IS_BINARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_BINARY_OPERATION))

#define IR_IS_BINARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_BINARY_OPERATION))

#define IR_BINARY_OPERATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_BINARY_OPERATION, IrBinaryOperationClass))

typedef struct
{
    IrExpression parent;

    /* private */
    ast_binary_op_type_t  operation;
    IrExpression         *left;
    IrExpression         *right;
} IrBinaryOperation;

typedef struct
{
    IrExpressionClass parent_class;
} IrBinaryOperationClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType
ir_binary_operation_get_type(void);

IrBinaryOperation *
ir_binary_operation_new(ast_binary_op_type_t operation,
                        IrExpression *left,
                        IrExpression *right,
                        guint line_number);

ast_binary_op_type_t
ir_binary_operation_get_operation(IrBinaryOperation *self);

IrExpression *
ir_binary_operation_get_left(IrBinaryOperation *self);

IrExpression *
ir_binary_operation_get_right(IrBinaryOperation *self);

void
ir_binary_operation_set_left(IrBinaryOperation *self,
                             IrExpression *left);

void
ir_binary_operation_set_right(IrBinaryOperation *self,
                             IrExpression *right);

/**
 * Returns true if this is an integer arithmetic binary operation.
 * Integer arithmetic operations +, -, *, / over integer expressions.
 */
bool
ir_binary_operation_is_iarithm(IrBinaryOperation *self);

/**
 * Return true of this is an integer comparison operation.
 * Integer comparison are == and != between to integers.
 */
bool
ir_binary_operation_is_icomp(IrBinaryOperation *self);

/**
 * Return true of this is an conditional operation.
 * Conditional operation are && and ||.
 */
bool
ir_binary_operation_is_conditional(IrBinaryOperation *self);

#endif /* IR_BINARY_OPERATION_INC_X */
