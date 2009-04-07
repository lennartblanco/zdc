#include <stdbool.h>

#include "x86.h"
#include "x86_frame_offset.h"
#include "x86_reg_location.h"
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

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
x86_gen_code(IrCompileUnit *comp_unit,
             FILE *out_stream,
             const char *source_file)
{
    sym_table_t *global_sym_table;
    GList *p;

    x86_prelude(out_stream, source_file);
    global_sym_table = ir_compile_unit_get_symbols(comp_unit);

    p = sym_table_get_all_symbols(global_sym_table);
    for (; p != NULL; p = g_list_next(p))
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
    g_list_free(p);

        
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
    sym_table_t *param_symbols;
    static int cntr = 0;

    func_name = ir_function_get_name(func);
    /* generate function symbol declaration and function entry point label */
    fprintf(out,
            ".globl %s\n"
            "    .type %s, @function\n"
            "%s:\n"
            "    pushl %%ebp\n"
            "    movl %%esp, %%ebp\n",
            func_name, func_name, func_name);

    /* generate dummy body */
    fprintf(out,
            "    movl $%d, %%eax\n"
            "    ret\n",
            (cntr++));

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
}

