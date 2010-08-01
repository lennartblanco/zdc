#include "ir_to_iml.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_bool_constant.h"
#include "ir_char_constant.h"
#include "ir_scalar.h"
#include "iml_constant.h"
#include "iml_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

static ImlConstant *
ir_constant_to_iml(IrConstant *constant)
{
    iml_data_type_t type;
    guint8 v8;
    guint16 v16 = 0;
    guint32 v32;

    if (IR_IS_INT_CONSTANT(constant))
    {
        type = iml_32b;
        v32 = ir_int_constant_get_value(IR_INT_CONSTANT(constant));
    }
    else if (IR_IS_UINT_CONSTANT(constant))
    {
        type = iml_32b;
        v32 = ir_uint_constant_get_value(IR_UINT_CONSTANT(constant));
    }
    else if (IR_IS_BOOL_CONSTANT(constant))
    {
        type = iml_8b;
        v8 = ir_bool_constant_get_value(IR_BOOL_CONSTANT(constant));
    }
    else if (IR_IS_CHAR_CONSTANT(constant))
    {
        type = iml_8b;
        v8 = ir_char_constant_get_value(IR_CHAR_CONSTANT(constant));
    }
    else
    {
        /* unexpected constant type */
        assert(false);
    }

    switch (type)
    {
        case iml_8b:
            return iml_constant_new_8b(v8);
        case iml_16b:
            return iml_constant_new_16b(v16);
        case iml_32b:
            return iml_constant_new_32b(v32);
        default:
            /* unexpected type */
            assert(false);
    }
}

/**
 * Add operations to provided function to evaluate the expression.
 *
 * @return the variable or constant where the result of expression will be 
 *         stored
 */
ImlOperand *
iml_add_expression_eval(IrFunctionDef *function,
                        IrExpression *ir_expression)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(ir_expression));

    if (IR_IS_CONSTANT(ir_expression))
    {
        return
            IML_OPERAND(ir_constant_to_iml(IR_CONSTANT(ir_expression)));
    }
    else if (IR_IS_SCALAR(ir_expression))
    {
        return
            IML_OPERAND(
                ir_variable_get_location(
                    ir_scalar_get_variable(IR_SCALAR(ir_expression))));
    }

    /* unexpected expression type */
    assert(false);
}

void
add_to_func_frame(IrFunctionDef *parent_function,
                  IrVariable *variable,
                  bool is_function_parameter)
{
    assert(IR_IS_FUNCTION_DEF(parent_function));
    assert(IR_IS_VARIABLE(variable));

    iml_data_type_t iml_datatype;
    DtDataType *ir_datatype;

    /* create IML variable of appropriate type */
    ir_datatype = ir_variable_get_data_type(variable);
    switch (dt_data_type_get_size(ir_datatype))
    {
      case 4:
        iml_datatype = iml_32b;
        break;
      case 2:
        iml_datatype = iml_16b;
        break;
      case 1:
        iml_datatype = iml_8b;
        break;
      default:
        /* unexpected storage size */
        assert(FALSE);
    }
    ImlVariable *iml_var = iml_variable_new(iml_datatype);

    /* add IML variable to function frame */
    iml_func_frame_t *frame = ir_function_def_get_frame(parent_function);
    if (is_function_parameter) {
        iml_func_frame_add_parameter(frame, iml_var);
    } else {
        iml_func_frame_add_local(frame, iml_var);
    }
    ir_variable_set_location(variable, iml_var);

    /* if this is a function parameter, then we are done here */
    if (is_function_parameter) {
        return;
    }

    /* */
    IrExpression *init_exp = ir_variable_get_initializer(variable);

    if (init_exp == NULL)
    {
        init_exp = dt_data_type_get_init(ir_variable_get_data_type(variable));
    }

    ImlOperand *init_val = iml_add_expression_eval(parent_function, init_exp);
    ir_function_add_operation(parent_function,
                              iml_operation_new(iml_copy, init_val, iml_var));

}

