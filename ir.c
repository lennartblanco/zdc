#include <stdbool.h>

#include "ir.h"
#include "sym_table.h"
#include "string.h"
#include "stdio.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/ 
struct ir_variable_def_s
{
    ast_data_type_t type;
    char *name;
    ir_symbol_address_t address;
};

struct ir_function_def_s
{
    char *name;
    GSList *parameters;
    sym_table_t *local;
    ast_node_t *body;
    ast_data_type_t return_type;
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


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

ir_variable_def_t*
new_ir_variable_def(const char* name, 
                    ast_data_type_t type)
{
    assert(name);

    ir_variable_def_t *var_def;

    var_def = g_malloc(sizeof(*var_def));

    var_def->name = strdup(name);
    var_def->type = type;

    return var_def;
}

void
ir_variable_def_assign_address(ir_variable_def_t *var,
                               ir_symbol_address_t address)
{
    assert(var);

    var->address = address;
}

ir_symbol_address_t
ir_variable_def_get_address(ir_variable_def_t *var)
{
    assert(var);

    return var->address;
}

ast_data_type_t
ir_variable_def_get_type(ir_variable_def_t *var)
{
    assert(var);

    return var->type;
}

ir_function_def_t*
new_ir_function_def()
{
    ir_function_def_t *func_def;

    func_def = g_malloc(sizeof(*func_def));

    func_def->parameters = NULL;
    func_def->local = sym_table_new(NULL);
    func_def->body = NULL;

    return func_def;
}

int
ir_function_def_add_parameter(ir_function_def_t *func,
                              ir_variable_def_t *var)
{
    assert(func);
    assert(var);

    ir_symbol_t *symb;

    GSList *p;

    for (p = func->parameters; p != NULL; p = g_slist_next(p))
    {
        ir_variable_def_t *v = p->data;
        if (strcmp(v->name, var->name) == 0)
        {
            printf("reclaration of function parameter '%s'\n", var->name);
            return -1;
        } 
    }

    func->parameters = 
        g_slist_append(func->parameters, var);
    
    return 0;
}

GSList *
ir_function_def_get_parameters(ir_function_def_t *func)
{
    assert(func);

    return func->parameters;
}

int
ir_function_def_add_local_var(ir_function_def_t *func,
                              ir_variable_def_t *var)
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

sym_table_t *
ir_function_def_get_local_vars(ir_function_def_t *func)
{
    assert(func);

    return func->local;
}

void
ir_function_def_set_name(ir_function_def_t* func,
                         const char *name)
{
    assert(func);
    assert(name);

    func->name = strdup(name);
}

char *
ir_function_def_get_name(ir_function_def_t *func)
{
    assert(func);

    return (func->name);
}

void
ir_function_def_set_body(ir_function_def_t* func,
                         ast_node_t *body)
{
    assert(func);
    assert(body);

    func->body = body;
}

ast_node_t *
ir_function_def_get_body(ir_function_def_t *func)
{
    assert(func);

    return func->body;
}

void
ir_function_def_set_return_type(ir_function_def_t *func,
                                ast_data_type_t return_type)
{
    assert(func);

    func->return_type = return_type;
}

ast_data_type_t
ir_function_def_get_return_type(ir_function_def_t *func)
{
    assert(func);

    return func->return_type;
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

GList *
ir_compile_unit_get_functions(ir_compile_unit_t *compile_unit)
{
    return sym_table_get_all_symbols(compile_unit->global_sym_table);
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
new_ir_symbol_variable(ir_variable_def_t *variable)
{
    ir_symbol_t *symb;

    symb = g_malloc(sizeof(*symb));
    symb->type = ir_variable_symb;
    symb->data.variable = variable;

    return symb;
}

ir_function_def_t *
ir_symbol_get_function(ir_symbol_t *sym)
{
    assert(sym);
    assert(sym->type == ir_function_symb);

    return sym->data.function;
}

ir_variable_def_t *
ir_symbol_get_variable(ir_symbol_t *sym)
{
    assert(sym);
    assert(sym->type == ir_variable_symb);

    return sym->data.variable;
}

void
ir_symbol_del(ir_symbol_t *sym)
{
    free(sym);
}
