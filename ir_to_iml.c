#include "ir_to_iml.h"
#include "ir_basic_constant.h"
#include "ir_array_cell.h"
#include "ir_array_slice.h"
#include "ir_conditional.h"
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
 *                             static table                                  *
 *---------------------------------------------------------------------------*/

/**
 * cast operations table for basic data types
 */
static iml_opcode_t basic_cast_ops[last_basic_type][last_basic_type] =
{
           /* void char bool byte ubyte short ushort int uint */
 /*  void  */ {-1, -1, -1, -1, -1, -1, -1, -1, -1},
 /*  char  */ {-1, iml_copy, iml_copy, iml_copy, iml_copy, iml_trunc, iml_trunc, iml_trunc, iml_trunc},
 /*  bool  */ {-1, iml_bconv, iml_copy, iml_bconv, iml_bconv, iml_bconv, iml_bconv, iml_bconv, iml_bconv},
 /*  byte  */ {-1, iml_copy, iml_copy, iml_copy, iml_copy, iml_trunc, iml_trunc, iml_trunc, iml_trunc},
 /* ubyte  */ {-1, iml_copy, iml_copy, iml_copy, iml_copy, iml_trunc, iml_trunc, iml_trunc, iml_trunc},
 /* short  */ {-1, iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy, iml_trunc, iml_trunc},
 /* ushort */ {-1, iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy, iml_trunc, iml_trunc},
 /*  int   */ {-1, iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy},
 /*  uint  */ {-1, iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy}
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static ImlConstant *
ir_constant_to_iml(IrConstant *constant);

static ImlOperand *
iml_add_unary_op_eval(IrFunctionDef *function,
                      IrUnaryOperation *op,
                      ImlVariable *res,
                      bool discard_result);

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
iml_add_struct_member_eval(IrFunctionDef *function,
                           IrStructMember *ptr_dref,
                           ImlVariable *res);

static ImlOperand *
iml_add_conditional_eval(IrFunctionDef *function,
                         IrConditional *cond,
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
add_struct_assignment(IrFunctionDef *function,
                      IrVariable *lvalue,
                      IrExpression *value);

static void
add_ptr_dref_assignment(IrFunctionDef *function,
                        IrPtrDref *lvalue,
                        IrExpression *value);

static void
add_struct_member_assignment(IrFunctionDef *function,
                             IrStructMember *lvalue,
                             IrExpression *value);

static iml_data_type_t
dt_to_iml_type(DtDataType *dt_type);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Add operations to provided function to evaluate the expression.
 *
 * @param discard_result if true, the result of the expression will not be
 *                       stored anywhere. For expressions with side effects,
 *                       where result is not used, set this flag to avoid
 *                       generating useless temp variable allocations and
 *                       copy operations. The dest parameter must be NULL
 *                       then this flag is set.
 *
 * @return the variable or constant where the result of expression will be
 *         stored
 */
ImlOperand *
iml_add_expression_eval(IrFunctionDef *function,
                        IrExpression *ir_expression,
                        ImlVariable *dest,
                        bool discard_result)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(ir_expression));
    assert(dest == NULL || iml_is_variable(dest));
    assert((discard_result && dest == NULL) || !discard_result);

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
                                    dest,
                                    discard_result);
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
    else if (ir_is_array_literal(ir_expression))
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
        res = iml_add_expression_eval(function, val, dest, discard_result);
    }
    else if (IR_IS_STRUCT_MEMBER(ir_expression))
    {
        res = iml_add_struct_member_eval(function,
                                         IR_STRUCT_MEMBER(ir_expression),
                                         dest);
    }
    else if (IR_IS_CONDITIONAL(ir_expression))
    {
        res = iml_add_conditional_eval(function,
                                       IR_CONDITIONAL(ir_expression),
                                       dest);
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
        assert(res != NULL);
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
                                                           NULL,
                                                           false));
    }
    iml_args = g_slist_reverse(iml_args);

    body = ir_function_def_get_body(function);
    callee =
        IR_FUNCTION(
             sym_table_get_symbol(ir_code_block_get_symbols(body),
                                  ir_function_call_get_name(func_call), NULL));


    if (res == NULL &&
        !dt_basic_is_void(ir_expression_get_data_type(IR_EXPRESSION(func_call))))
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

        if (dt_is_basic(var_type) || DT_IS_ENUM(var_type))
        {
            iml_add_expression_eval(
                    function,
                    value,
                    ir_variable_get_location(IR_VARIABLE(lvalue)),
                    false);
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
        else if (DT_IS_STRUCT(var_type))
        {
            add_struct_assignment(function, IR_VARIABLE(lvalue), value);
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
    else if (IR_IS_STRUCT_MEMBER(lvalue))
    {
        add_struct_member_assignment(function,
                                     IR_STRUCT_MEMBER(lvalue),
                                     value);
    }
    else
    {
        /* unexpected lvalue type */
        assert(false);
    }

}

void
iml_add_while_head(IrFunctionDef *function,
                   IrExpression *condition,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(condition));
    assert(loop_head);
    assert(loop_end);

    /* label the start of loop */
    ir_function_def_add_operation(function, loop_head);

    /* figure out the jump operation to issue after condition evaluation */
    iml_operation_t *jump_op;
    if (ir_expression_is_constant(condition))
    {
        /*
         * a constant expression,
         * this is either an eternal loop or the loop which is always skipped
         */
        assert(IR_IS_BASIC_CONSTANT(condition));
        if (ir_basic_constant_get_bool(IR_BASIC_CONSTANT(condition)))
        {
            /* eternal loop, no jump instruction needed */
            jump_op = NULL;
        }
        else
        {
            /* just skip the loop body */
            jump_op =
                iml_operation_new(iml_jmp,
                                  iml_operation_get_operand(loop_end, 1));
        }
    }
    else
    {
        /* insert iml operation for validation of loop condition */
        ImlOperand *condition_eval_res =
            iml_add_expression_eval(function, condition, NULL, false);

        /* jump past the loop body if condition evaluates to false */
        jump_op =
            iml_operation_new(iml_jmpneq,
                              condition_eval_res,
                              iml_constant_new_8b(1),
                              iml_operation_get_operand(loop_end, 1));
    }

    /* add jump operation if needed */
    if (jump_op != NULL)
    {
        ir_function_def_add_operation(function, jump_op);
    }
}

