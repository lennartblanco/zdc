#include "x86_func_call.h"
#include "dt_basic_type.h"

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

    int arg_num;
    GSList *i;
    DtDataType *func_data_type;

    i = ir_function_call_get_arguments(func_call);
    arg_num = g_slist_length(i);
    for (; i != NULL; i = g_slist_next(i))
    {
        x86_compile_expression(params, i->data, sym_table);
    }

    if (arg_num > 0)
    {
        fprintf(params->out,
                "# put last argument into eax\n"
                "    popl %%eax\n");
    }

    fprintf(params->out,
            "# invoke function\n"
            "    call %s\n",
            ir_function_call_get_name(func_call));

    if (arg_num > 1)
    {
        fprintf(params->out,
                "# remove function call arguments from the stack\n"
                "    addl $%d, %%esp\n",
                (arg_num - 1) * 4);
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
    }

    fprintf(params->out,
            "# invoke function\n"
            "    call %s\n",
            ir_function_call_get_name(func_call));

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


