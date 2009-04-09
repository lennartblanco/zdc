#include <stdbool.h>

#include "x86.h"
#include "x86_frame_offset.h"
#include "x86_reg_location.h"
#include "ast_basic_type.h"
#include "ast_static_array_type.h"
#include "ast_variable_declaration.h"
#include "ir_variable.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

static void
x86_prelude(FILE *out, const char *source_file);

static void
x86_compile_function_def(FILE *out, IrFunction *func);

static int
x86_code_block_assign_addrs(int first_num,
                            IrCodeBlock *code_block);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream,
             const char *source_file)
{
    sym_table_t *global_sym_table;
    GList *symbols_list;
    GList *p;

    x86_prelude(out_stream, source_file);
    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    symbols_list = sym_table_get_all_symbols(global_sym_table);
    for (p = symbols_list; p != NULL; p = g_list_next(p))
    {
        if (IR_IS_FUNCTION(p->data))
        {
            x86_compile_function_def(out_stream, p->data);
        }
        else
        {
            /* unexpected symbol type */
            assert(false);
        }
    }
    g_list_free(symbols_list);

        
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/

static void
x86_prelude(FILE *out, const char *source_file)
{
    fprintf(out,
            "    .file \"%s\"\n"
            "    .text\n",
            source_file);
}

static void
x86_compile_function_def(FILE *out, IrFunction *func)
{
    GSList *i;
    char *func_name;
    int len;
    int addr;
    int stack_size;
    sym_table_t *param_symbols;

    func_name = ir_function_get_name(func);
    /* generate function symbol declaration and function entry point label */
    fprintf(out,
            ".globl %s\n"
            "    .type %s, @function\n"
            "%s:\n"
            "    pushl %%ebp\n"
            "    movl %%esp, %%ebp\n",
            func_name, func_name, func_name);

    /* assign locations to function parameter variables */
    i = ir_function_get_parameters(func);
    param_symbols = ir_function_get_parameter_symbols(func);

    len = g_slist_length(i);
    addr = len * 4;
    for (; i != NULL; i = g_slist_next(i))
    {
        AstVariableDeclaration *var = i->data;


        /* convert ast variable declaration to IR variable object */
        IrVariable *variable =
            IR_VARIABLE(sym_table_get_symbol(param_symbols,
                                 ast_variable_declaration_get_name(var)));


        /* assign variable number */
        if (addr >= 8)
        {
            ir_variable_set_location(variable, 
                                     G_OBJECT(x86_frame_offset_new(addr)));
        }
        else
        {
            ir_variable_set_location(variable, 
                                     G_OBJECT(x86_reg_location_new(eax_reg)));

        }
        addr -= 4;
    }

    stack_size = x86_code_block_assign_addrs(-4, ir_function_get_body(func));
    printf("%s stack size %d\n", ir_function_get_name(func), stack_size);
    fprintf(out,
            "    subl $%d, %%esp\n",
            -stack_size);

}

static int
x86_get_variable_storage_size(IrVariable *variable)
{
    assert(variable);
    assert(IR_IS_VARIABLE(variable));

    AstDataType *variable_type;

    variable_type = ir_variable_get_data_type(variable);
    if (XDP_IS_AST_BASIC_TYPE(variable_type))
    {
        AstBasicType *basic_type = XDP_AST_BASIC_TYPE(variable_type);

        switch (ast_basic_type_get_data_type(basic_type))
        {
            case int_type:
                return 4;
            case bool_type:
                return 1;
            default:
                /* unexpected basic data type */
                assert(false);
        }
    }
    else if (XDP_IS_AST_STATIC_ARRAY_TYPE(variable_type))
    {
        AstStaticArrayType *array_type;
        int len;

        array_type = XDP_AST_STATIC_ARRAY_TYPE(variable_type);
        len = ast_static_array_type_get_length(array_type);
        switch (ast_static_array_type_get_data_type(array_type))
        {
            case int_type:
                return len * 4;
            case bool_type:
                return len * 1;
            default:
                /* unexpected basic data type */
                assert(false);
        }
    }
    else
    {
        /* unexpected data type */
        assert(false);
    }
    /* we should not get here */
    assert(false);
    return 0;
}

static int
x86_code_block_assign_addrs(int first_num,
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

        ir_variable_set_location(var, G_OBJECT(x86_frame_offset_new(num)));
        num -= x86_get_variable_storage_size(var);


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
            vars = x86_code_block_assign_addrs(num, j->data);
            /* 
             * keep track if highest local number slot assigned 
             * in our sub-blocks 
             */
        }

        if (vars < last_num)
        {
            last_num = vars;
        }
    }

    return last_num;
}

