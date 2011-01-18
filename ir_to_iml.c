#include "ir_to_iml.h"
#include "ir_int_constant.h"
#include "ir_uint_constant.h"
#include "ir_bool_constant.h"
#include "ir_char_constant.h"
#include "ir_array_cell.h"
#include "ir_array_slice.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_ptr_dref.h"
#include "ir_property.h"
#include "ir_cast.h"
#include "ir_null.h"
#include "iml_constant.h"
#include "iml_variable.h"
#include "types.h"
#include "dt_pointer.h"
#include "dt_static_array.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static ImlConstant *
ir_constant_to_iml(IrConstant *constant);

static ImlOperand *
iml_add_unary_op_eval(IrFunctionDef *function,
                      IrUnaryOperation *op,
                      ImlVariable *res);

static ImlOperand *
iml_add_binary_op_eval(IrFunctionDef *function,
                       IrBinaryOperation *bin_op,
                       ImlVariable *res);

static ImlOperand *
iml_add_cast_eval(IrFunctionDef *function, IrCast *cast, ImlVariable *dest);

static ImlOperand *
iml_add_array_cell_eval(IrFunctionDef *function,
                        IrArrayCell *cell,
                        ImlVariable *res);

static ImlOperand *
iml_add_array_slice_eval(IrFunctionDef *function,
                         IrArraySlice *slice,
                         ImlVariable *res);

static ImlOperand *
iml_add_ptr_dref_eval(IrFunctionDef *function,
                      IrPtrDref *ptr_dref,
                      ImlVariable *res);

static ImlOperand *
iml_add_property_eval(IrFunctionDef *function,
                      IrProperty *prop,
                      ImlVariable *res);

static void
add_array_cell_assignment(IrFunctionDef *function,
                          IrArrayCell *lvalue,
                          IrExpression *value);

static void
add_array_slice_assignment(IrFunctionDef *function,
                           IrArraySlice *lvalue,
                           IrExpression *value);

static void
add_array_assignment(IrFunctionDef *function,
                     IrVariable *lvalue,
                     IrExpression *value);

static void
add_static_array_assignment(IrFunctionDef *function,
                            IrVariable *lvalue,
                            IrExpression *value);

static ImlOperand *
iml_add_array_literal_eval(IrFunctionDef *function,
                           IrArrayLiteral *expr,
                           ImlVariable *res);

static void
add_pointer_assignment(IrFunctionDef *function,
                       IrVariable *lvalue,
                       IrExpression *value);

static void
add_ptr_dref_assignment(IrFunctionDef *function,
                        IrPtrDref *lvalue,
                        IrExpression *value);

