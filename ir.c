#include <stdbool.h>

#include "ir.h"
#include "sym_table.h"
#include "string.h"
#include "stdio.h"

#include <assert.h>

/*------------------*
 * type definitions *
 *------------------*/

struct ir_function_def_s
{
    char *name;
    sym_table_t *parameters;
    sym_table_t *local;
    ast_node_t *body;
};

struct ir_compile_unit_s
{
    sym_table_t *global_sym_table;
};

struct ir_symbol_s
{
    ir_symbol_type_t type;
    ir_symbol_data_t data;
};


/*--------------------*
 * exported functions *
 *--------------------*/

ir_function_def_t*
new_ir_function_def()
{
    ir_function_def_t *func_def;

    func_def = g_malloc(sizeof(*func_def));

    func_def->parameters = sym_table_new(NULL);
    func_def->local = sym_table_new(NULL);
    func_def->body = NULL;

    return func_def;
}

int
ir_function_def_add_parameter(ir_function_def_t* func,
                              ast_var_decl_t *var)
{
    assert(func);
    assert(var);

    ir_symbol_t *symb;

    symb = new_ir_symbol_variable(var);

    int res = sym_table_add_symbol(func->parameters,
                                   var->name, symb);
    if (res == -1) {
        printf("reclaration of function parameter '%s'\n", var->name);
        ir_symbol_del(symb);
        return -1;
    }
    
    return 0;
}

int
ir_function_def_add_local_var(ir_function_def_t* func,
                              ast_var_decl_t *var)
{
    assert(func);
    assert(var);

    ir_symbol_t *symb;

    symb = new_ir_symbol_variable(var);

    int res = sym_table_add_symbol(func->local,
                                   var->name, symb);
    if (res == -1) {
        printf("reclaration of local variable '%s'\n", var->name);
        ir_symbol_del(symb);
        return -1;
    }
    
    return 0;
}

void
ir_function_def_set_name(ir_function_def_t* func,
                     const char *name)
{
    assert(func);
    assert(name);

    func->name = strdup(name);
}

void
ir_function_def_set_body(ir_function_def_t* func,
                         ast_node_t *body)
{
    assert(func);
    assert(body);

    func->body = body;
}

int
ir_compile_unit_add_function(ir_compile_unit_t *compile_unit,
                             ir_function_def_t *function_def)
{
    ir_symbol_t *symb;
    int res;

    symb = new_ir_symbol_function(function_def);

    res = sym_table_add_symbol(compile_unit->global_sym_table,
                               function_def->name, symb);
    if (res == -1) {
        printf("reclaration of function '%s'\n", function_def->name);
        ir_symbol_del(symb);
        return -1;
    }
    
    return 0;
}

ir_compile_unit_t*
new_ir_compile_unit()
{
    ir_compile_unit_t *comp_unit;

    comp_unit = g_malloc(sizeof(*comp_unit));
    comp_unit->global_sym_table = sym_table_new(NULL);

    return comp_unit;
}


ir_symbol_t*
new_ir_symbol_function(ir_function_def_t *function)
{
    ir_symbol_t *symb;

    symb = g_malloc(sizeof(*symb));
    symb->type = ir_function_symb;
    symb->data.function = function;

    return symb;
}

ir_symbol_t*
new_ir_symbol_variable(ast_var_decl_t *variable)
{
    ir_symbol_t *symb;

    symb = g_malloc(sizeof(*symb));
    symb->type = ir_variable_symb;
    symb->data.variable = variable;

    return symb;
}


void
ir_symbol_del(ir_symbol_t *sym)
{
    free(sym);
}
