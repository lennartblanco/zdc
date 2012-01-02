#include "ir_to_iml.h"
#include "ir_basic_constant.h"
#include "ir_array_cell.h"
#include "ir_array_slice.h"
#include "ir_assignment.h"
#include "ir_conditional.h"
#include "ir_unary_operation.h"
#include "ir_binary_operation.h"
#include "ir_ptr_dref.h"
#include "ir_address_of.h"
#include "ir_property.h"
#include "ir_cast.h"
#include "ir_null.h"
#include "ir_var_ref.h"
#include "iml_constant.h"
#include "iml_variable.h"
#include "types.h"
#include "dt_void.h"
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
           /* char bool byte ubyte short ushort int uint */
 /*  char  */ {iml_copy, iml_copy, iml_copy, iml_copy, iml_trunc, iml_trunc, iml_trunc, iml_trunc},
 /*  bool  */ {iml_bconv, iml_copy, iml_bconv, iml_bconv, iml_bconv, iml_bconv, iml_bconv, iml_bconv},
 /*  byte  */ {iml_copy, iml_copy, iml_copy, iml_copy, iml_trunc, iml_trunc, iml_trunc, iml_trunc},
 /* ubyte  */ {iml_copy, iml_copy, iml_copy, iml_copy, iml_trunc, iml_trunc, iml_trunc, iml_trunc},
 /* short  */ {iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy, iml_trunc, iml_trunc},
 /* ushort */ {iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy, iml_trunc, iml_trunc},
 /*  int   */ {iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy},
 /*  uint  */ {iml_zpad, iml_zpad, iml_sigext, iml_zpad, iml_sigext, iml_zpad, iml_copy, iml_copy}
};

static iml_opcode_t basic_to_ptr_cast_ops[last_basic_type] =
{
    iml_zpad,   /* char */
    iml_zpad,   /* bool */
    iml_sigext, /* byte */
    iml_zpad,   /* ubyte */
    iml_sigext, /* short */
    iml_zpad,   /* ushort */
    iml_copy,   /* int */
    iml_copy    /* uint */
};

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static ImlConstant *
ir_constant_to_iml(IrConstant *constant);

static ImlOperand *
iml_add_unary_op_eval(iml_function_t *function,
                      IrUnaryOperation *op,
                      ImlVariable *res,
                      bool discard_result);

static ImlOperand *
iml_add_binary_op_eval(iml_function_t *function,
                       IrBinaryOperation *bin_op,
                       ImlVariable *res);

static ImlOperand *
iml_add_cast_eval(iml_function_t *function,
                  IrCast *cast,
                  ImlVariable *dest);

static ImlOperand *
iml_add_array_cell_eval(iml_function_t *function,
                        IrArrayCell *cell,
                        ImlVariable *res);

static ImlOperand *
iml_add_array_slice_eval(iml_function_t *function,
                         IrArraySlice *slice,
                         ImlVariable *res);

static ImlOperand *
iml_add_ptr_dref_eval(iml_function_t *function,
                      IrExpression *ref,
                      ImlVariable *res);

static ImlOperand *
iml_add_address_of_eval(iml_function_t *function,
                        IrAddressOf *addr_of,
                        ImlVariable *res);

static ImlOperand *
iml_add_struct_member_eval(iml_function_t *function,
                           IrStructMember *struct_member,
                           ImlVariable *res);

static ImlOperand *
iml_add_conditional_eval(iml_function_t *function,
                         IrConditional *cond,
                         ImlVariable *res);

static ImlOperand *
iml_add_property_eval(iml_function_t *function,
                      IrProperty *prop,
                      ImlVariable *res);

static void
add_array_cell_assignment(iml_function_t *function,
                          IrArrayCell *lvalue,
                          IrExpression *value);

static void
add_array_slice_assignment(iml_function_t *function,
                           IrArraySlice *lvalue,
                           IrExpression *value);

static void
add_array_assignment(iml_function_t *function,
                     IrVariable *lvalue,
                     IrExpression *value);

static void
add_static_array_assignment(iml_function_t *function,
                            IrVariable *lvalue,
                            IrExpression *value);

static ImlOperand *
iml_add_array_literal_eval(iml_function_t *function,
                           IrArrayLiteral *expr,
                           ImlVariable *res);

static void
add_pointer_assignment(iml_function_t *function,
                       IrVariable *lvalue,
                       IrExpression *value);

static void
add_struct_assignment(iml_function_t *function,
                      IrVariable *lvalue,
                      IrExpression *value);

static void
add_ptr_dref_assignment(iml_function_t *function,
                        IrExpression *lvalue,
                        IrExpression *value);