static iml_data_type_t
dt_to_iml_type(DtDataType *dt_type);

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
                        IrExpression *ir_expression,
                        ImlVariable *dest)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(ir_expression));

    ImlOperand *res = NULL;

    if (IR_IS_CONSTANT(ir_expression))
    {
        res = IML_OPERAND(ir_constant_to_iml(IR_CONSTANT(ir_expression)));
    }
    else if (IR_IS_VARIABLE(ir_expression))
    {
        res =
            IML_OPERAND(ir_variable_get_location(IR_VARIABLE(ir_expression)));
    }
    else if (IR_IS_UNARY_OPERATION(ir_expression))
    {
        res = iml_add_unary_op_eval(function,
                                    IR_UNARY_OPERATION(ir_expression),
                                    dest);
    }
    else if (IR_IS_BINARY_OPERATION(ir_expression))
    {
        res = iml_add_binary_op_eval(function,
                                     IR_BINARY_OPERATION(ir_expression),
                                     dest);
    }
    else if (IR_IS_FUNCTION_CALL(ir_expression))
    {
        res = iml_add_func_call_eval(function,
                                     IR_FUNCTION_CALL(ir_expression),
                                     dest);
    }
    else if (IR_IS_CAST(ir_expression))
    {
        res = iml_add_cast_eval(function, IR_CAST(ir_expression), dest);
    }
    else if (IR_IS_ARRAY_CELL(ir_expression))
    {
        res = iml_add_array_cell_eval(function,
                                      IR_ARRAY_CELL(ir_expression),
                                      dest);
    }
    else if (IR_IS_PROPERTY(ir_expression))
    {
        res = iml_add_property_eval(function,
                                    IR_PROPERTY(ir_expression),
                                    dest);
    }
    else if (IR_IS_ARRAY_LITERAL(ir_expression))
    {
        res = iml_add_array_literal_eval(function,
                                         IR_ARRAY_LITERAL(ir_expression),
                                         dest);
    }
    else if (IR_IS_ARRAY_SLICE(ir_expression))
    {
        res = iml_add_array_slice_eval(function,
                                       IR_ARRAY_SLICE(ir_expression),
                                       dest);
    }
    else if (IR_IS_PTR_DREF(ir_expression))
    {
        res = iml_add_ptr_dref_eval(function,
                                    IR_PTR_DREF(ir_expression),
                                    dest);
    }
    else if (IR_IS_ENUM_MEMBER(ir_expression))
    {
        IrExpression *val;

        val = ir_enum_member_get_value(IR_ENUM_MEMBER(ir_expression));
        res = iml_add_expression_eval(function, val, dest);
    }
    else
    {
        /* unexpected expression type */
        assert(false);
    }

    /*
     * if a destination variable was specified,
     * and iml operation result is not already placed there
     * add a copy operation
     */
    if (dest != NULL && res != IML_OPERAND(dest))
    {
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_copy, res, dest));
        res = IML_OPERAND(dest);
    }

    return res;
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
    char *var_name;

    /* create IML variable of appropriate type */
    ir_datatype = ir_variable_get_data_type(variable);
    iml_datatype = dt_to_iml_type(ir_datatype);

    var_name = ir_variable_get_name(variable);
    if (var_name == NULL)
    {
        var_name = "(anon)";
    }
    ImlVariable *iml_var;

    if (iml_datatype == iml_blob)
    {
        iml_var = iml_variable_blob_new(dt_data_type_get_size(ir_datatype),
                                        var_name);
    }
    else
    {
        iml_var = iml_variable_new(iml_datatype, var_name);
    }

    /* add IML variable to function frame */
    iml_func_frame_t *frame = ir_function_def_get_frame(parent_function);
    if (is_function_parameter) {
        iml_func_frame_add_parameter(frame, iml_var);
    } else {
        iml_func_frame_add_local(frame, iml_var);
    }
    ir_variable_set_location(variable, iml_var);
}

ImlOperand *
iml_add_func_call_eval(IrFunctionDef *function,
                       IrFunctionCall *func_call,
                       ImlVariable *res)
{
    GSList *iml_args = NULL;
    GSList *ir_args;
    GSList *i;
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
                                                           i->data,
                                                           NULL));
    }
    iml_args = g_slist_reverse(iml_args);

    body = ir_function_def_get_body(function);
    callee =
        IR_FUNCTION(
             sym_table_get_symbol(ir_code_block_get_symbols(body),
                                  ir_function_call_get_name(func_call), NULL));


    if (res == NULL &&
        !types_is_void(ir_expression_get_data_type(IR_EXPRESSION(func_call))))
    {
        res = iml_func_frame_get_temp(frame, iml_32b);
    }

    call_op = iml_operation_new(opcode,
                                ir_function_get_mangled_name(callee),
                                iml_args,
                                res);
    ir_function_def_add_operation(function, call_op);

    /* mark any variables used as function arguments as unused */
    for (i = iml_args; i != NULL; i = g_slist_next(i))
    {
        iml_func_frame_unused_oper(frame, i->data);
    }

    return IML_OPERAND(res);
}

void
iml_add_assignment(IrFunctionDef *function,
                   IrExpression *lvalue,
                   IrExpression *value)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(lvalue));
    assert(ir_expression_is_lvalue(lvalue));
    assert(IR_IS_EXPRESSION(value));

    if (IR_IS_VARIABLE(lvalue))
    {
        DtDataType *var_type;

        var_type = ir_expression_get_data_type(lvalue);

        if (DT_IS_BASIC(var_type) || DT_IS_ENUM(var_type))
        {
            iml_add_expression_eval(
                    function, value,
                    ir_variable_get_location(IR_VARIABLE(lvalue)));
        }
        else if (DT_IS_STATIC_ARRAY_TYPE(var_type))
        {
            add_static_array_assignment(function, IR_VARIABLE(lvalue), value);
        }
        else if (DT_IS_ARRAY(var_type))
        {
            add_array_assignment(function, IR_VARIABLE(lvalue), value);
        }
        else if (DT_IS_POINTER(var_type))
        {
            add_pointer_assignment(function, IR_VARIABLE(lvalue), value);
        }
        else
        {
            /* variable of unexpected data type */
            assert(false);
        }
    }
    else if (IR_IS_ARRAY_CELL(lvalue))
    {
        add_array_cell_assignment(function, IR_ARRAY_CELL(lvalue), value);
    }
    else if (IR_IS_ARRAY_SLICE(lvalue))
    {
        add_array_slice_assignment(function, IR_ARRAY_SLICE(lvalue), value);
    }
    else if (IR_IS_PTR_DREF(lvalue))
    {
        add_ptr_dref_assignment(function, IR_PTR_DREF(lvalue), value);
    }
    else
    {
        /* unexpected lvalue type */
        assert(false);
    }

}

