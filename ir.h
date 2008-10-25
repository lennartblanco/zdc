#ifndef IR_INC_X
#define IR_INC_X

#include "ast.h"
#include "sym_table_types.h"

typedef struct ir_variable_def_s ir_variable_def_t;
typedef struct ir_function_def_s ir_function_def_t;
typedef struct ir_compile_unit_s ir_compile_unit_t;
typedef struct ir_symbol_s ir_symbol_t;

typedef union ir_symbol_data_u
{
    ir_function_def_t *function;
    ir_variable_def_t *variable;
} ir_symbol_data_t;

typedef enum ir_symbol_type_e
{
    ir_variable_symb,
    ir_function_symb
} ir_symbol_type_t;

/******************************
 * IR Symbols operations      *
 ******************************/

/**
 * create an IR symbol representing a function
 */
ir_symbol_t*
new_ir_symbol_function(ir_function_def_t *function);

/**
 * create an IR symbol representing a variable declaration
 */
ir_symbol_t*
new_ir_symbol_variable(ir_variable_def_t *variable);

/**
 * get function represented by this symbol. 
 *
 * calling this function on symbol not representing functions
 * leads to undefined behavior.
 * 
 * @return the function represented
 */
ir_function_def_t *
ir_symbol_get_function(ir_symbol_t *sym);

/**
 * get variable represented by this symbol. 
 *
 * calling this function on symbol not representing variable
 * leads to undefined behavior.
 * 
 * @return the variable represented
 */
ir_variable_def_t *
ir_symbol_get_variable(ir_symbol_t *sym);


/**
 * free all resources allocated by this symbol
 */
void
ir_symbol_del(ir_symbol_t *sym);

/******************************
 * IR Compile Unit operations *
 ******************************/

/**
 * constructor
 */
ir_compile_unit_t*
new_ir_compile_unit();

/**
 * add a function definition to this compile unit
 *
 * @param compile_unit   the compile unit to add to
 * @param function_name  function's name
 * @param function_def   the IR-representation of the function
 */
int
ir_compile_unit_add_function(ir_compile_unit_t *compile_unit,
                             ir_function_def_t *function_def);

/**
 * get all the functions defined in this compile unit
 *
 * @param compile_unit  the compile unit to fetch functions from
 *
 * @return functions as a list of function as ir_symbol_t pointers
 */
GList *
ir_compile_unit_get_functions(ir_compile_unit_t *compile_unit);

/*************************************
 * IR variable definition operations *
 *************************************/

ir_variable_def_t*
new_ir_variable_def(const char* name, 
                    ast_data_type_t type);

/**
 * @return this variables data type
 */
ast_data_type_t
ir_variable_def_get_type(ir_variable_def_t *var);

/*************************************
 * IR function definition operations *
 *************************************/

/**
 * constructor
 */
ir_function_def_t*
new_ir_function_def();

/**
 * add a formal call parameter
 *
 * @return 0  if added successfully
 * @return -1 if there were errors
 */
int
ir_function_def_add_parameter(ir_function_def_t *func,
                              ir_variable_def_t *var);

sym_table_t *
ir_function_def_get_parameters(ir_function_def_t *func);

int
ir_function_def_add_local_var(ir_function_def_t *func,
                              ir_variable_def_t *var);
                              

void
ir_function_def_set_name(ir_function_def_t *func,
                         const char *name);

char *
ir_function_def_get_name(ir_function_def_t *func);

void
ir_function_def_set_body(ir_function_def_t *func,
                         ast_node_t *body);

#endif /* IR_INC_X */
