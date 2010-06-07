#include "x86_func_call.h"
#include "types.h"
#include "ir_function_def.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
compile_d_func_call(x86_comp_params_t *params,
                    IrFunctionCall *func_call,
                    sym_table_t *sym_table,
                    bool retain_return_value);

static void
compile_c_func_call(x86_comp_params_t *params,
                    IrFunctionCall *func_call,
                    sym_table_t *sym_table,
                    bool retain_return_value);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * @param retain_return_value if true and called function is not void, the return 
 *                            value of the function is pushed on the stack,
 *                            if false, the return value is discarded
 */
void
x86_compile_func_call(x86_comp_params_t *params,
                      IrFunctionCall *func_call,
                      sym_table_t *sym_table,
                      bool retain_return_value)
{
    assert(IR_IS_FUNCTION_CALL(func_call));

    switch (ir_function_call_get_linkage(func_call))
    {
        case ir_d_linkage:
            compile_d_func_call(params,
                                func_call,
                                sym_table,
                                retain_return_value);
            break;
        case ir_c_linkage:
            compile_c_func_call(params,
                                func_call,
                                sym_table,
                                retain_return_value);
            break;
        default:
            /* unexpected linkage type */
            assert(false);
    }
}


/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
compile_d_func_call(x86_comp_params_t *params,
                    IrFunctionCall *func_call,
                    sym_table_t *sym_table,
                    bool retain_return_value)
{
    assert(params);
    assert(IR_IS_FUNCTION_CALL(func_call));
    assert(sym_table);

    GSList *i;
    DtDataType *func_data_type;
    IrFunctionDef *callee_def;
    bool last_param_in_reg = false;
    int args_size_on_stack = 0;

    callee_def =
        IR_FUNCTION_DEF(
             sym_table_get_symbol(sym_table,
                                  ir_function_call_get_name(func_call), NULL));

    /*
     * generate code that evaluates all function call arguments expression
     * and pushes them onto stack
     */
    i = ir_function_call_get_arguments(func_call);
    for (; i != NULL; i = g_slist_next(i))
    {
        x86_compile_expression(params, i->data, sym_table);
    }

    /*
     * calculate the size call arguments are using on stack,
     * also note if last argument is passed on stack or via a register
     */
    i = ir_function_def_get_parameters(callee_def);
    for (; i != NULL; i = g_slist_next(i))
    {
        DtDataType *param_type;

        if (IR_IS_VARIABLE(i->data)) {
            param_type = ir_variable_get_data_type(IR_VARIABLE(i->data));
        }
        else
        {
            /* unnamed parameter */
            param_type = i->data;
        }

        if (g_slist_next(i) != NULL)
        {
            args_size_on_stack += dt_data_type_get_size(param_type);
        }
        else
        {
            /* last argument */
            last_param_in_reg = x86_in_reg_as_last_func_arg(param_type);
            if (!last_param_in_reg)
            {
                args_size_on_stack += dt_data_type_get_size(param_type);
            }
        }
        args_size_on_stack = x86_inc_to_word_boundary(args_size_on_stack);
    }

    /* pop last argument into eax if needed */
    if (last_param_in_reg)
    {
        fprintf(params->out,
                "# put last argument into eax\n"
                "    popl %%eax\n");
    }

    fprintf(params->out,
            "# invoke function\n"
            "    call %s\n",
            ir_function_def_get_mangled_name(callee_def));

    if (args_size_on_stack > 0)
    {
        fprintf(params->out,
                "    addl $%d, %%esp #pop call args from stack\n",
                args_size_on_stack);
    }

    func_data_type = ir_expression_get_data_type(IR_EXPRESSION(func_call));

    if (DT_IS_ENUM_TYPE(func_data_type))
    {
        func_data_type =
            dt_enum_type_get_base_type(DT_ENUM_TYPE(func_data_type));
    }

    if (!DT_IS_BASIC_TYPE(func_data_type))
    {
        /*
         * only calling function with enum or
         * basic type return values implemented
         */
        assert(false);
    }

    if (retain_return_value)
    {
        switch (dt_basic_type_get_data_type(DT_BASIC_TYPE(func_data_type)))
        {
           case void_type:
               /* nop */
               break;
           case bool_type:
           case char_type:
           case int_type:
           case uint_type:
               fprintf(params->out,
                       "# push function return value on the stack\n"
                       "    push %%eax\n");
               break;
           default:
               /* unexpected basic data type */
               assert(false);
        }
    }
}

static void
compile_c_func_call(x86_comp_params_t *params,
                    IrFunctionCall *func_call,
                    sym_table_t *sym_table,
                    bool retain_return_value)
{
    assert(params);
    assert(IR_IS_FUNCTION_CALL(func_call));
    assert(sym_table);

    GSList *i;
    DtDataType *func_data_type;
    int args_size_on_stack = 0;

    i = ir_function_call_get_arguments(func_call);

    /*
     * in x86 c calling convention, function arguments are
     * pushed in reversed order, so we need to make copy of
     * arguments list and reverse it
     */
    i = g_slist_copy(i);
    i = g_slist_reverse(i);
    for (; i != NULL; i = g_slist_next(i))
    {
        x86_compile_expression(params, i->data, sym_table);
        args_size_on_stack +=
            x86_get_expression_storage_size(IR_EXPRESSION(i->data));
    }
    g_slist_free(i);

    fprintf(params->out,
            "# invoke function\n"
            "    call %s\n",
            ir_function_call_get_name(func_call));

    if (args_size_on_stack > 0)
    {
        fprintf(params->out,
                "# remove function call arguments from the stack\n"
                "    addl $%d, %%esp\n",
                args_size_on_stack);    
    }

    func_data_type = ir_expression_get_data_type(IR_EXPRESSION(func_call));

    if (!DT_IS_BASIC_TYPE(func_data_type))
    {
        /* calling function with non-basic return type is not implemented */
        assert(false);
    }

    if (retain_return_value)
    {
        switch (dt_basic_type_get_data_type(DT_BASIC_TYPE(func_data_type)))
        {
           case void_type:
               /* nop */
               break;
           case bool_type:
           case int_type:
               fprintf(params->out,
                       "# push function return value on the stack\n"
                       "    push %%eax\n");
               break;
           default:
               /* unexpected basic data type */
               assert(false);
        }
    }
}