void
iml_add_foreach_head(IrFunctionDef *function,
                     IrForeach *foreach,
                     ImlVariable **index,
                     ImlVariable **length,
                     iml_operation_t **loop_head)
{
    IrExpression *ir_aggregate;
    DtArray *aggregate_type;
    ImlOperand *aggregate;
    IrVariable *ir_index;
    IrVariable *ir_value;
    ImlVariable *value;
    ImlVariable *aggr_ptr;
    iml_func_frame_t *frame = ir_function_def_get_frame(function);

    /* set-up index variable */
    ir_index = ir_foreach_get_index(foreach);
    if (ir_index != NULL)
    {
        add_to_func_frame(function, ir_index, false);
        *index = ir_variable_get_location(ir_index);
    }
    else
    {
        *index = iml_func_frame_get_temp(frame, iml_32b);
    }

    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_copy,
                                                    iml_constant_new_32b(0),
                                                    *index));

    /* set-up value variable */
    ir_value = ir_foreach_get_value(foreach);
    assert(ir_value);
    add_to_func_frame(function, ir_value, false);
    value = ir_variable_get_location(ir_value);

    /* generate iml to evaluate aggregate expression */
    ir_aggregate = ir_foreach_get_aggregate(foreach);
    aggregate_type = DT_ARRAY(ir_expression_get_data_type(ir_aggregate));
    aggregate = iml_add_expression_eval(function, ir_aggregate, NULL);

    /* store length of the aggregate array in a temp variable */
    *length = iml_func_frame_get_temp(frame, iml_32b);
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    aggregate,
                                                    iml_constant_new_32b(0),
                                                    4,
                                                    *length));

    /* store pointer to the start of aggregate array in a temp variable */
    aggr_ptr = iml_func_frame_get_temp(frame, iml_ptr);
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    aggregate,
                                                    iml_constant_new_32b(1),
                                                    4,
                                                    aggr_ptr));

    /* insert loop label */
    *loop_head =
        iml_operation_new(iml_label,
                          ir_module_gen_label(
                              ir_symbol_get_parent_module(
                                  IR_SYMBOL(function))));
    ir_function_def_add_operation(function, *loop_head);

    /* generate iml for assigning value variable with aggregates element */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_getelm,
                          aggr_ptr,
                          *index,
                          dt_array_get_element_size(aggregate_type),
                          value));
}

