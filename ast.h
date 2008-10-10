#ifndef AST_INC_X
#define AST_INC_X

#include <glib.h>

typedef struct ast_node_s ast_node_t;

/**
 * AST node types
 */
typedef enum ast_node_type_e 
{
    ast_return_statment_node,  /** rename to ast_return_node */
    ast_function_definition_node,
    ast_statment_list_node,
    ast_function_call_node,
    ast_var_declaration_node,
    ast_var_value_node,
    ast_assigment_node,
    ast_binary_oper_node,
    ast_constant_node,
    ast_negation_node    /** arithmetic negation, e.g. -(10+2) */
} ast_node_type_t;

/*
 * Aithmetic binary operations types
 */
typedef enum ast_oper_type_e
{
    ast_plus_op,
    ast_minus_op,
    ast_mult_op,
    ast_division_op
} ast_oper_type_t;

typedef enum ast_data_type_e
{
    ast_void_type,
    ast_integer_type,          /* int 32-bit signed */
    ast_uinteger_type          /* uint 32-bit unsigned */
} ast_data_type_t;

/*
 * Function call argument list
 */
typedef struct func_args_list_s func_args_list_t;
struct func_args_list_s
{
    ast_node_t *value;
    func_args_list_t *next;
};


/*****
 * AST node payloads
 ****/

typedef struct return_stmt_s
{
    ast_node_t *ret_val;
} return_stmt_t;

typedef struct func_parameters_s
{
    GSList *params;
} func_parameters_t;


typedef struct function_definition_s
{
    ast_data_type_t ret_type;
    char *name;
    func_parameters_t *formal_args;
    ast_node_t *body;
} function_definition_t;

typedef struct statment_list_s
{
    ast_node_t *car;
    ast_node_t *next;
} statment_list_t;

typedef struct function_call_s
{
    char *name;
    func_args_list_t *args;
} function_call_t;

typedef struct assigment_s
{
    char *lvalue;
    ast_node_t *value;
} assigment_t;

/** arithmetic binary operation */
typedef struct binary_op_s
{
    ast_oper_type_t oper_type;
    ast_node_t *left;
    ast_node_t *right;
} binary_op_t;

/** constant expression */
typedef struct constant_s
{
    int value;
} constant_t;

/** arithmetic negation */
typedef struct negation_s
{
    ast_node_t *value;
} negation_t;

/** variable declaration */
typedef struct var_decl_s
{
    ast_data_type_t type;
    char *name;
} var_decl_t;

typedef struct var_val_s
{
    char *name;
} var_val_t;

/*****
 * AST-node data type definition
 ****/

/** union of all the payloads */
typedef union ast_data_u
{
    return_stmt_t ret_stmt;
    function_definition_t function_def;
    statment_list_t stmt;
    function_call_t function_call;
    binary_op_t binary_op;
    constant_t constant;
    negation_t negation;
    var_decl_t var_decl;
    var_val_t var_val;
    assigment_t assigment;
} ast_data_t;

/** THE ast-node definition */
struct ast_node_s
{
    ast_node_type_t type;
    ast_data_t data;
};

/*****
 * Different AST-nodes constructors
 ****/
ast_node_t *new_const(int value);
ast_node_t *new_negation(ast_node_t *value);
ast_node_t *new_binary_operation(ast_oper_type_t oper_type, 
                                 ast_node_t *left, 
                                 ast_node_t *right);
ast_node_t *new_var_declaration(ast_data_type_t var_type, 
                                char *var_name);
ast_node_t *new_assigment(char *lvalue, ast_node_t *value);
ast_node_t *new_var_value(char *name);
ast_node_t *new_function_call(char *name, func_args_list_t *args);
ast_node_t *new_statment_list(ast_node_t *cur, ast_node_t *next);
func_args_list_t *new_func_args_list(ast_node_t *value, func_args_list_t *next);
ast_node_t *new_function_definition(char *name, ast_data_type_t ret_type, void *arg_lst, ast_node_t *body);

ast_node_t *new_return_statment(ast_node_t *ret_val);

func_parameters_t *new_func_parameters();
void func_parameters_add_param(func_parameters_t *func_param, var_decl_t *new_param);
void func_parameters_for_each(func_parameters_t *func_params, void (*iter) (var_decl_t *param));

void add_statment(ast_node_t *stmt);
void for_each_statment(void (*foobar)(ast_node_t *stmt));


#endif /* AST_INC_X */