void
iml_add_while_tail(IrFunctionDef *function,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(loop_head);
    assert(loop_end);

    /* jump to loop start */
    ir_function_def_add_operation(
            function,
            iml_operation_new(iml_jmp,
                              iml_operation_get_operand(loop_head, 1)));

    /* and loop end label */
    ir_function_def_add_operation(function, loop_end);
}

void
iml_add_foreach_head(IrFunctionDef *function,
                     IrForeach *foreach,
                     ImlVariable **index,
                     ImlVariable **length,
                     iml_operation_t **loop_head,
                     iml_operation_t *loop_end)
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
                                                    iml_constant_zero_32b(),
                                                    *index));

    /* set-up value variable */
    ir_value = ir_foreach_get_value(foreach);
    assert(ir_value);
    add_to_func_frame(function, ir_value, false);
    value = ir_variable_get_location(ir_value);

    /* generate iml to evaluate aggregate expression */
    ir_aggregate = ir_foreach_get_aggregate(foreach);
    aggregate_type = DT_ARRAY(ir_expression_get_data_type(ir_aggregate));
    aggregate = iml_add_expression_eval(function, ir_aggregate, NULL, false);

    /* store length of the aggregate array in a temp variable */
    *length = iml_func_frame_get_temp(frame, iml_32b);
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    aggregate,
                                                    iml_constant_zero_32b(),
                                                    4,
                                                    *length));

    /* store pointer to the start of aggregate array in a temp variable */
    aggr_ptr = iml_func_frame_get_temp(frame, iml_ptr);
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_getelm,
                                                    aggregate,
                                                    iml_constant_one_32b(),
                                                    4,
                                                    aggr_ptr));

    /* insert loop label */
    *loop_head =
        iml_operation_new(iml_label,
                          ir_module_gen_label(
                              ir_symbol_get_parent_module(
                                  IR_SYMBOL(function))));
    ir_function_def_add_operation(function, *loop_head);

    /* generate iml to jump to loop head if index is less then length */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_jmpugreatereq,
                          *index,
                          *length,
                          iml_operation_get_operand(loop_end, 1)));

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
                                                    iml_constant_one_32b(),
                                                    index));

    /* generate iml to jump to loop head */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_jmp,
                          iml_operation_get_operand(loop_label, 1)));
}