void
iml_add_foreach_tail(IrFunctionDef *function,
                     ImlVariable *index,
                     ImlVariable *length,
                     iml_operation_t *loop_label)
{
    /* generate iml to advance index */
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_add,
                                                    index,
                                                    iml_constant_new_32b(1),
                                                    index));

    /* generate iml to jump to loop head if index is less then length */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_jmpuless,
                          index,
                          length,
                          iml_operation_get_operand(loop_label, 1)));
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static iml_data_type_t
dt_to_iml_type(DtDataType *dt_type)
{
    iml_data_type_t iml_type;

    if (DT_IS_BASIC(dt_type))
    {
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
    }
    else if (DT_IS_POINTER(dt_type))
    {
        iml_type = iml_ptr;
    }
    else if (DT_IS_ARRAY(dt_type))
    {
        iml_type = iml_blob;
    }
    else if (DT_IS_ENUM(dt_type))
    {
        return dt_to_iml_type(
                  dt_enum_get_base_type(DT_ENUM(dt_type)));
    }
    else
    {
        /* unexpected data type */
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
    DtBasic *operands_type;

    operands_type =
       DT_BASIC(ir_expression_get_data_type(ir_binary_operation_get_left(op)));

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
                dt_basic_is_signed(operands_type) ? iml_smult : iml_umult;
            break;
        case ast_division_op:
            opcode = dt_basic_is_signed(operands_type) ? iml_sdiv : iml_udiv;
            break;
        case ast_modulo_op:
            opcode = dt_basic_is_signed(operands_type) ? iml_smod : iml_umod;
            break;
        case ast_and_op:
            opcode = iml_and;
            break;
        case ast_or_op:
            opcode = iml_or;
            break;
        case ast_equal_op:
            opcode = iml_equal;
            break;
        case ast_not_equal_op:
            opcode = iml_nequal;
            break;
        case ast_less_op:
            opcode =
                dt_basic_is_signed(operands_type) ? iml_sless : iml_uless;
            break;
        case ast_greater_op:
            opcode =
                dt_basic_is_signed(operands_type) ? iml_sgreater :
                                                    iml_ugreater;
            break;
        case ast_less_or_eq_op:
            opcode =
                dt_basic_is_signed(operands_type) ? iml_slesseq : iml_ulesseq;
            break;
        case ast_greater_or_eq_op:
            opcode =
                dt_basic_is_signed(operands_type) ? iml_sgreatereq :
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
iml_add_unary_op_eval(IrFunctionDef *function,
                      IrUnaryOperation *op,
                      ImlVariable *res)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_UNARY_OPERATION(op));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *operand;
    iml_data_type_t iml_type;
    iml_opcode_t opcode;

    /* only arithmetic negation implemented */
    switch (ir_unary_operation_get_operation(op))
    {
        case ast_arithm_neg_op:
            opcode = iml_ineg;
            break;
        case ast_bool_neg_op:
            opcode = iml_bneg;
            break;
        default:
            /* unexpected unary operation */
            assert(false);
    }

    operand = iml_add_expression_eval(function,
                                      ir_unary_operation_get_operand(op),
                                      NULL);

    /* derive iml data type for temp variable */
    iml_type =
        dt_to_iml_type(ir_expression_get_data_type(IR_EXPRESSION(op)));

    if (res == NULL)
    {
      res = iml_func_frame_get_temp(frame, iml_type);
    }

    ir_function_def_add_operation(function,
                              iml_operation_new(opcode, operand, res));
    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_binary_op_eval(IrFunctionDef *function,
                       IrBinaryOperation *bin_op,
                       ImlVariable *res)
{
    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *left;
    ImlOperand *right;
    iml_opcode_t opcode;
    iml_data_type_t iml_type;

    left = iml_add_expression_eval(function,
                                   ir_binary_operation_get_left(bin_op),
                                   NULL);
    right = iml_add_expression_eval(function,
                                    ir_binary_operation_get_right(bin_op),
                                    NULL);

    /* derive iml data type for temp variable */
    iml_type =
        dt_to_iml_type(ir_expression_get_data_type(IR_EXPRESSION(bin_op)));

    if (res == NULL)
    {
      res = iml_func_frame_get_temp(frame, iml_type);
    }

    opcode = get_iml_opcode_binop(bin_op);

    ir_function_def_add_operation(function,
                                  iml_operation_new(opcode, left, right, res));

    /* mark any temporary variables used as operands as unused */
    iml_func_frame_unused_oper(frame, left);
    iml_func_frame_unused_oper(frame, right);

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_cast_eval(IrFunctionDef *function, IrCast *cast, ImlVariable *dest)
{
    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    iml_data_type_t iml_type;
    ImlOperand *src;

    src = iml_add_expression_eval(function, ir_cast_get_value(cast), NULL);

    iml_type = dt_to_iml_type(ir_cast_get_target_type(cast));

    if (dest == NULL)
    {
        dest = iml_func_frame_get_temp(frame, iml_type);
    }

    ir_function_def_add_operation(function,
                              iml_operation_new(iml_cast, src, dest));

    return IML_OPERAND(dest);
}

static ImlOperand *
iml_add_array_cell_eval(IrFunctionDef *function,
                        IrArrayCell *cell,
                        ImlVariable *res)
{
    assert(IR_IS_ARRAY_CELL(cell));

    IrVariable *array_symb;
    ImlOperand *index_val;
    ImlVariable *src;
    DtDataType *array_type;
    DtDataType *element_type;
    guint size;

    array_symb = ir_array_cell_get_symbol(cell);
    array_type = ir_expression_get_data_type(IR_EXPRESSION(array_symb));
    element_type = ir_expression_get_data_type(IR_EXPRESSION(cell));

    /* generate code for array index evaluation */
    index_val = iml_add_expression_eval(function,
                                        ir_array_cell_get_index(cell),
                                        NULL);

    /* figure out where the array cell value should end up */
    if (res == NULL)
    {
        res = iml_func_frame_get_temp(ir_function_def_get_frame(function),
                                      dt_to_iml_type(element_type));
    }

    src = ir_variable_get_location(array_symb);
    size = dt_data_type_get_size(element_type);

    if (DT_IS_STATIC_ARRAY_TYPE(array_type))
    {
        /* static array cell */
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    src,
                                                    index_val,
                                                    size,
                                                    res));
    }
    else
    {
        /* dynamic array cell */
        assert(DT_IS_ARRAY(array_type));
        ImlVariable *ptr;
        iml_func_frame_t *frame = ir_function_def_get_frame(function);

        /* generate code to store array pointer in a temp variable */
        ptr = iml_func_frame_get_temp(frame, iml_ptr);
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    src,
                                                    iml_constant_new_32b(1),
                                                    4,
                                                    ptr));

        /*
         * generate code to fetch the array cell value into
         * the destination variable
         */
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    ptr,
                                                    index_val,
                                                    size,
                                                    res));
    }

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_ptr_dref_eval(IrFunctionDef *function,
                      IrPtrDref *ptr_dref,
                      ImlVariable *res)
{
    assert(IR_IS_PTR_DREF(ptr_dref));

    ImlOperand *ptr_exp;
    iml_func_frame_t *frame = ir_function_def_get_frame(function);

    /* add iml to evaluate pointer expression */
    ptr_exp = iml_add_expression_eval(function,
                                      ir_ptr_dref_get_expression(ptr_dref),
                                      NULL);

    /* figure out where to store the result */
    if (res == NULL)
    {
        iml_data_type_t res_type;

        res_type =
            dt_to_iml_type(
                ir_expression_get_data_type(IR_EXPRESSION(ptr_dref)));
        res = iml_func_frame_get_temp(frame, res_type);
    }

    /* add iml to fetch the value from the address to the result variable */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_getelm,
                          ptr_exp,
                          iml_constant_new_32b(0),
                          ir_ptr_dref_get_dest_size(ptr_dref),
                          res));

    /* pointer expression operand not needed any more */
    iml_func_frame_unused_oper(frame, ptr_exp);

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_property_eval(IrFunctionDef *function,
                      IrProperty *prop,
                      ImlVariable *res)
{
    assert(IR_IS_PROPERTY(prop));

    IrVariable *prop_exp;
    ImlVariable *src;
    iml_func_frame_t *frame = ir_function_def_get_frame(function);

    /* only length property of variables implemented for now */
    assert(ir_property_get_id(prop) == ir_prop_length);
    assert(IR_IS_VARIABLE(ir_property_get_expression(prop)));

    prop_exp = IR_VARIABLE(ir_property_get_expression(prop));
    /* only length of dynamic array implemented for now */
    assert(DT_IS_ARRAY(ir_variable_get_data_type(prop_exp)));

    src = ir_variable_get_location(prop_exp);

    if (res == NULL)
    {
        res = iml_func_frame_get_temp(frame, iml_32b);
    }

    ir_function_def_add_operation(function,
                              iml_operation_new(iml_getelm,
                                                src,
                                                iml_constant_new_32b(0),
                                                4,
                                                res));

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_array_literal_eval(IrFunctionDef *function,
                           IrArrayLiteral *expr,
                           ImlVariable *res)
{
    assert(IR_IS_ARRAY_LITERAL(expr));

    ImlVariable *ptr;
    ImlConstant *length;
    ImlConstant *size;
    iml_operation_t *op;
    guint element_size;
    iml_func_frame_t *frame = ir_function_def_get_frame(function);


    /*
     * generate code to allocate memory for array literal
     */
    ptr = iml_func_frame_get_temp(frame, iml_ptr);
    size = iml_constant_new_32b(ir_array_literal_get_size(expr));

    ir_function_def_add_operation(function,
                                  iml_operation_new_call_c("GC_malloc",
                                                           ptr,
                                                           size, NULL));

    if (ir_expression_is_constant(IR_EXPRESSION(expr)))
    {
        /*
         * generate code to copy literal data from data section to
         * allocated memory
         */

        const char *label = ir_array_literal_get_data_label(expr);
        assert(label != NULL); /* should be stored in data section */

        op = iml_operation_new_call_c("memcpy",
                                      NULL,
                                      ptr,
                                      iml_constant_new_ptr(label),
                                      size,
                                      NULL);
        ir_function_def_add_operation(function, op);
    }
    else
    {
        /*
         * generate code to evaluate each array literal value,
         * and store the result in the allocated memory chunk
         */
        DtDataType *dt;
        GSList *i;
        ImlVariable *temp;
        guint cntr;

        /* figure out array element size */
        dt = ir_expression_get_data_type(IR_EXPRESSION(expr));
        assert(DT_IS_ARRAY(dt));
        element_size = dt_array_get_element_size(DT_ARRAY(dt));

        temp = iml_func_frame_get_temp(frame, iml_32b);

        i = ir_array_literal_get_values(expr);
        cntr = 0;
        for (; i != NULL; i = g_slist_next(i), cntr += 1)
        {
            iml_add_expression_eval(function,
                                    i->data,
                                    temp);
            op = iml_operation_new(iml_setelm,
                                   temp,
                                   ptr,
                                   iml_constant_new_32b(cntr),
                                   element_size);
            ir_function_def_add_operation(function, op);
        }
    }

    if (res == NULL)
    {
        res = iml_func_frame_get_temp(frame, iml_blob, 8);
    }

    /*
     * store length and pointer address in array handle blob
     */
    length = iml_constant_new_32b(ir_array_literal_get_length(expr));
    op = iml_operation_new(iml_setelm,
                           length,
                           res,
                           iml_constant_new_32b(0),
                           4);
    ir_function_def_add_operation(function, op);

    op = iml_operation_new(iml_setelm,
                           ptr,
                           res,
                           iml_constant_new_32b(1),
                           4);
    ir_function_def_add_operation(function, op);

    /* temp pointer is no longer used */
    iml_func_frame_unused_oper(frame, IML_OPERAND(ptr));


    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_array_slice_eval(IrFunctionDef *function,
                         IrArraySlice *slice,
                         ImlVariable *res)
{
    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *array;
    ImlOperand *start;
    ImlOperand *end;
    ImlOperand *length;
    ImlVariable *start_ptr;
    DtDataType *array_type;
    guint element_size;

    if (res == NULL)
    {
        res = iml_func_frame_get_temp(frame, iml_blob, 8);
    }

    /* figure out the element size of sliced array */
    array_type = ir_expression_get_data_type(ir_array_slice_get_array(slice));
    assert(DT_IS_ARRAY(array_type));
    element_size = dt_array_get_element_size(DT_ARRAY(array_type));

    /*
     * generate code to evaluate array expression,
     * slice start and end expressions
     */
    array = iml_add_expression_eval(function,
                                    ir_array_slice_get_array(slice),
                                    NULL);

    start = iml_add_expression_eval(function,
                                    ir_array_slice_get_start(slice),
                                    NULL);

    end = iml_add_expression_eval(function,
                                  ir_array_slice_get_end(slice),
                                  NULL);

    /*
     * generate code to calculate length of the slice
     */
    /* subtract end from the start */
    length = IML_OPERAND(iml_func_frame_get_temp(frame, iml_32b));
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_sub,
                                                end,
                                                start,
                                                length));
    /* store length in the result blob */
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_setelm,
                                                length,
                                                res,
                                                iml_constant_new_32b(0),
                                                4));


    /*
     * generate code to calculate pointer to the first element in the slice
     */

    /* get sliced array start pointer in */
    start_ptr = iml_func_frame_get_temp(frame, iml_ptr);
    if (DT_IS_STATIC_ARRAY_TYPE(array_type))
    {
        /* static array sliced */
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getaddr,
                                                    array,
                                                    start_ptr));
    }
    else
    {
        /* dynamic array sliced */
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    array,
                                                    iml_constant_new_32b(1),
                                                    4,
                                                    start_ptr));
    }

    /* multiply slice start with element size if needed */
    if (element_size > 1)
    {
        ir_function_def_add_operation(
                function,
                iml_operation_new(iml_umult,
                                  start,
                                  iml_constant_new_32b(element_size),
                                  length));
    }
    else
    {
        length = start;
    }

    /* move forward the array start pointer to slice start */
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_add,
                                                start_ptr,
                                                length,
                                                start_ptr));

    /* store slice start pointer in the result blob */
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_setelm,
                                                start_ptr,
                                                res,
                                                iml_constant_new_32b(1),
                                                4));

    /* mark operands as unused */
    iml_func_frame_unused_oper(frame, start);
    iml_func_frame_unused_oper(frame, end);
    iml_func_frame_unused_oper(frame, length);
    iml_func_frame_unused_oper(frame, IML_OPERAND(start_ptr));

    return IML_OPERAND(res);
}

