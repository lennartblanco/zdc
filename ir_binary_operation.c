#include "ir_binary_operation.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_binary_operation_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_binary_operation_do_get_data_type(IrExpression *self);

DtDataType *
ir_binary_operation_get_conditional_op_type(IrBinaryOperation *self);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

GType 
ir_binary_operation_get_type(void)
{
    static GType type = 0;
    if (type == 0) 
    {
      static const GTypeInfo info = 
      {
        sizeof (IrBinaryOperationClass),
        NULL,   /* base_init */
        NULL,   /* base_finalize */
        ir_binary_operation_class_init, /* class_init */
        NULL,   /* class_finalize */
        NULL,   /* class_data */
        sizeof (IrBinaryOperation),
        0,      /* n_preallocs */
        NULL    /* instance_init */
      };
      type = g_type_register_static(IR_TYPE_EXPRESSION,
                                    "IrBinaryOperationType",
                                    &info, 0);
    }
    return type;
}

IrBinaryOperation *
ir_binary_operation_new(ast_binary_op_type_t operation,
                        IrExpression *left,
                        IrExpression *right,
                        guint line_number)
{
    IrBinaryOperation *obj;

    obj = g_object_new(IR_TYPE_BINARY_OPERATION,
                       "ir-node-line-number", line_number,
                       NULL);
    obj->operation = operation;
    obj->left = left;
    obj->right = right;

    return obj;
}

ast_binary_op_type_t
ir_binary_operation_get_operation(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    return self->operation;
}

IrExpression *
ir_binary_operation_get_left(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    return self->left;
}

IrExpression *
ir_binary_operation_get_right(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    return self->right;
}

void
ir_binary_operation_set_left(IrBinaryOperation *self,
                             IrExpression *left)
{
    assert(IR_IS_BINARY_OPERATION(self));

    self->left = left;
}

void
ir_binary_operation_set_right(IrBinaryOperation *self,
                             IrExpression *right)
{
    assert(IR_IS_BINARY_OPERATION(self));

    self->right = right;
}

bool
ir_binary_operation_is_arithm(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    ast_binary_op_type_t op_type;

    op_type = ir_binary_operation_get_operation(self);

    return 
        op_type == ast_plus_op  ||
        op_type == ast_minus_op ||
        op_type == ast_mult_op  ||
        op_type == ast_division_op;
}

bool
ir_binary_operation_is_icomp(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    DtDataType *data_type;
    basic_data_type_t bdt;
    ast_binary_op_type_t op_type;

    data_type = ir_expression_get_data_type(IR_EXPRESSION(self));
    if (!DT_IS_BASIC_TYPE(data_type))
    {
        return false;
    }

    bdt = dt_basic_type_get_data_type(DT_BASIC_TYPE(data_type));

    if (bdt != int_type && bdt != bool_type)
    {
        return false;
    }

    op_type = ir_binary_operation_get_operation(self);

    return op_type == ast_equal_op      || 
           op_type == ast_not_equal_op  ||
           op_type == ast_less_op       ||
           op_type == ast_greater_op    ||
           op_type == ast_less_or_eq_op ||
           op_type == ast_greater_or_eq_op;
}

bool
ir_binary_operation_is_conditional(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    ast_binary_op_type_t op_type;

    op_type = ir_binary_operation_get_operation(self);

    return op_type == ast_and_op || op_type == ast_or_op;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_binary_operation_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_binary_operation_do_get_data_type;
}

static DtDataType *
ir_binary_operation_do_get_data_type(IrExpression *self)
{
    DtDataType *data_type = NULL;
    IrBinaryOperation *bin_op = IR_BINARY_OPERATION(self);
  

    switch (bin_op->operation) 
    {
        case ast_plus_op:
        case ast_minus_op:
        case ast_mult_op:
        case ast_division_op:
            data_type = ir_expression_get_data_type(bin_op->left);
            break;
        case ast_less_op:
        case ast_greater_op:
        case ast_less_or_eq_op:
        case ast_greater_or_eq_op:
        case ast_equal_op:
        case ast_not_equal_op:
            data_type = types_get_bool_type();
            break;
        case ast_and_op:
        case ast_or_op:
            data_type = ir_binary_operation_get_conditional_op_type(bin_op);
            break;
        default:
            /* unexpected binary operation type */
            g_assert_not_reached();
    }

    return data_type;
}

DtDataType *
ir_binary_operation_get_conditional_op_type(IrBinaryOperation *self)
{
    if (types_is_void(ir_expression_get_data_type(self->right)))
    {
        return types_get_void_type();
    }

    return types_get_bool_type();
}