ImlOperand *
iml_add_foreach_range_head(IrFunctionDef *function,
                           IrVariable *index,
                           IrExpression *lower_exp,
                           IrExpression *loop_test_exp,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_VARIABLE(index));
    assert(IR_IS_EXPRESSION(lower_exp));
    assert(IR_IS_EXPRESSION(loop_test_exp));
    assert(loop_head);
    assert(loop_end);

    /* assign lower expression result to index variable */
    iml_add_assignment(function, IR_EXPRESSION(index), lower_exp);

    /* insert loop head label */
    ir_function_def_add_operation(function, loop_head);

    /* evaluate upper expressions */
    ImlOperand *loop_test_res =
        iml_add_expression_eval(function, loop_test_exp, NULL, false);

    ir_function_def_add_operation(function,
        iml_operation_new(iml_jmpneq,
                          loop_test_res,
                          iml_constant_true(),
                          iml_operation_get_operand(loop_end, 1)));

    return loop_test_res;
}

void
iml_add_foreach_range_tail(IrFunctionDef *function,
                           IrExpression *inc_exp,
                           ImlOperand *head_temp_op,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(inc_exp));
    assert(IML_IS_OPERAND(head_temp_op));
    assert(loop_head);
    assert(loop_end);

    /* generate iml for loop's index increment expression */
    iml_add_expression_eval(function, inc_exp, NULL, true);

    /* generate jump to the loop's head */
    ir_function_def_add_operation(function,
        iml_operation_new(iml_jmp,
                          iml_operation_get_operand(loop_head, 1)));

    /* insert loop end label */
    ir_function_def_add_operation(function, loop_end);

    /* mark possible temp variables used in the loop as unused */
    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    iml_func_frame_unused_oper(frame, head_temp_op);
}

void
iml_add_for_head(IrFunctionDef *function,
                 IrExpression *test,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(test) || test == NULL);
    assert(loop_head);
    assert(loop_end);

    /* insert loop head label */
    ir_function_def_add_operation(function, loop_head);

    if (test == NULL)
    {
        /* if there is no loop test expression, then we are done */
        return;
    }

    iml_func_frame_t *frame = ir_function_def_get_frame(function);

    /* generate code for evaluating loop test expression */
    ImlVariable *test_res = iml_func_frame_get_temp(frame, iml_8b);
    iml_add_expression_eval(function, test, test_res, false);

    /* skip loop body if test expression evaluates to false */
    ir_function_def_add_operation(
            function,
            iml_operation_new(iml_jmpneq,
                              test_res,
                              iml_constant_true(),
                              iml_operation_get_operand(loop_end, 1)));

    /* mark test expression result variable as unused */
    iml_func_frame_unused_oper(frame, IML_OPERAND(test_res));
}