static void
add_array_assignment(IrFunctionDef *function,
                     IrVariable *lvalue,
                     IrExpression *value)
{
    ImlVariable *dest = ir_variable_get_location(lvalue);

    if (IR_IS_NULL(value))
    {
        /* handle the special case of null assignment */
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_mset,
                                                    iml_constant_new_32b(0),
                                                    2,
                                                    dest));
    }
    else
    {
        iml_add_expression_eval(function, value, dest);
    }

}



static void
add_static_array_assignment(IrFunctionDef *function,
                            IrVariable *lvalue,
                            IrExpression *value)
{
    ImlOperand *res_val;
    ImlVariable *array_var;
    DtArray *array_type;
    guint32 array_length;

    array_type =
            DT_ARRAY(ir_expression_get_data_type(IR_EXPRESSION(lvalue)));

    array_var = ir_variable_get_location(lvalue);
    array_length =
        dt_static_array_get_length(DT_STATIC_ARRAY(array_type));

    if (DT_IS_BASIC(ir_expression_get_data_type(value)))
    {
        /* assignment of basic types to static array */

        res_val = iml_add_expression_eval(function, value, NULL);
        ir_function_def_add_operation(function,
                                      iml_operation_new(iml_mset,
                                                        res_val,
                                                        array_length,
                                                        array_var));
    }
    else
    {
        /*
         * assignment of array type to static array
         */
        assert(DT_IS_ARRAY(ir_expression_get_data_type(value)));

        iml_func_frame_t *frame = ir_function_def_get_frame(function);
        ImlOperand *rvalue;
        ImlVariable *dest_ptr;
        ImlVariable *src_ptr;
        ImlConstant *memcpy_size;

        /* generate code to evaluate the rvalue */
        rvalue = iml_add_expression_eval(function, value, NULL);

        /* generate code to get pointer to the array */
        dest_ptr = iml_func_frame_get_temp(frame, iml_ptr);
        ir_function_def_add_operation(function,
                                      iml_operation_new(iml_getaddr,
                                                        array_var,
                                                        dest_ptr));

        /* generate code to get pointer to rvalue */
        src_ptr = iml_func_frame_get_temp(frame, iml_ptr);
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    rvalue,
                                                    iml_constant_new_32b(1),
                                                    4,
                                                    src_ptr));

        /* generate code to copy rvalue to the array */
        memcpy_size =
            iml_constant_new_32b(array_length *
                                 dt_array_get_element_size(array_type));
        ir_function_def_add_operation(
                function,
                iml_operation_new_call_c("memcpy",
                                         NULL,
                                         dest_ptr,
                                         src_ptr,
                                         memcpy_size,
                                         NULL));

        /* mark temporaries as unused */
        iml_func_frame_unused_oper(frame, rvalue);
        iml_func_frame_unused_oper(frame, IML_OPERAND(dest_ptr));
        iml_func_frame_unused_oper(frame, IML_OPERAND(src_ptr));
    }
}

