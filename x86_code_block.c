#include "x86_code_block.h"
#include "x86_func_call.h"
#include "x86_while.h"
#include "x86_foreach.h"
#include "x86_if_else.h"
#include "x86_frame_offset.h"
#include "types.h"
#include "ir_return.h"
#include "ir_assigment.h"
#include "ir_if_else.h"
#include "ir_array.h"
#include "ir_array_literal.h"
#include "ir_scalar.h"
#include "ir_array_slice.h"
#include "ir_uint_constant.h"


#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

/**
 * Generate code for initialization of an variable.
 *
 * @param params       compilation pass parameters handle
 * @param variable     variable which should be initialized
 * @param sym_table    symbols where the array is defined
 */
static void
x86_compile_variable_initializer(x86_comp_params_t *params,
                                 IrVariable *variable,
                                 sym_table_t *sym_table);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_compile_code_block(x86_comp_params_t *params,
                       IrCodeBlock *code_block)
{
    GList *symbols_list;
    GList *l;
    sym_table_t *locals;

    locals = ir_code_block_get_symbols(code_block);
    /*
     * generate code to initialize all new variables in this code block
     */
    symbols_list = sym_table_get_all_symbols(locals);
    for (l = symbols_list; l != NULL; l = g_list_next(l))
    {
        x86_compile_variable_initializer(params,
                                         l->data,
                                         locals);
    }
    g_list_free(symbols_list);

    /*
     * generate code for the statments in this code block
     */
    GSList *stmts = ir_code_block_get_statments(code_block);
    for (;stmts != NULL; stmts = g_slist_next(stmts))
    {
        IrStatment *statment = IR_STATMENT(stmts->data);

        if (IR_IS_RETURN(statment))
        {
            IrReturn *ret = IR_RETURN(statment);
            IrExpression *return_val = ir_return_get_return_value(ret);
            if (return_val != NULL)
            {
                x86_compile_expression(params, return_val, locals);
                fprintf(params->out,
                        "    popl %%eax\n");
            }
            fprintf(params->out,
                    "    leave\n"
                    "    ret\n");
        }
        else if (IR_IS_FUNCTION_CALL(statment))
        {
            x86_compile_func_call(params, 
                                  IR_FUNCTION_CALL(statment),
                                  locals,
                                  false);
        }
        else if (IR_IS_ASSIGMENT(statment))
        {
            IrAssigment *assig = IR_ASSIGMENT(statment);
            x86_compile_assigment(params, assig, locals);
        }
        else if (IR_IS_CODE_BLOCK(statment))
        {
            x86_compile_code_block(params,
                                   IR_CODE_BLOCK(statment));
        }
        else if (IR_IS_IF_ELSE(statment))
        {
            x86_compile_if_else(params,
                                IR_IF_ELSE(statment),
                                locals);
        }
        else if (IR_IS_WHILE(statment))
        {
            x86_compile_while(params,
                              IR_WHILE(statment),
                              locals);
        }
        else if (IR_IS_FOREACH(statment))
        {
            x86_compile_foreach(params,
                                IR_FOREACH(statment),
                                locals);
        }
        else
        {
            /* unexpected statment type */
            printf("%s\n", g_type_name(G_TYPE_FROM_INSTANCE(stmts->data)));
            assert(false);
        }
    }
}

