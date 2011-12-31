#include "ir_binary_operation.h"
#include "types.h"
#include "dt_void.h"
#include "dt_pointer.h"
#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_binary_operation_class_init(gpointer klass, gpointer dummy);

static DtDataType *
ir_binary_operation_do_get_data_type(IrExpression *self);

static DtDataType *
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
ir_binary_operation_new(binary_op_type_t operation,
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

binary_op_type_t
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

    binary_op_type_t op_type = ir_binary_operation_get_operation(self);

    return 
        op_type == op_plus     ||
        op_type == op_minus    ||
        op_type == op_mult     ||
        op_type == op_division ||
        op_type == op_modulo;
}

bool
ir_binary_operation_is_icomp(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    DtDataType *data_type;
    basic_data_type_t bdt;

    data_type = ir_expression_get_data_type(ir_expression(self));
    if (!dt_is_basic(data_type))
    {
        return false;
    }

    bdt = dt_basic_get_data_type(dt_basic(data_type));

    if (bdt != int_type && bdt != bool_type)
    {
        return false;
    }

    binary_op_type_t op_type = ir_binary_operation_get_operation(self);

    return op_type == op_equal      ||
           op_type == op_not_equal  ||
           op_type == op_less       ||
           op_type == op_greater    ||
           op_type == op_less_or_eq ||
           op_type == op_greater_or_eq;
}

bool
ir_binary_operation_is_conditional(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));

    binary_op_type_t op_type = ir_binary_operation_get_operation(self);

    return op_type == op_and || op_type == op_or;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_binary_operation_class_init(gpointer klass, gpointer dummy)
{
    IR_EXPRESSION_CLASS(klass)->do_get_data_type =
        ir_binary_operation_do_get_data_type;
}

static DtDataType *
get_substaction_data_type(IrBinaryOperation *self)
{
    assert(IR_IS_BINARY_OPERATION(self));
    assert(self->operation == op_minus);

    DtDataType *left_type = ir_expression_get_data_type(self->left);

    if (!DT_IS_POINTER(left_type))
    {
        return left_type;
    }

    /* this is pointer arithmetic subtraction */
    if (DT_IS_POINTER(ir_expression_get_data_type(self->right)))
    {
        /*
         * left and right operand are pointers
         * thus the resulting up is int
         */
        return types_get_int_type();
    }

    /* pointer and integer subtraction */
    return left_type;
}

static DtDataType *
ir_binary_operation_do_get_data_type(IrExpression *self)
{
    DtDataType *data_type = NULL;
    IrBinaryOperation *bin_op = IR_BINARY_OPERATION(self);
  

    switch (bin_op->operation) 
    {
        case op_plus:
        case op_mult:
        case op_division:
        case op_modulo:
            data_type = ir_expression_get_data_type(bin_op->left);
            break;
        case op_minus:
            data_type = get_substaction_data_type(bin_op);
            break;
        case op_less:
        case op_greater:
        case op_less_or_eq:
        case op_greater_or_eq:
        case op_equal:
        case op_not_equal:
            data_type = types_get_bool_type();
            break;
        case op_and:
        case op_or:
            data_type = ir_binary_operation_get_conditional_op_type(bin_op);
            break;
        default:
            /* unexpected binary operation type */
            g_assert_not_reached();
    }

    return data_type;
}

static DtDataType *
ir_binary_operation_get_conditional_op_type(IrBinaryOperation *self)
{
    if (DT_IS_VOID(ir_expression_get_data_type(self->right)))
    {
        return types_get_void_type();
    }

    return types_get_bool_type();
}