static void
add_array_cell_assignment(IrFunctionDef *function,
                          IrArrayCell *lvalue,
                          IrExpression *value)
{
    IrVariable *array_symb;
    DtDataType *array_type;
    ImlOperand *res_val;
    ImlOperand *index_val;
    ImlVariable *dest;
    DtDataType *element_type;
    guint size;

    array_symb = ir_array_cell_get_symbol(lvalue);
    array_type = ir_expression_get_data_type(IR_EXPRESSION(array_symb));
    element_type = ir_expression_get_data_type(IR_EXPRESSION(lvalue));

    res_val = iml_add_expression_eval(function, value, NULL);
    dest = ir_variable_get_location(array_symb);
    index_val = iml_add_expression_eval(function,
                                        ir_array_cell_get_index(lvalue),
                                        NULL);
    size = dt_data_type_get_size(element_type);

    if (DT_IS_STATIC_ARRAY_TYPE(array_type))
    {
        /* static array cell assignment */
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_setelm,
                                                    res_val,
                                                    dest,
                                                    index_val,
                                                    size));
    }
    else
    {
        assert(DT_IS_ARRAY(array_type));

        /* dynamic array cell assignment */

        ImlVariable *ptr;
        iml_func_frame_t *frame = ir_function_def_get_frame(function);

        /* generate code to store array pointer in a temp variable */
        ptr = iml_func_frame_get_temp(frame, iml_ptr);
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    dest,
                                                    iml_constant_new_32b(1),
                                                    4,
                                                    ptr));

        /* generate code to store value in the array cell */
        ir_function_def_add_operation(function,
                                  iml_operation_new(iml_setelm,
                                                    res_val,
                                                    ptr,
                                                    index_val,
                                                    size));
    }
}

