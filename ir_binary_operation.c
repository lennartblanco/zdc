#include "ir_binary_operation.h"
#include "ast_basic_type.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
ir_binary_operation_class_init(gpointer klass, gpointer dummy);

static AstDataType *
ir_binary_operation_do_get_data_type(IrExpression *self);

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
                        IrExpression *right)
{
    IrBinaryOperation *obj;

    obj = g_object_new(IR_TYPE_BINARY_OPERATION, NULL);
    obj->operation = operation;
    obj->left = left;
    obj->right = right;

    return obj;
}

ast_binary_op_type_t
ir_binary_operation_get_operation(IrBinaryOperation *self)
{
    assert(self);
    assert(IR_IS_BINARY_OPERATION(self));

    return self->operation;
}

IrExpression *
ir_binary_operation_get_left(IrBinaryOperation *self)
{
    assert(self);
    assert(IR_IS_BINARY_OPERATION(self));

    return self->left;
}

IrExpression *
ir_binary_operation_get_right(IrBinaryOperation *self)
{
    assert(self);
    assert(IR_IS_BINARY_OPERATION(self));

    return self->right;
}

bool
ir_binary_operation_is_iarithm(IrBinaryOperation *self)
{
    AstDataType *data_type;
    basic_data_type_t bdt;
    ast_binary_op_type_t op_type;

    assert(self);
    assert(IR_IS_BINARY_OPERATION(self));

    data_type = ir_expression_get_data_type(IR_EXPRESSION(self));
    if (!XDP_IS_AST_BASIC_TYPE(data_type))
    {
        return false;
    }

    bdt = ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(data_type));

    if (bdt != int_type && bdt != bool_type)
    {
        return false;
    }

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
    AstDataType *data_type;
    basic_data_type_t bdt;
    ast_binary_op_type_t op_type;

    assert(self);
    assert(IR_IS_BINARY_OPERATION(self));

    data_type = ir_expression_get_data_type(IR_EXPRESSION(self));
    if (!XDP_IS_AST_BASIC_TYPE(data_type))
    {
        return false;
    }

    bdt = ast_basic_type_get_data_type(XDP_AST_BASIC_TYPE(data_type));

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

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
ir_binary_operation_class_init(gpointer klass, gpointer dummy)
{
    ((IrExpressionClass *)klass)->do_get_data_type =
        ir_binary_operation_do_get_data_type;
}

static AstDataType *
ir_binary_operation_do_get_data_type(IrExpression *self)
{
    static AstDataType *bool_data_type = NULL;
    AstDataType *data_type = NULL;
    IrBinaryOperation *bin_op = IR_BINARY_OPERATION(self);
  

    switch (bin_op->operation) 
    {
        case ast_plus_op:
        case ast_minus_op:
        case ast_mult_op:
        case ast_division_op:
            data_type = ir_expression_get_data_type(bin_op->left);
            break;
        case ast_or_op:
        case ast_and_op:
        case ast_less_op:
        case ast_greater_op:
        case ast_less_or_eq_op:
        case ast_greater_or_eq_op:
        case ast_equal_op:
        case ast_not_equal_op:
            if (bool_data_type == NULL)
            {
                bool_data_type =
                    XDP_AST_DATA_TYPE(ast_basic_type_new(bool_type));
            }
            data_type = bool_data_type;
            break;
        default:
            /* unexpected binary operation type */
            g_assert_not_reached();
    }

    return data_type;
}
