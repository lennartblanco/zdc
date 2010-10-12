#include "ir_to_iml.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_bool_constant.h"
#include "ir_char_constant.h"
#include "ir_scalar.h"
#include "ir_binary_operation.h"
#include "ir_cast.h"
#include "iml_constant.h"
#include "iml_variable.h"
#include "types.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static ImlConstant *
ir_constant_to_iml(IrConstant *constant);

static ImlOperand *
iml_add_binary_op_eval(IrFunctionDef *function, IrBinaryOperation *bin_op);

static ImlOperand *
iml_add_cast_eval(IrFunctionDef *function, IrCast *cast);


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

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
    else if (IR_IS_BINARY_OPERATION(ir_expression))
    {
        return
            iml_add_binary_op_eval(function, IR_BINARY_OPERATION(ir_expression));
    }
    else if (IR_IS_FUNCTION_CALL(ir_expression))
    {
        return iml_add_func_call_eval(function,
                                      IR_FUNCTION_CALL(ir_expression));
    }
    else if (IR_IS_CAST(ir_expression))
    {
        return iml_add_cast_eval(function,
                                 IR_CAST(ir_expression));
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
    ImlVariable *iml_var = iml_variable_new(iml_datatype,
                                            ir_variable_get_name(variable));

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

ImlOperand *
iml_add_func_call_eval(IrFunctionDef *function, IrFunctionCall *func_call)
{
    GSList *iml_args = NULL;
    GSList *ir_args;
    GSList *i;
    ImlVariable *res_var = NULL;
    iml_func_frame_t *frame;
    iml_operation_t *call_op;
    IrFunction *callee;
    IrCodeBlock *body;
    iml_opcode_t opcode;

    switch (ir_function_call_get_linkage(func_call))
    {
        case ir_d_linkage:
            opcode = iml_call;
            break;
        case ir_c_linkage:
            opcode = iml_call_c;
            break;
        default:
            /* unexpected linkage */
            assert(false);
    }

    frame = ir_function_def_get_frame(function);

    /* generate iml code for eveluation of call parameters */
    ir_args = ir_function_call_get_arguments(func_call);
    for (i = ir_args; i != NULL; i = g_slist_next(i))
    {
        iml_args = g_slist_prepend(iml_args,
                                   iml_add_expression_eval(function,
                                                           i->data));
    }
    iml_args = g_slist_reverse(iml_args);

    body = ir_function_def_get_body(function);
    callee =
        IR_FUNCTION(
             sym_table_get_symbol(ir_code_block_get_symbols(body),
                                  ir_function_call_get_name(func_call), NULL));


    if (!types_is_void(ir_expression_get_data_type(IR_EXPRESSION(func_call))))
    {
        res_var = iml_func_frame_get_temp(frame, iml_32b);
    }

    call_op = iml_operation_new(opcode,
                                ir_function_get_mangled_name(callee),
                                iml_args,
                                res_var);
    ir_function_add_operation(function, call_op);

    return IML_OPERAND(res_var);

}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static iml_data_type_t
dt_to_iml_type(DtDataType *dt_type)
{
    iml_data_type_t iml_type;

    switch (dt_data_type_get_size(dt_type))
    {
        case 4:
            iml_type = iml_32b;
            break;
        case 2:
            iml_type = iml_16b;
            break;
        case 1:
            iml_type = iml_8b;
            break;
        default:
            assert(false);
    }

    return iml_type;
}

/**
 * Map IR binary operation to IML opcode
 */
static iml_opcode_t
get_iml_opcode_binop(IrBinaryOperation *op)
{
    iml_opcode_t opcode;
    DtBasicType *operands_type;

    operands_type =
        DT_BASIC_TYPE(
                ir_expression_get_data_type(ir_binary_operation_get_left(op)));

    /* right operand must be of same data type */
    assert(
        dt_data_type_is_same(DT_DATA_TYPE(operands_type),
            ir_expression_get_data_type(ir_binary_operation_get_right(op))));

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_plus_op:
            opcode = iml_add;
            break;
        case ast_minus_op:
            opcode = iml_sub;
            break;
        case ast_mult_op:
            opcode =
                dt_basic_type_is_signed(operands_type) ? iml_smult :
                                                         iml_umult;
            break;
        case ast_equal_op:
            opcode = iml_equal;
            break;
        case ast_not_equal_op:
            opcode = iml_nequal;
            break;
        case ast_less_op:
            opcode =
                dt_basic_type_is_signed(operands_type) ? iml_sless : iml_uless;
            break;
        case ast_greater_op:
            opcode =
                dt_basic_type_is_signed(operands_type) ? iml_sgreater :
                                                         iml_ugreater;
            break;
        case ast_less_or_eq_op:
            opcode =
                dt_basic_type_is_signed(operands_type) ? iml_slesseq :
                                                         iml_ulesseq;
            break;
        case ast_greater_or_eq_op:
            opcode =
                dt_basic_type_is_signed(operands_type) ? iml_sgreatereq :
                                                         iml_ugreatereq;
            break;
        default:
            /* unexpected binary operation type */
            assert(false);
    }


    return opcode;
}

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

static ImlOperand *
iml_add_binary_op_eval(IrFunctionDef *function, IrBinaryOperation *bin_op)
{
    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *left;
    ImlOperand *right;
    ImlVariable *res;
    iml_opcode_t opcode;
    iml_data_type_t iml_type;

    left = iml_add_expression_eval(function,
                                   ir_binary_operation_get_left(bin_op));
    right = iml_add_expression_eval(function,
                                    ir_binary_operation_get_right(bin_op));

    /* derive iml data type for temp variable */
    iml_type =
        dt_to_iml_type(ir_expression_get_data_type(IR_EXPRESSION(bin_op)));

    res = iml_func_frame_get_temp(frame, iml_type);

    opcode = get_iml_opcode_binop(bin_op);

    ir_function_add_operation(function,
                              iml_operation_new(opcode, left, right, res));

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_cast_eval(IrFunctionDef *function, IrCast *cast)
{
    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    iml_data_type_t iml_type;
    ImlOperand *src;
    ImlVariable *dest;

    src = iml_add_expression_eval(function, ir_cast_get_value(cast));

    iml_type = dt_to_iml_type(ir_cast_get_target_type(cast));
    dest = iml_func_frame_get_temp(frame, iml_type);

    ir_function_add_operation(function,
                              iml_operation_new(iml_cast, src, dest));

    return IML_OPERAND(dest);
}