static void
add_array_slice_assignment(IrFunctionDef *function,
                           IrArraySlice *lvalue,
                           IrExpression *value)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_ARRAY_SLICE(lvalue));
    assert(IR_IS_EXPRESSION(value));

    DtDataType *array_type;
    guint element_size;
    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *src;
    ImlOperand *dest;
    ImlVariable *src_ptr;
    ImlVariable *dest_ptr;
    ImlVariable *length;

    /* figure out the byte size of array elements */
    array_type = ir_expression_get_data_type(value);
    assert(DT_IS_ARRAY(array_type));
    element_size = dt_array_get_element_size(DT_ARRAY(array_type));

    /* generate code to evaluate left and right values */
    dest = iml_add_expression_eval(function, IR_EXPRESSION(lvalue), NULL);
    src = iml_add_expression_eval(function, IR_EXPRESSION(value), NULL);

    /* store source pointer in temp variable */
    src_ptr = iml_func_frame_get_temp(frame, iml_ptr);
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_getelm,
                                                src,
                                                iml_constant_new_32b(1),
                                                4,
                                                src_ptr));

    /* generate code to calculate the length in bytes of the array to copy */
    length = iml_func_frame_get_temp(frame, iml_32b);
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_getelm,
                                                src,
                                                iml_constant_new_32b(0),
                                                4,
                                                length));

    /* multiply length with element size, if needed */
    if (element_size > 1)
    {
        ir_function_def_add_operation(
                function,
                iml_operation_new(iml_umult,
                                  length,
                                  iml_constant_new_32b(element_size),
                                  length));
    }

    /* store destination pointer in temp variable */
    dest_ptr = iml_func_frame_get_temp(frame, iml_ptr);
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_getelm,
                                                dest,
                                                iml_constant_new_32b(1),
                                                4,
                                                dest_ptr));


    /* generate code to copy memory from source array to destination */
    ir_function_def_add_operation(
            function,
            iml_operation_new_call_c("memcpy", NULL,
                                     dest_ptr, src_ptr, length, NULL));
}