void
iml_add_for_tail(IrFunctionDef *function,
                 IrExpression *step,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_EXPRESSION(step) || step == NULL);
    assert(loop_head);
    assert(loop_end);

    if (step != NULL)
    {
        /* generate code to evaluate loop step expression */
        iml_add_expression_eval(function, step, NULL, true);
    }


    /* generate code to jump to loop start */
    ir_function_def_add_operation(
            function,
            iml_operation_new(iml_jmp,
                              iml_operation_get_operand(loop_head, 1)));

    /* insert loop end label */
    ir_function_def_add_operation(function, loop_end);
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static iml_data_type_t
dt_to_iml_type(DtDataType *dt_type)
{
    iml_data_type_t iml_type;

    if (dt_is_basic(dt_type))
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
    else if (DT_IS_ARRAY(dt_type) || DT_IS_STRUCT(dt_type))
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
 * Figure out if an operation is signed or unsigned by looking at data types
 * of it's operands.
 *
 * @return true if op is signed operation, false otherwise
 */
static bool
is_signed_op(IrBinaryOperation *op)
{
    DtDataType *operands_type;

    ast_binary_op_type_t optype = ir_binary_operation_get_operation(op);
    assert(optype == ast_mult_op ||
           optype == ast_division_op ||
           optype == ast_modulo_op ||
           optype == ast_less_op ||
           optype == ast_greater_op ||
           optype == ast_less_or_eq_op ||
           optype == ast_greater_or_eq_op);

    operands_type =
       ir_expression_get_data_type(ir_binary_operation_get_left(op));

    if (dt_is_basic(operands_type))
    {
        return dt_basic_is_signed(DT_BASIC(operands_type));
    }

    assert(DT_IS_POINTER(operands_type));
    return true;
}

/**
 * Map IR binary operation to IML opcode
 */
static iml_opcode_t
get_iml_opcode_binop(IrBinaryOperation *op)
{
    iml_opcode_t opcode;

    switch (ir_binary_operation_get_operation(op))
    {
        case ast_plus_op:
            opcode = iml_add;
            break;
        case ast_minus_op:
            opcode = iml_sub;
            break;
        case ast_mult_op:
            opcode = is_signed_op(op) ? iml_smult : iml_umult;
            break;
        case ast_division_op:
            opcode = is_signed_op(op) ? iml_sdiv : iml_udiv;
            break;
        case ast_modulo_op:
            opcode = is_signed_op(op) ? iml_smod : iml_umod;
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
            opcode = is_signed_op(op) ? iml_sless : iml_uless;
            break;
        case ast_greater_op:
            opcode = is_signed_op(op) ? iml_sgreater : iml_ugreater;
            break;
        case ast_less_or_eq_op:
            opcode = is_signed_op(op) ? iml_slesseq : iml_ulesseq;
            break;
        case ast_greater_or_eq_op:
            opcode = is_signed_op(op) ? iml_sgreatereq : iml_ugreatereq;
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
    guint16 v16;
    guint32 v32;

    assert(IR_IS_BASIC_CONSTANT(constant));

    DtDataType *exp_type =
        ir_expression_get_data_type(IR_EXPRESSION(constant));

    if (dt_basic_is_int(exp_type))
    {
        type = iml_32b;
        v32 = ir_basic_constant_get_int(IR_BASIC_CONSTANT(constant));
    }
    else if (dt_basic_is_uint(exp_type))
    {
        type = iml_32b;
        v32 = ir_basic_constant_get_uint(IR_BASIC_CONSTANT(constant));
    }
    else if (dt_basic_is_short(exp_type))
    {
        type = iml_16b;
        v16 = ir_basic_constant_get_short(IR_BASIC_CONSTANT(constant));
    }
    else if (dt_basic_is_ushort(exp_type))
    {
        type = iml_16b;
        v16 = ir_basic_constant_get_ushort(IR_BASIC_CONSTANT(constant));
    }
    else if (dt_basic_is_byte(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_byte(IR_BASIC_CONSTANT(constant));
    }
    else if (dt_basic_is_ubyte(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_ubyte(IR_BASIC_CONSTANT(constant));
    }
    else if (dt_basic_is_char(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_char(IR_BASIC_CONSTANT(constant));
    }
    else if (dt_basic_is_bool(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_bool(IR_BASIC_CONSTANT(constant));
    }
    else
    {
        assert(false); /* unexpected data type */
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
iml_add_neg_op_eval(IrFunctionDef *function,
                    IrUnaryOperation *op,
                    ImlVariable *res)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_UNARY_OPERATION(op));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *operand;
    iml_data_type_t iml_type;
    iml_opcode_t opcode;

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
                                      NULL,
                                      false);

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
iml_add_incdec_op_eval(IrFunctionDef *function,
                       IrUnaryOperation *op,
                       ImlVariable *res,
                       bool discard_result)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_UNARY_OPERATION(op));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *operand;
    IrExpression *ir_operand;
    DtDataType *operand_type;
    iml_operation_t *mod_op;
    iml_opcode_t opcode;
    ImlConstant *mod_constant;
    bool copy_after_mod;

    switch (ir_unary_operation_get_operation(op))
    {
        case ast_pre_inc_op:
            opcode = iml_add;
            copy_after_mod = true;
            break;
        case ast_pre_dec_op:
            opcode = iml_sub;
            copy_after_mod = true;
            break;
        case ast_post_inc_op:
            opcode = iml_add;
            copy_after_mod = false;
            break;
        case ast_post_dec_op:
            opcode = iml_sub;
            copy_after_mod = false;
            break;
        default:
            /* unexpected unary operation */
            assert(false);
    }

    ir_operand = ir_unary_operation_get_operand(op);
    operand_type = ir_expression_get_data_type(ir_operand);

    /*
     * figure out the size of increment/decrement operation
     */
    if (DT_IS_POINTER(operand_type))
    {
        /* pointers are (in/de)-cremented by the base type size */
        mod_constant =
            iml_constant_new_32b(
                dt_pointer_get_base_type_size(DT_POINTER(operand_type)));
    }
    else
    {
        mod_constant = iml_constant_one_32b();
    }

    /*
     * create the increment/decrement operation
     */
    operand = iml_add_expression_eval(function, ir_operand, NULL, false);
    mod_op = iml_operation_new(opcode, operand, mod_constant, operand);

    /*
     * create operation to store the value
     * of this expression to result operand
     */
    iml_operation_t *copy_op = NULL;
    if (!discard_result)
    {
        if (res == NULL)
        {
            res = iml_func_frame_get_temp(frame, dt_to_iml_type(operand_type));
        }
        copy_op = iml_operation_new(iml_copy, operand, res);
    }

    if (copy_after_mod)
    {
        /* pre (in/de)-crement operation */
        ir_function_def_add_operation(function, mod_op);
        if (copy_op != NULL)
        {
            ir_function_def_add_operation(function, copy_op);
        }
    }
    else
    {
        /* post (in/de)-crement operation */
        if (copy_op != NULL)
        {
            ir_function_def_add_operation(function, copy_op);
        }
        ir_function_def_add_operation(function, mod_op);
    }

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_unary_op_eval(IrFunctionDef *function,
                      IrUnaryOperation *op,
                      ImlVariable *res,
                      bool discard_result)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_UNARY_OPERATION(op));

    switch (ir_unary_operation_get_operation(op))
    {
        case ast_arithm_neg_op:
        case ast_bool_neg_op:
            return iml_add_neg_op_eval(function, op, res);
        case ast_pre_inc_op:
        case ast_pre_dec_op:
        case ast_post_inc_op:
        case ast_post_dec_op:
            return iml_add_incdec_op_eval(function, op, res, discard_result);
        default:
            /* unexpected unary operation */
            assert(false);
    }
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
                                   NULL,
                                   false);
    right = iml_add_expression_eval(function,
                                    ir_binary_operation_get_right(bin_op),
                                    NULL,
                                    false);

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

static iml_opcode_t
get_cast_opcode(IrCast *cast)
{
    DtDataType *src_type =
        ir_expression_get_data_type(ir_cast_get_value(cast));
    DtDataType *target_type = ir_cast_get_target_type(cast);

    if (dt_is_basic(target_type))
    {
        if (dt_is_basic(src_type))
        {
            basic_data_type_t src_bdt =
                dt_basic_get_data_type(DT_BASIC(src_type));
            basic_data_type_t trgt_bdt =
                dt_basic_get_data_type(DT_BASIC(target_type));

            assert(basic_cast_ops[trgt_bdt][src_bdt] != -1);
            return basic_cast_ops[trgt_bdt][src_bdt];
        }
        else
        {
            /* unexpected source data type */
            assert(false);
        }
    }

    /* unexpected target data type */
    assert(false);
}

static ImlOperand *
iml_add_cast_eval(IrFunctionDef *function, IrCast *cast, ImlVariable *dest)
{
    iml_opcode_t cast_op = get_cast_opcode(cast);

    ImlOperand *src =
        iml_add_expression_eval(function,
                                ir_cast_get_value(cast),
                                NULL,
                                false);
    if (cast_op == iml_copy)
    {
        return src;
    }

    if (dest == NULL)
    {
        dest =
            iml_func_frame_get_temp(
                ir_function_def_get_frame(function),
                dt_to_iml_type(ir_cast_get_target_type(cast)));
    }

    ir_function_def_add_operation(function,
                                   iml_operation_new(cast_op,
                                                     src,
                                                     dest));
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
                                        NULL,
                                        false);

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
                                                    iml_constant_one_32b(),
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
                                      NULL,
                                      false);

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
        iml_operation_new(iml_get, ptr_exp, NULL, res));

    /* pointer expression operand not needed any more */
    iml_func_frame_unused_oper(frame, ptr_exp);

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_conditional_eval(IrFunctionDef *function,
                         IrConditional *cond,
                         ImlVariable *res)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_CONDITIONAL(cond));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlVariable *cond_var;

    /* figure out where to store the result */
    if (res == NULL)
    {
        iml_data_type_t res_type;

        res_type =
            dt_to_iml_type(
                ir_expression_get_data_type(IR_EXPRESSION(cond)));
        res = iml_func_frame_get_temp(frame, res_type);
    }

    /* generate labels */
    IrModule *mod = ir_symbol_get_parent_module(IR_SYMBOL(function));
    char *skip_label = ir_module_gen_label(mod);
    char *end_label = ir_module_gen_label(mod);

    /* evaluate condition expression */
    cond_var =
        IML_VARIABLE(iml_add_expression_eval(function,
                                             ir_conditional_get_cond(cond),
                                             NULL,
                                             false));

    /* insert jump to false evaluation operation */
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_jmpneq,
                                                    cond_var,
                                                    iml_constant_true(),
                                                    skip_label));

    /* evalute true expression */
    iml_add_expression_eval(function,
                            ir_conditional_get_true(cond),
                            res,
                            false);

    /* insert skip false evaluation jump */
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_jmp,
                                                    end_label));

    /* insert skip label */
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_label, skip_label));

    /* evaluate false expression */
    iml_add_expression_eval(function,
                            ir_conditional_get_false(cond),
                            res,
                            false);

    /* insert end label */
    ir_function_def_add_operation(function,
                                  iml_operation_new(iml_label, end_label));

    /* free conditional variable if it was temporary */
    iml_func_frame_unused_oper(frame, IML_OPERAND(cond_var));

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_struct_member_eval(IrFunctionDef *function,
                           IrStructMember *struct_member,
                           ImlVariable *res)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_STRUCT_MEMBER(struct_member));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *base;
    ImlConstant *offset;

    /* generate iml operation to evaluate struct base expression */
    base = iml_add_expression_eval(function,
                                   ir_struct_member_get_base(struct_member),
                                   NULL,
                                   false);

    /* if base expression is of blob type, get a pointer to it */
    if (iml_operand_get_data_type(base) == iml_blob)
    {
        /* replace base operand with a pointer to it */
        ImlVariable *base_ptr = iml_func_frame_get_temp(frame, iml_ptr);

        ir_function_def_add_operation(function,
            iml_operation_new(iml_getaddr, base, base_ptr));

        iml_func_frame_unused_oper(frame, base);
        base = IML_OPERAND(base_ptr);
    }

    /* figure out where to store the result */
    if (res == NULL)
    {
        iml_data_type_t res_type;

        res_type =
            dt_to_iml_type(
                ir_expression_get_data_type(IR_EXPRESSION(struct_member)));
        res = iml_func_frame_get_temp(frame, res_type);
    }

    /* store rvalue at the base + offset */
    offset = iml_constant_new_32b(ir_struct_member_get_offset(struct_member));
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_get, base, offset, res));

    /* mark base operand as unused */
    iml_func_frame_unused_oper(frame, base);

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
                                                iml_constant_zero_32b(),
                                                4,
                                                res));

    return IML_OPERAND(res);
}

