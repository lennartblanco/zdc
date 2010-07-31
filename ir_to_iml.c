#include "ir_to_iml.h"
#include "iml_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Add operations to provided function to evaluate the expression.
 *
 * @return the variable or constant where the result of expression will be 
 *         stored
 */
void *
iml_add_expression_eval(IrFunctionDef *function,
                        IrExpression *ir_expression)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(ir_expression));

    /* not implemented */
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
    iml_variable_t *iml_var = iml_variable_new(iml_datatype);

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

printf("init_exp %p ", init_exp);
ir_node_print(init_exp, stdout, 2);
printf("\n");
    iml_add_expression_eval(parent_function, init_exp);
}

