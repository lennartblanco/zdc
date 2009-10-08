#include "ir_unary_operation.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_unary_operation_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_unary_operation_do_get_data_type(IrExpression *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_unary_operation_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrUnaryOperationClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_unary_operation_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrUnaryOperation),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrUnaryOperationType",
                                    &info, 0);
    }
    return type;
}

IrUnaryOperation *
ir_unary_operation_new(ast_unary_op_type_t operation,
                       IrExpression *operand)
{
    IrUnaryOperation *obj;

    obj = g_object_new(IR_TYPE_UNARY_OPERATION, NULL);
    obj->operation = operation;
    obj->operand = operand;

    return obj;
}

ast_unary_op_type_t
ir_unary_operation_get_operation(IrUnaryOperation *self)
{
    assert(IR_IS_UNARY_OPERATION(self));

    return self->operation;
}

IrExpression *
ir_unary_operation_get_operand(IrUnaryOperation *self)
{
    assert(IR_IS_UNARY_OPERATION(self));

    return self->operand;
}

void
ir_unary_operation_set_operand(IrUnaryOperation *self,
                               IrExpression *operand)
{
    assert(IR_IS_UNARY_OPERATION(self));

    self->operand = operand;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_unary_operation_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_unary_operation_do_get_data_type;
}

static DtDataType *
ir_unary_operation_do_get_data_type(IrExpression *self)
{
    IrUnaryOperation *op = IR_UNARY_OPERATION(self);
    IrExpression *operand;

    operand = ir_unary_operation_get_operand(op);
  
    return ir_expression_get_data_type(operand);
}