static ImlOperand *
iml_add_array_literal_eval(IrFunctionDef *function,
                           IrArrayLiteral *expr,
                           ImlVariable *res)
{
    assert(ir_is_array_literal(expr));

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

        const char *label = ir_literal_get_data_label(IR_LITERAL(expr));
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
                                    temp,
                                    false);
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
                           iml_constant_zero_32b(),
                           4);
    ir_function_def_add_operation(function, op);

    op = iml_operation_new(iml_setelm,
                           ptr,
                           res,
                           iml_constant_one_32b(),
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
                                    NULL,
                                    false);

    start = iml_add_expression_eval(function,
                                    ir_array_slice_get_start(slice),
                                    NULL,
                                    false);

    end = iml_add_expression_eval(function,
                                  ir_array_slice_get_end(slice),
                                  NULL,
                                  false);

    /*
     * generate code to calculate length of the slice
     */

    /* subtract end from the start */
    if (iml_is_constant(end) && iml_is_constant(start))
    {
        length =
          IML_OPERAND(
            iml_constant_new_32b(
                iml_constant_get_val_32b(IML_CONSTANT(end)) -
                iml_constant_get_val_32b(IML_CONSTANT(start))));
    }
    else
    {
        length = IML_OPERAND(iml_func_frame_get_temp(frame, iml_32b));
        ir_function_def_add_operation(function,
                                      iml_operation_new(iml_sub,
                                                        end,
                                                        start,
                                                        length));
    }
    /* store length in the result blob */
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_setelm,
                                                length,
                                                res,
                                                iml_constant_zero_32b(),
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
                                                        iml_constant_one_32b(),
                                                        4,
                                                        start_ptr));
    }

    /* multiply slice start with element size if needed */
    if (element_size > 1)
    {
        if (iml_is_constant(start))
        {
            length =
                IML_OPERAND(
                    iml_constant_new_32b(
                        element_size *
                            iml_constant_get_val_32b(IML_CONSTANT(start))));
        }
        else
        {
            ir_function_def_add_operation(
                    function,
                    iml_operation_new(iml_umult,
                                      start,
                                      iml_constant_new_32b(element_size),
                                      length));
        }
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
                                                    iml_constant_one_32b(),
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
                                                    iml_constant_zero_32b(),
                                                    2,
                                                    dest));
    }
    else
    {
        iml_add_expression_eval(function, value, dest, false);
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

    if (dt_is_basic(ir_expression_get_data_type(value)))
    {
        /* assignment of basic types to static array */

        res_val = iml_add_expression_eval(function, value, NULL, false);
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
        rvalue = iml_add_expression_eval(function, value, NULL, false);

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
                                                    iml_constant_one_32b(),
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

    res_val = iml_add_expression_eval(function, value, NULL, false);
    dest = ir_variable_get_location(array_symb);
    index_val = iml_add_expression_eval(function,
                                        ir_array_cell_get_index(lvalue),
                                        NULL,
                                        false);
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
                                                    iml_constant_one_32b(),
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
    dest =
        iml_add_expression_eval(function, IR_EXPRESSION(lvalue), NULL, false);
    src =
        iml_add_expression_eval(function, IR_EXPRESSION(value), NULL, false);

    /* store source pointer in temp variable */
    src_ptr = iml_func_frame_get_temp(frame, iml_ptr);
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_getelm,
                                                src,
                                                iml_constant_one_32b(),
                                                4,
                                                src_ptr));

    /* generate code to calculate the length in bytes of the array to copy */
    length = iml_func_frame_get_temp(frame, iml_32b);
    ir_function_def_add_operation(function,
                              iml_operation_new(iml_getelm,
                                                src,
                                                iml_constant_zero_32b(),
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
                                                iml_constant_one_32b(),
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
        ir_function_def_add_operation(
            function,
            iml_operation_new(iml_copy,
                              iml_constant_zero_32b(),
                              dest));
    }
    else
    {
        iml_add_expression_eval(function, value, dest, false);
    }
}

