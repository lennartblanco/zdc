#ifndef IR_UNARY_OPERATION_INC_X
#define IR_UNARY_OPERATION_INC_X

#include "ast_unary_operation.h"
#include "ir_expression.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define IR_TYPE_UNARY_OPERATION ir_unary_operation_get_type()

#define IR_UNARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), IR_TYPE_UNARY_OPERATION, IrUnaryOperation))

#define IR_UNARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), IR_TYPE_UNARY_OPERATION, IrUnaryOperationClass))

#define IR_IS_UNARY_OPERATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), IR_TYPE_UNARY_OPERATION))

#define IR_IS_UNARY_OPERATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), IR_TYPE_UNARY_OPERATION))

#define IR_UNARY_OPERATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), IR_TYPE_UNARY_OPERATION, IrUnaryOperationClass))

typedef struct
{
    IrExpression parent;

    /* private */
    ast_unary_op_type_t  operation;
    IrExpression        *operand;
} IrUnaryOperation;

typedef struct
{
    IrExpressionClass parent_class;
} IrUnaryOperationClass;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_unary_operation_get_type(void);

IrUnaryOperation *
ir_unary_operation_new(ast_unary_op_type_t operation,
                       IrExpression *operand);

ast_unary_op_type_t
ir_unary_operation_get_operation(IrUnaryOperation *self);

IrExpression *
ir_unary_operation_get_operand(IrUnaryOperation *self);

void
ir_unary_operation_set_operand(IrUnaryOperation *self, IrExpression *operand);

#endif /* IR_UNARY_OPERATION_INC_X */