static void
add_struct_member_assignment(iml_function_t *function,
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
iml_add_expression_eval(iml_function_t *function,
                        IrExpression *ir_expression,
                        ImlVariable *dest,
                        bool discard_result)
{
    assert(function);
    assert(IR_IS_EXPRESSION(ir_expression));
    assert(dest == NULL || iml_is_variable(dest));
    assert((discard_result && dest == NULL) || !discard_result);

    ImlOperand *res = NULL;

    if (IR_IS_CONSTANT(ir_expression))
    {
        res = iml_operand(ir_constant_to_iml(IR_CONSTANT(ir_expression)));
    }
    else if (IR_IS_VAR_VALUE(ir_expression))
    {
        IrVariable *var = ir_var_value_get_var(IR_VAR_VALUE(ir_expression));
        res = iml_operand(ir_variable_get_location(var));
    }
    else if (IR_IS_ASSIGNMENT(ir_expression))
    {
        IrAssignment *assign = ir_assignment(ir_expression);
        res = iml_add_assignment(function,
                                 ir_assignment_get_lvalue(assign),
                                 ir_assignment_get_value(assign));
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
                                     ir_binary_operation(ir_expression),
                                     dest);
    }
    else if (IR_IS_CALL(ir_expression))
    {
        res = iml_add_call_eval(function, IR_CALL(ir_expression), dest);
    }
    else if (IR_IS_CAST(ir_expression))
    {
        res = iml_add_cast_eval(function, ir_cast(ir_expression), dest);
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
                                         ir_array_literal(ir_expression),
                                         dest);
    }
    else if (IR_IS_ARRAY_SLICE(ir_expression))
    {
        res = iml_add_array_slice_eval(function,
                                       IR_ARRAY_SLICE(ir_expression),
                                       dest);
    }
    else if (IR_IS_PTR_DREF(ir_expression) || IR_IS_VAR_REF(ir_expression))
    {
        res = iml_add_ptr_dref_eval(function,
                                    ir_expression,
                                    dest);
    }
    else if (IR_IS_ADDRESS_OF(ir_expression))
    {
        res = iml_add_address_of_eval(function,
                                      ir_address_of(ir_expression),
                                      dest);
    }
    else if (IR_IS_ENUM_MEMBER(ir_expression))
    {
        IrExpression *val;

        val = ir_enum_member_get_value(ir_enum_member(ir_expression));
        res = iml_add_expression_eval(function, val, dest, discard_result);
    }
    else if (IR_IS_STRUCT_MEMBER(ir_expression))
    {
        res = iml_add_struct_member_eval(function,
                                         ir_struct_member(ir_expression),
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
    if (dest != NULL && res != iml_operand(dest))
    {
        assert(res != NULL);
        iml_function_add_operation(function,
                                   iml_operation_new(iml_copy, res, dest));
        res = iml_operand(dest);
    }

    return res;
}

void
add_to_func_frame(iml_function_t *function,
                  IrVariable *variable,
                  bool is_function_parameter)
{
    assert(function);
    assert(IR_IS_VARIABLE(variable));

    iml_data_type_t iml_datatype;
    DtDataType *ir_datatype;
    char *var_name;

    /* create IML variable of appropriate type */
    ir_datatype = ir_variable_get_data_type(variable);
    if (ir_variable_is_ref(variable))
    {
        iml_datatype = iml_ptr;
    }
    else
    {
        iml_datatype = dt_to_iml_type(ir_datatype);
    }

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
    if (is_function_parameter) {
        iml_function_add_parameter(function, iml_var);
    } else {
        iml_function_add_local(function, iml_var);
    }
    ir_variable_set_location(variable, iml_var);
}

ImlOperand *
iml_add_call_eval(iml_function_t *function, IrCall *call, ImlVariable *res)
{
    GSList *iml_args = NULL;
    GSList *ir_args;
    GSList *i;
    iml_operation_t *call_op;
    IrFunction *callee;
    iml_opcode_t opcode;

    switch (ir_call_get_linkage(call))
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

    /*
     * generate iml code for eveluation of call parameters
     */
    ir_args = ir_call_get_arguments(call);

    /* add 'this' argument if present */
    if (ir_call_get_this_arg(call) != NULL)
    {
        ir_args = g_slist_append(ir_args, ir_call_get_this_arg(call));
    }

    for (i = ir_args; i != NULL; i = g_slist_next(i))
    {
        iml_args = g_slist_prepend(iml_args,
                                   iml_add_expression_eval(function,
                                                           i->data,
                                                           NULL,
                                                           false));
    }
    iml_args = g_slist_reverse(iml_args);

    callee = ir_call_get_function(call);

    if (res == NULL &&
        !DT_IS_VOID(ir_expression_get_data_type(ir_expression(call))))
    {
        res = iml_function_get_temp(function, iml_32b);
    }

    call_op = iml_operation_new(opcode,
                                ir_function_get_mangled_name(callee),
                                iml_args,
                                res);
    iml_function_add_operation(function, call_op);

    /* mark any variables used as function arguments as unused */
    for (i = iml_args; i != NULL; i = g_slist_next(i))
    {
        iml_function_unused_oper(function, i->data);
    }

    return iml_operand(res);
}

ImlOperand *
iml_add_assignment(iml_function_t *function,
                   IrExpression *lvalue,
                   IrExpression *value)
{
    assert(function);
    assert(IR_IS_EXPRESSION(lvalue));
    assert(ir_expression_is_lvalue(lvalue));
    assert(IR_IS_EXPRESSION(value));

    ImlVariable *res = NULL;

    if (IR_IS_VAR_VALUE(lvalue))
    {
        DtDataType *var_type;

        var_type = ir_expression_get_data_type(lvalue);
        IrVariable *var = ir_var_value_get_var(IR_VAR_VALUE(lvalue));
        res = ir_variable_get_location(var);

        if (dt_is_basic(var_type) || dt_is_enum(var_type))
        {
            iml_add_expression_eval(
                    function,
                    value,
                    res,
                    false);
        }
        else if (DT_IS_STATIC_ARRAY_TYPE(var_type))
        {
            add_static_array_assignment(function, var, value);
        }
        else if (DT_IS_ARRAY(var_type))
        {
            add_array_assignment(function, var, value);
        }
        else if (DT_IS_POINTER(var_type))
        {
            add_pointer_assignment(function, var, value);
        }
        else if (DT_IS_STRUCT(var_type))
        {
            add_struct_assignment(function, var, value);
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
    else if (IR_IS_PTR_DREF(lvalue) || IR_IS_VAR_REF(lvalue))
    {
        add_ptr_dref_assignment(function, lvalue, value);
    }
    else if (IR_IS_STRUCT_MEMBER(lvalue))
    {
        add_struct_member_assignment(function,
                                     ir_struct_member(lvalue),
                                     value);
    }
    else
    {
        /* unexpected lvalue type */
        assert(false);
    }

    return iml_operand(res);
}

void
iml_add_while_head(iml_function_t *function,
                   IrExpression *condition,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end)
{
    assert(function);
    assert(IR_IS_EXPRESSION(condition));
    assert(loop_head);
    assert(loop_end);

    /* label the start of loop */
    iml_function_add_operation(function, loop_head);

    /* figure out the jump operation to issue after condition evaluation */
    iml_operation_t *jump_op;
    if (ir_expression_is_constant(condition))
    {
        /*
         * a constant expression,
         * this is either an eternal loop or the loop which is always skipped
         */
        assert(IR_IS_BASIC_CONSTANT(condition));
        if (ir_basic_constant_get_bool(ir_basic_constant(condition)))
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
        iml_function_add_operation(function, jump_op);
    }
}

void
iml_add_while_tail(iml_function_t *function,
                   iml_operation_t *loop_head,
                   iml_operation_t *loop_end)
{
    assert(function);
    assert(loop_head);
    assert(loop_end);

    /* jump to loop start */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_jmp,
                          iml_operation_get_operand(loop_head, 1)));

    /* and loop end label */
    iml_function_add_operation(function, loop_end);
}

void
iml_add_foreach_head(iml_function_t *function,
                     IrForeach *foreach,
                     ImlVariable **index,
                     ImlVariable **length,
                     iml_operation_t *loop_head,
                     iml_operation_t *loop_end)
{
    IrExpression *ir_aggregate;
    DtArray *aggregate_type;
    ImlOperand *aggregate;
    IrVariable *ir_index;
    IrVariable *ir_value;
    ImlVariable *value;
    ImlVariable *aggr_ptr;

    /* set-up index variable */
    ir_index = ir_foreach_get_index(foreach);
    if (ir_index != NULL)
    {
        add_to_func_frame(function, ir_index, false);
        *index = ir_variable_get_location(ir_index);
    }
    else
    {
        *index = iml_function_get_temp(function, iml_32b);
    }

    iml_function_add_operation(function,
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
    aggregate_type = dt_array(ir_expression_get_data_type(ir_aggregate));
    aggregate = iml_add_expression_eval(function, ir_aggregate, NULL, false);

    /* store length of the aggregate array in a temp variable */
    *length = iml_function_get_temp(function, iml_32b);
    iml_function_add_operation(function,
                               iml_operation_new(iml_getelm,
                                                 aggregate,
                                                 iml_constant_zero_32b(),
                                                 4,
                                                 *length));

    /* store pointer to the start of aggregate array in a temp variable */
    aggr_ptr = iml_function_get_temp(function, iml_ptr);
    iml_function_add_operation(function,
                               iml_operation_new(iml_getelm,
                                                 aggregate,
                                                 iml_constant_one_32b(),
                                                 4,
                                                 aggr_ptr));

    /* insert loop label */
    iml_function_add_operation(function, loop_head);

    /* generate iml to jump to loop head if index is less then length */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_jmpugreatereq,
                          *index,
                          *length,
                          iml_operation_get_operand(loop_end, 1)));

    /* generate iml for assigning value variable with aggregates element */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_getelm,
                          aggr_ptr,
                          *index,
                          dt_array_get_element_size(aggregate_type),
                          value));
}