int
x86_code_block_assign_addrs(x86_comp_params_t *params,
                            int first_num,
                            IrCodeBlock *code_block)
{
    sym_table_t *symbols;
    GList *symbols_list;
    GList *i;
    GSList *j;
    int num = first_num;
    int last_num;

    /*
     * assign number to this code block's local variables
     */
    symbols = ir_code_block_get_symbols(code_block);

    symbols_list = sym_table_get_all_symbols(symbols);
    for (i = symbols_list; i != NULL; i = g_list_next(i))
    {
        IrVariable *var = i->data;

        if (!IR_IS_VARIABLE(var))
        {
            /* skip non-variables in symbol table */
            continue;
        }

        num -= x86_get_expression_storage_size(IR_EXPRESSION(var));
        ir_variable_set_location(var, G_OBJECT(x86_frame_offset_new(num)));
        fprintf(params->out, "# variable '%s' location %d\n",
                ir_variable_get_name(var), num);

    }
    g_list_free(symbols_list);
    last_num = num;

    /*
     * assign numbers to children code block's variables
     */
    j = ir_code_block_get_statments(code_block);
    for (; j != NULL; j = g_slist_next(j))
    {
        int vars = 0;
        if (IR_IS_CODE_BLOCK(j->data))
        {
            vars = x86_code_block_assign_addrs(params, num, j->data);
        }
        else if (IR_IS_IF_ELSE(j->data))
        {
            vars = x86_if_else_assign_addrs(params, num, j->data);
        }
        else if (IR_IS_FOREACH(j->data))
        {
            vars = x86_foreach_assign_addrs(params, num, j->data);
        }
        else if (IR_IS_WHILE(j->data))
        {
            vars = x86_while_assign_addrs(params, num, j->data);
        }

        /* 
         * keep track of lowers frame offset assigned 
         * in our sub-blocks 
         */
        if (vars < last_num)
        {
            last_num = vars;
        }
    }

    return last_num;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
x86_compile_variable_initializer(x86_comp_params_t *params,
                                 IrVariable *variable,
                                 sym_table_t *sym_table)
{
    DtDataType *var_type = ir_variable_get_data_type(variable);
    IrExpression *var_init = ir_variable_get_initializer(variable);
    IrLvalue *lval;
    IrAssigment *assigment;

    if (DT_IS_BASIC_TYPE(var_type) || DT_IS_ENUM_TYPE(var_type))
    {
        if (var_init == NULL)
        {
            /* construct default initializer value for the type */
            var_init = dt_data_type_get_init(var_type);
        }

        lval = IR_LVALUE(ir_scalar_new(ir_variable_get_name(variable), 0));
        ir_lvalue_set_variable(lval, variable);
    }
    else if (DT_IS_STATIC_ARRAY_TYPE(var_type))
    {
        DtStaticArrayType *var_array_type = DT_STATIC_ARRAY_TYPE(var_type);
        IrUintConstant *start_idx;
        IrUintConstant *end_idx;

        /*
         * build an lvalue that represent a slice over the whole array
         */
        start_idx = ir_uint_constant_new(0, 0);
        end_idx =
            ir_uint_constant_new(
                dt_static_array_type_get_length(var_array_type), 0);

        lval = IR_LVALUE(ir_array_slice_new(ir_variable_get_name(variable),
                                            IR_EXPRESSION(start_idx),
                                            IR_EXPRESSION(end_idx),
                                            0));
        ir_lvalue_set_variable(lval, variable);

        if (var_init == NULL)
        {
            /*
             * construct default initializer value for 
             * the arrays element type
             */
            /* only static arrays over basic types are supported */
            var_init =
                dt_data_type_get_init(
                    dt_static_array_type_get_data_type(var_array_type));
        }
    }
    else if (DT_IS_ARRAY_TYPE(var_type))
    {
         int offset;

         if (var_init == NULL)
         {
             offset =
                 x86_frame_offset_get_offset(
                     X86_FRAME_OFFSET(ir_variable_get_location(variable)));

             fprintf(params->out,
                     "    # initilize dynamic array to length 0\n"
                     "    movl $0, %d(%%ebp)\n"
                     "    movl $0, %d(%%ebp)\n",
                     offset, offset + 4);
             return;
         }
         lval = IR_LVALUE(ir_array_new(variable));
    }
    else
    {
        /* unexpected data type */
        assert(false);
    }

    /* generate assigment code of the initializer value to the variable */
    assigment = ir_assigment_new(lval, var_init, 0);
    x86_compile_assigment(params, assigment, sym_table);

    /* clean-up temp assigment statment objects */
    g_object_unref(assigment);
    g_object_unref(lval);
}


