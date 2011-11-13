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
                       IrExpression *operand,
                       guint line_number)
{
    IrUnaryOperation *obj;

    obj = g_object_new(IR_TYPE_UNARY_OPERATION,
                       "ir-node-line-number", line_number,
                       NULL);
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

static bool
ir_unary_operation_has_effect(IrExpression *self)
{
    assert(IR_IS_UNARY_OPERATION(self));

    bool has_effect;

    switch (ir_unary_operation_get_operation(IR_UNARY_OPERATION(self)))
    {
        case ast_arithm_neg_op:     /*  -(exp)  */
        case ast_bool_neg_op:       /*  !(exp)  */
            has_effect = false;
            break;
        case ast_pre_inc_op:        /*  ++(exp) */
        case ast_pre_dec_op:        /*  --(exp) */
        case ast_post_inc_op:       /* (exp)++ */
        case ast_post_dec_op:       /* (exp)-- */
            has_effect = true;
            break;
        default:
            /* unexpected operation type */
            assert(false);
    }

    return has_effect;
}

static void
ir_unary_operation_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_unary_operation_do_get_data_type;

    IR_EXPRESSION_CLASS(klass)->has_effect =
            ir_unary_operation_has_effect;
}

static DtDataType *
ir_unary_operation_do_get_data_type(IrExpression *self)
{
    IrUnaryOperation *op = IR_UNARY_OPERATION(self);
    IrExpression *operand;

    operand = ir_unary_operation_get_operand(op);

    return ir_expression_get_data_type(operand);
}