void
iml_add_foreach_tail(iml_function_t *function,
                     ImlVariable *index,
                     ImlVariable *length,
                     iml_operation_t *loop_label)
{
    /* generate iml to advance index */
    iml_function_add_operation(function,
                               iml_operation_new(iml_add,
                                                 index,
                                                 iml_constant_one_32b(),
                                                 index));

    /* generate iml to jump to loop head */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_jmp,
                          iml_operation_get_operand(loop_label, 1)));
}

ImlOperand *
iml_add_foreach_range_head(iml_function_t *function,
                           IrVarValue *index,
                           IrExpression *lower_exp,
                           IrExpression *loop_test_exp,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end)
{
    assert(function);
    assert(IR_IS_VAR_VALUE(index));
    assert(IR_IS_EXPRESSION(lower_exp));
    assert(IR_IS_EXPRESSION(loop_test_exp));
    assert(loop_head);
    assert(loop_end);

    /* assign lower expression result to index variable */
    iml_add_assignment(function, ir_expression(index), lower_exp);

    /* insert loop head label */
    iml_function_add_operation(function, loop_head);

    /* evaluate upper expressions */
    ImlOperand *loop_test_res =
        iml_add_expression_eval(function, loop_test_exp, NULL, false);

    iml_function_add_operation(function,
        iml_operation_new(iml_jmpneq,
                          loop_test_res,
                          iml_constant_true(),
                          iml_operation_get_operand(loop_end, 1)));

    return loop_test_res;
}

void
iml_add_foreach_range_tail(iml_function_t *function,
                           IrExpression *inc_exp,
                           ImlOperand *head_temp_op,
                           iml_operation_t *loop_head,
                           iml_operation_t *loop_end)
{
    assert(function);
    assert(IR_IS_EXPRESSION(inc_exp));
    assert(IML_IS_OPERAND(head_temp_op));
    assert(loop_head);
    assert(loop_end);

    /* generate iml for loop's index increment expression */
    iml_add_expression_eval(function, inc_exp, NULL, true);

    /* generate jump to the loop's head */
    iml_function_add_operation(function,
        iml_operation_new(iml_jmp,
                          iml_operation_get_operand(loop_head, 1)));

    /* insert loop end label */
    iml_function_add_operation(function, loop_end);

    /* mark possible temp variables used in the loop as unused */
    iml_function_unused_oper(function, head_temp_op);
}

void
iml_add_for_head(iml_function_t *function,
                 IrExpression *test,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end)
{
    assert(function);
    assert(IR_IS_EXPRESSION(test) || test == NULL);
    assert(loop_head);
    assert(loop_end);

    /* insert loop head label */
    iml_function_add_operation(function, loop_head);

    if (test == NULL)
    {
        /* if there is no loop test expression, then we are done */
        return;
    }

    /* generate code for evaluating loop test expression */
    ImlVariable *test_res = iml_function_get_temp(function, iml_8b);
    iml_add_expression_eval(function, test, test_res, false);

    /* skip loop body if test expression evaluates to false */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_jmpneq,
                          test_res,
                          iml_constant_true(),
                          iml_operation_get_operand(loop_end, 1)));

    /* mark test expression result variable as unused */
    iml_function_unused_oper(function, iml_operand(test_res));
}