static void
add_pointer_assignment(IrFunctionDef *function,
                      IrVariable *lvalue,
                      IrExpression *value)
{
    ImlVariable *dest = ir_variable_get_location(lvalue);

    if (IR_IS_NULL(value))
    {
        /* handle the special case of null assignment */
        ir_function_def_add_operation(function,
                                     iml_operation_new(iml_copy,
                                                       iml_constant_new_32b(0),
                                                       dest));
    }
    else
    {
        iml_add_expression_eval(function, value, dest);
    }
}

static void
add_ptr_dref_assignment(IrFunctionDef *function,
                        IrPtrDref *lvalue,
                        IrExpression *value)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_PTR_DREF(lvalue));
    assert(IR_IS_EXPRESSION(value));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *lval;
    ImlOperand *rval;

    /* generate iml operation to evaluate pointer expression */
    lval = iml_add_expression_eval(function,
                                   ir_ptr_dref_get_expression(lvalue),
                                   NULL);
    /* generate iml operations to evaluate rvalue */
    rval = iml_add_expression_eval(function, value, NULL);

    /* add iml to write rvalue to the destination address */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_set, rval, lval, NULL));

    /* mark left and right operands as unused */
    iml_func_frame_unused_oper(frame, lval);
    iml_func_frame_unused_oper(frame, rval);
}