static void
add_struct_assignment(IrFunctionDef *function,
                      IrVariable *lvalue,
                      IrExpression *value)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_VARIABLE(lvalue));
    assert(IR_IS_EXPRESSION(value));

    /* only assignment of struct literals implemented */
    assert(ir_is_struct_literal(value));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);

    ImlVariable *lval = ir_variable_get_location(lvalue);
    ImlVariable *lval_ptr =
        iml_func_frame_get_temp(frame, iml_ptr);

    /* generate code to store pointer to the stuct in a temp variable */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_getaddr, lval, lval_ptr));

    /*
     *  generate code to call memcpy to copy struct literal from
     *  .data section address to struct's memory
     */
    char *label = ir_literal_get_data_label(IR_LITERAL(value));
    ImlConstant *size = iml_constant_new_32b(iml_variable_get_size(lval));

    ir_function_def_add_operation(
        function,
        iml_operation_new_call_c("memcpy",
                                 NULL,
                                 lval_ptr,
                                 iml_constant_new_ptr(label),
                                 size,
                                 NULL));

    /* release temp variable */
    iml_func_frame_unused_oper(frame, IML_OPERAND(lval_ptr));
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
                                   NULL,
                                   false);
    /* generate iml operations to evaluate rvalue */
    rval = iml_add_expression_eval(function, value, NULL, false);

    /* add iml to write rvalue to the destination address */
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_set, rval, lval, NULL));

    /* mark left and right operands as unused */
    iml_func_frame_unused_oper(frame, lval);
    iml_func_frame_unused_oper(frame, rval);
}