void
iml_add_for_tail(iml_function_t *function,
                 IrExpression *step,
                 iml_operation_t *loop_head,
                 iml_operation_t *loop_end)
{
    assert(function);
    assert(IR_IS_EXPRESSION(step) || step == NULL);
    assert(loop_head);
    assert(loop_end);

    if (step != NULL)
    {
        /* generate code to evaluate loop step expression */
        iml_add_expression_eval(function, step, NULL, true);
    }


    /* generate code to jump to loop start */
    iml_function_add_operation(
            function,
            iml_operation_new(iml_jmp,
                              iml_operation_get_operand(loop_head, 1)));

    /* insert loop end label */
    iml_function_add_operation(function, loop_end);
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
    else if (dt_is_enum(dt_type))
    {
        return dt_to_iml_type(dt_enum_get_base_type(dt_enum(dt_type)));
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

    binary_op_type_t optype = ir_binary_operation_get_operation(op);
    assert(optype == op_mult ||
           optype == op_division ||
           optype == op_modulo ||
           optype == op_less ||
           optype == op_greater ||
           optype == op_less_or_eq ||
           optype == op_greater_or_eq);

    operands_type =
       ir_expression_get_data_type(ir_binary_operation_get_left(op));

    if (dt_is_basic(operands_type))
    {
        return dt_basic_is_signed(dt_basic(operands_type));
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
        case op_plus:
            opcode = iml_add;
            break;
        case op_minus:
            opcode = iml_sub;
            break;
        case op_mult:
            opcode = is_signed_op(op) ? iml_smult : iml_umult;
            break;
        case op_division:
            opcode = is_signed_op(op) ? iml_sdiv : iml_udiv;
            break;
        case op_modulo:
            opcode = is_signed_op(op) ? iml_smod : iml_umod;
            break;
        case op_and:
            opcode = iml_and;
            break;
        case op_or:
            opcode = iml_or;
            break;
        case op_equal:
            opcode = iml_equal;
            break;
        case op_not_equal:
            opcode = iml_nequal;
            break;
        case op_less:
            opcode = is_signed_op(op) ? iml_sless : iml_uless;
            break;
        case op_greater:
            opcode = is_signed_op(op) ? iml_sgreater : iml_ugreater;
            break;
        case op_less_or_eq:
            opcode = is_signed_op(op) ? iml_slesseq : iml_ulesseq;
            break;
        case op_greater_or_eq:
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
        ir_expression_get_data_type(ir_expression(constant));

    if (dt_basic_is_int(exp_type))
    {
        type = iml_32b;
        v32 = ir_basic_constant_get_int(ir_basic_constant(constant));
    }
    else if (dt_basic_is_uint(exp_type))
    {
        type = iml_32b;
        v32 = ir_basic_constant_get_uint(ir_basic_constant(constant));
    }
    else if (dt_basic_is_short(exp_type))
    {
        type = iml_16b;
        v16 = ir_basic_constant_get_short(ir_basic_constant(constant));
    }
    else if (dt_basic_is_ushort(exp_type))
    {
        type = iml_16b;
        v16 = ir_basic_constant_get_ushort(ir_basic_constant(constant));
    }
    else if (dt_basic_is_byte(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_byte(ir_basic_constant(constant));
    }
    else if (dt_basic_is_ubyte(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_ubyte(ir_basic_constant(constant));
    }
    else if (dt_basic_is_char(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_char(ir_basic_constant(constant));
    }
    else if (dt_basic_is_bool(exp_type))
    {
        type = iml_8b;
        v8 = ir_basic_constant_get_bool(ir_basic_constant(constant));
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
iml_add_neg_op_eval(iml_function_t *function,
                    IrUnaryOperation *op,
                    ImlVariable *res)
{
    assert(function);
    assert(IR_IS_UNARY_OPERATION(op));

    ImlOperand *operand;
    iml_data_type_t iml_type;
    iml_opcode_t opcode;

    switch (ir_unary_operation_get_operation(op))
    {
        case op_arithm_neg:
            opcode = iml_ineg;
            break;
        case op_bool_neg:
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
        dt_to_iml_type(ir_expression_get_data_type(ir_expression(op)));

    if (res == NULL)
    {
        res = iml_function_get_temp(function, iml_type);
    }

    iml_function_add_operation(function,
                               iml_operation_new(opcode, operand, res));
    return iml_operand(res);
}

static ImlOperand *
iml_add_incdec_op_eval(iml_function_t *function,
                       IrUnaryOperation *op,
                       ImlVariable *res,
                       bool discard_result)
{
    assert(function);
    assert(IR_IS_UNARY_OPERATION(op));

    ImlOperand *operand;
    IrExpression *ir_operand;
    DtDataType *operand_type;
    iml_operation_t *mod_op;
    iml_opcode_t opcode;
    ImlConstant *mod_constant;
    bool copy_after_mod;

    switch (ir_unary_operation_get_operation(op))
    {
        case op_pre_inc:
            opcode = iml_add;
            copy_after_mod = true;
            break;
        case op_pre_dec:
            opcode = iml_sub;
            copy_after_mod = true;
            break;
        case op_post_inc:
            opcode = iml_add;
            copy_after_mod = false;
            break;
        case op_post_dec:
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
            res = iml_function_get_temp(function,
                                          dt_to_iml_type(operand_type));
        }
        copy_op = iml_operation_new(iml_copy, operand, res);
    }

    if (copy_after_mod)
    {
        /* pre (in/de)-crement operation */
        iml_function_add_operation(function, mod_op);
        if (copy_op != NULL)
        {
            iml_function_add_operation(function, copy_op);
        }
    }
    else
    {
        /* post (in/de)-crement operation */
        if (copy_op != NULL)
        {
            iml_function_add_operation(function, copy_op);
        }
        iml_function_add_operation(function, mod_op);
    }

    return iml_operand(res);
}

static ImlOperand *
iml_add_unary_op_eval(iml_function_t *function,
                      IrUnaryOperation *op,
                      ImlVariable *res,
                      bool discard_result)
{
    assert(function);
    assert(IR_IS_UNARY_OPERATION(op));

    switch (ir_unary_operation_get_operation(op))
    {
        case op_arithm_neg:
        case op_bool_neg:
            return iml_add_neg_op_eval(function, op, res);
        case op_pre_inc:
        case op_pre_dec:
        case op_post_inc:
        case op_post_dec:
            return iml_add_incdec_op_eval(function, op, res, discard_result);
        default:
            /* unexpected unary operation */
            assert(false);
    }
}

static ImlOperand *
iml_add_binary_op_eval(iml_function_t *function,
                       IrBinaryOperation *bin_op,
                       ImlVariable *res)
{
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
        dt_to_iml_type(ir_expression_get_data_type(ir_expression(bin_op)));

    if (res == NULL)
    {
        res = iml_function_get_temp(function, iml_type);
    }

    opcode = get_iml_opcode_binop(bin_op);

    iml_function_add_operation(function,
                               iml_operation_new(opcode, left, right, res));

    /* mark any temporary variables used as operands as unused */
    iml_function_unused_oper(function, left);
    iml_function_unused_oper(function, right);

    return iml_operand(res);
}

static iml_opcode_t
get_cast_opcode(DtDataType *src_type, DtDataType *target_type)
{
    if (dt_is_basic(target_type))
    {
        if (dt_is_basic(src_type))
        {
            basic_data_type_t src_bdt =
                dt_basic_get_data_type(dt_basic(src_type));
            basic_data_type_t trgt_bdt =
                dt_basic_get_data_type(dt_basic(target_type));

            return basic_cast_ops[trgt_bdt][src_bdt];
        }
        else if (dt_is_enum(src_type))
        {
            return get_cast_opcode(dt_enum_get_base_type(dt_enum(src_type)),
                                   target_type);
        }
        else
        {
            /* unexpected source data type */
            assert(false);
        }
    }
    else if (DT_IS_ARRAY(target_type))
    {
        return iml_copy;
    }
    else if (DT_IS_POINTER(target_type))
    {
        if (dt_is_basic(src_type))
        {
            basic_data_type_t src_bdt =
                           dt_basic_get_data_type(dt_basic(src_type));
            return basic_to_ptr_cast_ops[src_bdt];
        }
        else if (DT_IS_POINTER(src_type))
        {
            return iml_copy;
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
iml_add_cast_eval(iml_function_t *function,
                  IrCast *cast,
                  ImlVariable *dest)
{
    assert(function);
    assert(IR_IS_CAST(cast));

    iml_opcode_t cast_op =
        get_cast_opcode(ir_expression_get_data_type(ir_cast_get_value(cast)),
                        ir_cast_get_target_type(cast));

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
            iml_function_get_temp(
                function,
                dt_to_iml_type(ir_cast_get_target_type(cast)));
    }

    iml_function_add_operation(function,
                               iml_operation_new(cast_op, src, dest));
    return iml_operand(dest);
}

static ImlOperand *
iml_add_array_cell_eval(iml_function_t *function,
                        IrArrayCell *cell,
                        ImlVariable *res)
{
    assert(function);
    assert(IR_IS_ARRAY_CELL(cell));

    IrVariable *array_symb;
    ImlOperand *index_val;
    ImlVariable *src;
    DtDataType *array_type;
    DtDataType *element_type;
    guint size;

    array_symb = ir_array_cell_get_symbol(cell);
    array_type = ir_expression_get_data_type(ir_expression(array_symb));
    element_type = ir_expression_get_data_type(ir_expression(cell));

    /* generate code for array index evaluation */
    index_val = iml_add_expression_eval(function,
                                        ir_array_cell_get_index(cell),
                                        NULL,
                                        false);

    /* figure out where the array cell value should end up */
    if (res == NULL)
    {
        res = iml_function_get_temp(function, dt_to_iml_type(element_type));
    }

    src = ir_variable_get_location(array_symb);
    size = dt_data_type_get_size(element_type);

    if (DT_IS_STATIC_ARRAY_TYPE(array_type))
    {
        /* static array cell */
        iml_function_add_operation(function,
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

        /* generate code to store array pointer in a temp variable */
        ptr = iml_function_get_temp(function, iml_ptr);
        iml_function_add_operation(function,
                                   iml_operation_new(iml_getelm,
                                                     src,
                                                     iml_constant_one_32b(),
                                                     4,
                                                     ptr));

        /*
         * generate code to fetch the array cell value into
         * the destination variable
         */
        iml_function_add_operation(function,
                                   iml_operation_new(iml_getelm,
                                                     ptr,
                                                     index_val,
                                                     size,
                                                     res));
    }

    return iml_operand(res);
}

static ImlOperand *
iml_add_ptr_dref_eval(iml_function_t *function,
                      IrExpression *ref,
                      ImlVariable *res)
{
    assert(IR_IS_PTR_DREF(ref) || IR_IS_VAR_REF(ref));

    ImlOperand *ptr_exp;

    /* add iml to evaluate pointer expression */
    if (IR_IS_PTR_DREF(ref))
    {
        ptr_exp =
            iml_add_expression_eval(function,
                                    ir_ptr_dref_get_expression(IR_PTR_DREF(ref)),
                                    NULL,
                                    false);
    }
    else
    {
        assert(IR_IS_VAR_REF(ref));

        ptr_exp =
            iml_operand(
                ir_variable_get_location(ir_var_ref_get_var(ir_var_ref(ref))));
    }


    /* figure out where to store the result */
    if (res == NULL)
    {
        iml_data_type_t res_type;

        res_type =
            dt_to_iml_type(
                ir_expression_get_data_type(ir_expression(ref)));
        res = iml_function_get_temp(function, res_type);
    }

    /* add iml to fetch the value from the address to the result variable */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_get, ptr_exp, NULL, res));

    /* pointer expression operand not needed any more */
    iml_function_unused_oper(function, ptr_exp);

    return iml_operand(res);
}

static ImlOperand *
iml_add_address_of_eval(iml_function_t *function,
                        IrAddressOf *addr_of,
                        ImlVariable *res)
{
    assert(function);
    assert(IR_IS_ADDRESS_OF(addr_of));

    ImlVariable *addr_exp;

    /* add iml to evaluate &-operand expression */
    addr_exp =
        iml_variable(
            iml_add_expression_eval(function,
                                    ir_address_of_get_expression(addr_of),
                                    NULL,
                                    false));

    /* figure out where to store the result */
    if (res == NULL)
    {
        res = iml_function_get_temp(function, iml_ptr);
    }

    /* add iml to fetch the value from the address to the result variable */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_getaddr, addr_exp, res));

    /* pointer expression operand not needed any more */
    iml_function_unused_oper(function, iml_operand(addr_exp));

    return iml_operand(res);
}

static ImlOperand *
iml_add_conditional_eval(iml_function_t *function,
                         IrConditional *cond,
                         ImlVariable *res)
{
    assert(function);
    assert(IR_IS_CONDITIONAL(cond));

    ImlVariable *cond_var;

    /* figure out where to store the result */
    if (res == NULL)
    {
        iml_data_type_t res_type;

        res_type =
            dt_to_iml_type(
                ir_expression_get_data_type(ir_expression(cond)));
        res = iml_function_get_temp(function, res_type);
    }

    /* generate labels */
    IrModule *mod = iml_function_get_parent_module(function);
    char *skip_label = ir_module_gen_label(mod);
    char *end_label = ir_module_gen_label(mod);

    /* evaluate condition expression */
    cond_var =
        iml_variable(iml_add_expression_eval(function,
                                             ir_conditional_get_cond(cond),
                                             NULL,
                                             false));

    /* insert jump to false evaluation operation */
    iml_function_add_operation(function,
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
    iml_function_add_operation(function,
                               iml_operation_new(iml_jmp, end_label));

    /* insert skip label */
    iml_function_add_operation(function,
                               iml_operation_new(iml_label, skip_label));

    /* evaluate false expression */
    iml_add_expression_eval(function,
                            ir_conditional_get_false(cond),
                            res,
                            false);

    /* insert end label */
    iml_function_add_operation(function,
                               iml_operation_new(iml_label, end_label));

    /* free conditional variable if it was temporary */
    iml_function_unused_oper(function, iml_operand(cond_var));

    return iml_operand(res);
}

static ImlOperand *
iml_add_struct_member_eval(iml_function_t *function,
                           IrStructMember *struct_member,
                           ImlVariable *res)
{
    assert(function);
    assert(IR_IS_STRUCT_MEMBER(struct_member));

    IrExpression *ir_base;
    ImlOperand *base;
    ImlConstant *offset;

    ir_base = ir_struct_member_get_base(struct_member);

    if (IR_IS_VAR_REF(ir_base))
    {
        base =
            iml_operand(
                ir_variable_get_location(
                    ir_var_ref_get_var(ir_var_ref(ir_base))));
    }
    else
    {
        /* generate iml operation to evaluate struct base expression */
        base = iml_add_expression_eval(function, ir_base, NULL, false);
    }

    /* if base expression is of blob type, get a pointer to it */
    if (iml_operand_get_data_type(base) == iml_blob)
    {
        /* replace base operand with a pointer to it */
        ImlVariable *base_ptr = iml_function_get_temp(function, iml_ptr);

        iml_function_add_operation(
            function,
            iml_operation_new(iml_getaddr, base, base_ptr));

        iml_function_unused_oper(function, base);
        base = iml_operand(base_ptr);
    }

    /* figure out where to store the result */
    if (res == NULL)
    {
        iml_data_type_t res_type;

        res_type =
            dt_to_iml_type(
                ir_expression_get_data_type(ir_expression(struct_member)));
        res = iml_function_get_temp(function, res_type);
    }

    /* store rvalue at the base + offset */
    offset = iml_constant_new_32b(ir_struct_member_get_offset(struct_member));
    iml_function_add_operation(
        function,
        iml_operation_new(iml_get, base, offset, res));

    /* mark base operand as unused */
    iml_function_unused_oper(function, base);

    return iml_operand(res);
}

static ImlOperand *
iml_add_property_eval(iml_function_t *function,
                      IrProperty *prop,
                      ImlVariable *res)
{
    assert(function);
    assert(IR_IS_PROPERTY(prop));

    IrVariable *prop_exp;
    ImlVariable *src;

    /* only length property of variables implemented for now */
    assert(ir_property_get_id(prop) == ir_prop_length);
    assert(IR_IS_VAR_VALUE(ir_property_get_expression(prop)));

    prop_exp =
        ir_var_value_get_var(IR_VAR_VALUE(ir_property_get_expression(prop)));
    /* only length of dynamic array implemented for now */
    assert(DT_IS_ARRAY(ir_variable_get_data_type(prop_exp)));

    src = ir_variable_get_location(prop_exp);

    if (res == NULL)
    {
        res = iml_function_get_temp(function, iml_32b);
    }

    iml_function_add_operation(function,
                               iml_operation_new(iml_getelm,
                                                 src,
                                                 iml_constant_zero_32b(),
                                                 4,
                                                 res));

    return iml_operand(res);
}

static ImlOperand *
iml_add_array_literal_eval(iml_function_t *function,
                           IrArrayLiteral *expr,
                           ImlVariable *res)
{
    assert(ir_is_array_literal(expr));

    ImlVariable *ptr;
    ImlConstant *length;
    ImlConstant *size;
    iml_operation_t *op;
    guint element_size;

    /*
     * generate code to allocate memory for array literal
     */
    ptr = iml_function_get_temp(function, iml_ptr);
    size = iml_constant_new_32b(ir_array_literal_get_size(expr));

    iml_function_add_operation(function,
                               iml_operation_new_call_c("GC_malloc", ptr,
                                                        size, NULL));

    if (ir_expression_is_constant(ir_expression(expr)))
    {
        /*
         * generate code to copy literal data from data section to
         * allocated memory
         */

        const char *label = ir_literal_get_data_label(ir_literal(expr));
        assert(label != NULL); /* should be stored in data section */

        op = iml_operation_new_call_c("memcpy",
                                      NULL,
                                      ptr,
                                      iml_constant_new_ptr(label),
                                      size,
                                      NULL);
        iml_function_add_operation(function, op);
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
        dt = ir_expression_get_data_type(ir_expression(expr));
        assert(DT_IS_ARRAY(dt));
        element_size = dt_array_get_element_size(dt_array(dt));

        temp = iml_function_get_temp(function, iml_32b);

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
            iml_function_add_operation(function, op);
        }
    }

    if (res == NULL)
    {
        res = iml_function_get_temp(function, iml_blob, 8);
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
    iml_function_add_operation(function, op);

    op = iml_operation_new(iml_setelm,
                           ptr,
                           res,
                           iml_constant_one_32b(),
                           4);
    iml_function_add_operation(function, op);

    /* temp pointer is no longer used */
    iml_function_unused_oper(function, iml_operand(ptr));

    return iml_operand(res);
}

static ImlOperand *
iml_add_array_slice_eval(iml_function_t *function,
                         IrArraySlice *slice,
                         ImlVariable *res)
{
    ImlOperand *array;
    ImlOperand *start;
    ImlOperand *end;
    ImlOperand *length;
    ImlVariable *start_ptr;
    DtDataType *array_type;
    guint element_size;

    if (res == NULL)
    {
        res = iml_function_get_temp(function, iml_blob, 8);
    }

    /* figure out the element size of sliced array */
    array_type = ir_expression_get_data_type(ir_array_slice_get_array(slice));
    assert(DT_IS_ARRAY(array_type));
    element_size = dt_array_get_element_size(dt_array(array_type));

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
          iml_operand(
            iml_constant_new_32b(
                iml_constant_get_val_32b(iml_constant(end)) -
                iml_constant_get_val_32b(iml_constant(start))));
    }
    else
    {
        length = iml_operand(iml_function_get_temp(function, iml_32b));
        iml_function_add_operation(function,
                                   iml_operation_new(iml_sub,
                                                     end,
                                                     start,
                                                     length));
    }

    /* store length in the result blob */
    iml_function_add_operation(function,
                               iml_operation_new(iml_setelm,
                                                 length,
                                                 res,
                                                 iml_constant_zero_32b(),
                                                 4));

    /*
     * generate code to calculate pointer to the first element in the slice
     */

    /* get sliced array start pointer in */
    start_ptr = iml_function_get_temp(function, iml_ptr);
    if (DT_IS_STATIC_ARRAY_TYPE(array_type))
    {
        /* static array sliced */
        iml_function_add_operation(function,
                                   iml_operation_new(iml_getaddr,
                                                     array,
                                                     start_ptr));
    }
    else
    {
        /* dynamic array sliced */
        iml_function_add_operation(function,
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
                iml_operand(
                    iml_constant_new_32b(
                        element_size *
                            iml_constant_get_val_32b(iml_constant(start))));
        }
        else
        {
            iml_function_add_operation(
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
    iml_function_add_operation(function,
                               iml_operation_new(iml_add,
                                                 start_ptr,
                                                 length,
                                                 start_ptr));

    /* store slice start pointer in the result blob */
    iml_function_add_operation(function,
                               iml_operation_new(iml_setelm,
                                                 start_ptr,
                                                 res,
                                                 iml_constant_one_32b(),
                                                  4));

    /* mark operands as unused */
    iml_function_unused_oper(function, start);
    iml_function_unused_oper(function, end);
    iml_function_unused_oper(function, length);
    iml_function_unused_oper(function, iml_operand(start_ptr));

    return iml_operand(res);
}

static void
add_array_assignment(iml_function_t *function,
                     IrVariable *lvalue,
                     IrExpression *value)
{
    assert(function);
    assert(IR_IS_VARIABLE(lvalue));
    assert(IR_IS_EXPRESSION(value));

    ImlVariable *dest = ir_variable_get_location(lvalue);

    if (IR_IS_NULL(value))
    {
        /* handle the special case of null assignment */
        iml_function_add_operation(function,
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
add_static_array_assignment(iml_function_t *function,
                            IrVariable *lvalue,
                            IrExpression *value)
{
    ImlOperand *res_val;
    ImlVariable *array_var;
    DtArray *array_type;
    guint32 array_length;

    array_type =
            dt_array(ir_expression_get_data_type(ir_expression(lvalue)));

    array_var = ir_variable_get_location(lvalue);
    array_length =
        dt_static_array_get_length(DT_STATIC_ARRAY(array_type));

    if (dt_is_basic(ir_expression_get_data_type(value)))
    {
        /* assignment of basic types to static array */

        res_val = iml_add_expression_eval(function, value, NULL, false);
        iml_function_add_operation(function,
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

        ImlOperand *rvalue;
        ImlVariable *dest_ptr;
        ImlVariable *src_ptr;
        ImlConstant *memcpy_size;

        /* generate code to evaluate the rvalue */
        rvalue = iml_add_expression_eval(function, value, NULL, false);

        /* generate code to get pointer to the array */
        dest_ptr = iml_function_get_temp(function, iml_ptr);
        iml_function_add_operation(function,
                                   iml_operation_new(iml_getaddr,
                                                     array_var,
                                                     dest_ptr));

        /* generate code to get pointer to rvalue */
        src_ptr = iml_function_get_temp(function, iml_ptr);
        iml_function_add_operation(function,
                                   iml_operation_new(iml_getelm,
                                                     rvalue,
                                                     iml_constant_one_32b(),
                                                     4,
                                                     src_ptr));

        /* generate code to copy rvalue to the array */
        memcpy_size =
            iml_constant_new_32b(array_length *
                                 dt_array_get_element_size(array_type));
        iml_function_add_operation(
                function,
                iml_operation_new_call_c("memcpy",
                                         NULL,
                                         dest_ptr,
                                         src_ptr,
                                         memcpy_size,
                                         NULL));

        /* mark temporaries as unused */
        iml_function_unused_oper(function, rvalue);
        iml_function_unused_oper(function, iml_operand(dest_ptr));
        iml_function_unused_oper(function, iml_operand(src_ptr));
    }
}

static void
add_array_cell_assignment(iml_function_t *function,
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
    array_type = ir_expression_get_data_type(ir_expression(array_symb));
    element_type = ir_expression_get_data_type(ir_expression(lvalue));

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
        iml_function_add_operation(function,
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

        /* generate code to store array pointer in a temp variable */
        ptr = iml_function_get_temp(function, iml_ptr);
        iml_function_add_operation(function,
                                   iml_operation_new(iml_getelm,
                                                     dest,
                                                     iml_constant_one_32b(),
                                                     4,
                                                     ptr));

        /* generate code to store value in the array cell */
        iml_function_add_operation(function,
                                   iml_operation_new(iml_setelm,
                                                     res_val,
                                                     ptr,
                                                     index_val,
                                                     size));
    }
}

static void
add_array_slice_assignment(iml_function_t *function,
                           IrArraySlice *lvalue,
                           IrExpression *value)
{
    assert(function);
    assert(IR_IS_ARRAY_SLICE(lvalue));
    assert(IR_IS_EXPRESSION(value));

    DtDataType *array_type;
    guint element_size;
    ImlOperand *src;
    ImlOperand *dest;
    ImlVariable *src_ptr;
    ImlVariable *dest_ptr;
    ImlVariable *length;

    /* figure out the byte size of array elements */
    array_type = ir_expression_get_data_type(value);
    assert(DT_IS_ARRAY(array_type));
    element_size = dt_array_get_element_size(dt_array(array_type));

    /* generate code to evaluate left and right values */
    dest =
        iml_add_expression_eval(function, ir_expression(lvalue), NULL, false);
    src =
        iml_add_expression_eval(function, ir_expression(value), NULL, false);

    /* store source pointer in temp variable */
    src_ptr = iml_function_get_temp(function, iml_ptr);
    iml_function_add_operation(function,
                               iml_operation_new(iml_getelm,
                                                 src,
                                                 iml_constant_one_32b(),
                                                 4,
                                                 src_ptr));

    /* generate code to calculate the length in bytes of the array to copy */
    length = iml_function_get_temp(function, iml_32b);
    iml_function_add_operation(function,
                               iml_operation_new(iml_getelm,
                                                 src,
                                                 iml_constant_zero_32b(),
                                                 4,
                                                 length));

    /* multiply length with element size, if needed */
    if (element_size > 1)
    {
        iml_function_add_operation(
                function,
                iml_operation_new(iml_umult,
                                  length,
                                  iml_constant_new_32b(element_size),
                                  length));
    }

    /* store destination pointer in temp variable */
    dest_ptr = iml_function_get_temp(function, iml_ptr);
    iml_function_add_operation(function,
                               iml_operation_new(iml_getelm,
                                                 dest,
                                                 iml_constant_one_32b(),
                                                 4,
                                                 dest_ptr));


    /* generate code to copy memory from source array to destination */
    iml_function_add_operation(
        function,
        iml_operation_new_call_c("memcpy", NULL, dest_ptr, src_ptr,
                                 length, NULL));
}

static void
add_pointer_assignment(iml_function_t *function,
                       IrVariable *lvalue,
                       IrExpression *value)
{
    assert(function);
    assert(IR_IS_VARIABLE(lvalue));
    assert(IR_IS_EXPRESSION(value));

    ImlVariable *dest = ir_variable_get_location(lvalue);

    if (IR_IS_NULL(value))
    {
        /* handle the special case of null assignment */
        iml_function_add_operation(
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
add_struct_assignment(iml_function_t *function,
                      IrVariable *lvalue,
                      IrExpression *value)
{
    assert(function);
    assert(IR_IS_VARIABLE(lvalue));
    assert(IR_IS_EXPRESSION(value));

    /* only assignment of struct literals implemented */
    assert(ir_is_struct_literal(value));

    ImlVariable *lval = ir_variable_get_location(lvalue);
    ImlVariable *lval_ptr = iml_function_get_temp(function, iml_ptr);

    /* generate code to store pointer to the stuct in a temp variable */
    iml_function_add_operation(
        function,
        iml_operation_new(iml_getaddr, lval, lval_ptr));

    /*
     *  generate code to call memcpy to copy struct literal from
     *  .data section address to struct's memory
     */
    char *label = ir_literal_get_data_label(ir_literal(value));
    ImlConstant *size = iml_constant_new_32b(iml_variable_get_size(lval));

    iml_function_add_operation(
        function,
        iml_operation_new_call_c("memcpy",
                                 NULL,
                                 lval_ptr,
                                 iml_constant_new_ptr(label),
                                 size,
                                 NULL));

    /* release temp variable */
    iml_function_unused_oper(function, iml_operand(lval_ptr));
}

static void
add_ptr_dref_assignment(iml_function_t *function,
                        IrExpression *lvalue,
                        IrExpression *value)
{
    assert(function);
    assert(IR_IS_PTR_DREF(lvalue) || IR_IS_VAR_REF(lvalue));
    assert(IR_IS_EXPRESSION(value));

    ImlOperand *lval;
    ImlOperand *rval;

    /* generate iml operation to evaluate pointer expression */
    if (IR_IS_PTR_DREF(lvalue))
    {
        lval = iml_add_expression_eval(function,
                                       ir_ptr_dref_get_expression(
                                                         IR_PTR_DREF(lvalue)),
                                       NULL,
                                       false);
    }
    else
    {
        assert(IR_IS_VAR_REF(lvalue));
        lval =
           iml_operand(
             ir_variable_get_location(ir_var_ref_get_var(ir_var_ref(lvalue))));
    }
    /* generate iml operations to evaluate rvalue */
    rval = iml_add_expression_eval(function, value, NULL, false);

    /* add iml to write rvalue to the destination address */
    iml_function_add_operation(function,
                               iml_operation_new(iml_set, rval, lval, NULL));

    /* mark left and right operands as unused */
    iml_function_unused_oper(function, lval);
    iml_function_unused_oper(function, rval);
}

static void
add_struct_member_assignment(iml_function_t *function,
                             IrStructMember *lvalue,
                             IrExpression *value)
{
    assert(function);
    assert(IR_IS_STRUCT_MEMBER(lvalue));
    assert(IR_IS_EXPRESSION(value));

    IrExpression *ir_base;
    ImlOperand *base;
    ImlOperand *rval;
    ImlConstant *offset;

    ir_base = ir_struct_member_get_base(lvalue);
    if (IR_IS_VAR_REF(ir_base))
    {
        base =
            iml_operand(
                ir_variable_get_location(
                    ir_var_ref_get_var(ir_var_ref(ir_base))));
    }
    else
    {
        /* generate iml operation to evaluate struct base expression */
        base = iml_add_expression_eval(function, ir_base, NULL, false);
    }

    /* if base expression is of blob type, get a pointer to it */
    if (iml_operand_get_data_type(base) == iml_blob)
    {
        /* replace base operand with a pointer to it */
        ImlVariable *base_ptr = iml_function_get_temp(function, iml_ptr);

        iml_function_add_operation(
            function,
            iml_operation_new(iml_getaddr, base, base_ptr));

        iml_function_unused_oper(function, base);
        base = iml_operand(base_ptr);
    }

    /* generate iml operations to evaluate rvalue */
    rval = iml_add_expression_eval(function, value, NULL, false);

    /* store rvalue at the base + offset */
    offset = iml_constant_new_32b(ir_struct_member_get_offset(lvalue));
    iml_function_add_operation(
        function,
        iml_operation_new(iml_set, rval, base, offset));

    /* mark operands as unused */
    iml_function_unused_oper(function, base);
    iml_function_unused_oper(function, rval);
}