static void
add_struct_member_assignment(IrFunctionDef *function,
                             IrStructMember *lvalue,
                             IrExpression *value)
{
    assert(IR_IS_FUNCTION_DEF(function));
    assert(IR_IS_STRUCT_MEMBER(lvalue));
    assert(IR_IS_EXPRESSION(value));

    iml_func_frame_t *frame = ir_function_def_get_frame(function);
    ImlOperand *base;
    ImlOperand *rval;
    ImlConstant *offset;

    /* generate iml operation to evaluate struct base expression */
    base = iml_add_expression_eval(function,
                                   ir_struct_member_get_base(lvalue),
                                   NULL,
                                   false);

    /* if base expression is of blob type, get a pointer to it */
    if (iml_operand_get_data_type(base) == iml_blob)
    {
        /* replace base operand with a pointer to it */
        ImlVariable *base_ptr = iml_func_frame_get_temp(frame, iml_ptr);

        ir_function_def_add_operation(function,
            iml_operation_new(iml_getaddr, base, base_ptr));

        iml_func_frame_unused_oper(frame, base);
        base = IML_OPERAND(base_ptr);
    }

    /* generate iml operations to evaluate rvalue */
    rval = iml_add_expression_eval(function, value, NULL, false);

    /* store rvalue at the base + offset */
    offset = iml_constant_new_32b(ir_struct_member_get_offset(lvalue));
    ir_function_def_add_operation(
        function,
        iml_operation_new(iml_set, rval, base, offset));

    /* mark operands as unused */
    iml_func_frame_unused_oper(frame, base);
    iml_func_frame_unused_oper(frame, rval);
}
