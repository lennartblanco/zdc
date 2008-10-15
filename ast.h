#ifndef AST_INC_X
#define AST_INC_X

#include <glib.h>

typedef struct ast_node_s ast_node_t;

/**
 * AST node types
 */
typedef enum ast_node_type_e
{
    ast_compile_unit_node,     /** a D-compile unit, e.g. one source file */
    ast_return_statment_node,  /** rename to ast_return_node */
    ast_function_definition_node,
    ast_code_block_node,
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
typedef struct ast_func_args_list_s ast_func_args_list_t;
struct ast_func_args_list_s
{
    ast_node_t *value;
    ast_func_args_list_t *next;
};


/*********************
 * AST node payloads *
 *********************/

typedef struct ast_compile_unit_s
{
    GSList *functions;
} ast_compile_unit_t;

typedef struct ast_code_block_s
{
    GSList *statments;
} ast_code_block_t;

typedef struct ast_return_stmt_s
{
    ast_node_t *ret_val;
} ast_return_stmt_t;

typedef struct ast_func_parameters_s
{
    GSList *params;
} ast_func_parameters_t;


typedef struct ast_function_definition_s
{
    ast_data_type_t ret_type;
    char *name;
    ast_func_parameters_t *formal_args;
    ast_node_t *body;
} ast_function_definition_t;

typedef struct ast_statment_list_s
{
    ast_node_t *car;
    ast_node_t *next;
} ast_statment_list_t;

typedef struct ast_function_call_s
{
    char *name;
    ast_func_args_list_t *args;
} ast_function_call_t;

typedef struct ast_assigment_s
{
    char *lvalue;
    ast_node_t *value;
} ast_assigment_t;

/** arithmetic binary operation */
typedef struct ast_binary_op_s
{
    ast_oper_type_t oper_type;
    ast_node_t *left;
    ast_node_t *right;
} ast_binary_op_t;

/** constant expression */
typedef struct ast_constant_s
{
    int value;
} ast_constant_t;

/** arithmetic negation */
typedef struct ast_negation_s
{
    ast_node_t *value;
} ast_negation_t;

/** variable declaration */
typedef struct ast_var_decl_s
{
    ast_data_type_t type;
    char *name;
} ast_var_decl_t;

typedef struct ast_var_val_s
{
    char *name;
} ast_var_val_t;

/*****
 * AST-node data type definition
 ****/

/** union of all the payloads */
typedef union ast_data_u
{
    ast_compile_unit_t compile_unit;
    ast_code_block_t code_block;
    ast_return_stmt_t ret_stmt;
    ast_function_definition_t function_def;
    ast_statment_list_t stmt;
    ast_function_call_t function_call;
    ast_binary_op_t binary_op;
    ast_constant_t constant;
    ast_negation_t negation;
    ast_var_decl_t var_decl;
    ast_var_val_t var_val;
    ast_assigment_t assigment;
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

ast_node_t *
new_const(int value);

ast_node_t *
new_negation(ast_node_t *value);

ast_node_t *
new_binary_operation(ast_oper_type_t oper_type, 
                     ast_node_t *left, 
                     ast_node_t *right);

ast_node_t *
new_var_declaration(ast_data_type_t var_type, 
                    char *var_name);

ast_node_t *
new_assigment(char *lvalue, 
              ast_node_t *value);

ast_node_t *
new_var_value(char *name);

ast_node_t *
new_function_call(char *name, 
                  ast_func_args_list_t *args);

ast_func_args_list_t *
new_func_args_list(ast_node_t *value, 
                   ast_func_args_list_t *next);

ast_node_t *new_function_definition(char *name, 
                                    ast_data_type_t ret_type, 
                                    void *arg_lst, 
                                    ast_node_t *body);

ast_node_t *new_return_statment(ast_node_t *ret_val);

ast_func_parameters_t *
new_func_parameters();

void 
func_parameters_add_param(ast_func_parameters_t *func_param,
                          ast_var_decl_t *new_param);

void
func_parameters_for_each(ast_func_parameters_t *func_params, 
                         void (*iter) (ast_var_decl_t *param));

/************************************
 * AST compile unit node operations *
 ***********************************/

/**
 * create a new compile unit
 */
ast_node_t *
new_compile_unit();

/**
 * add a function to a compile unit
 */
void
compile_unit_add_function(ast_node_t *compile_unit,
                          ast_node_t *function);

/**
 * iterate over function in this compile unit
 */
void
compile_unit_for_each_function(ast_node_t *compile_unit,
                               void (*iter) (ast_node_t *function, void *),
                               void *user_data);

/*********************************
 * AST code block node operation *
 *********************************/

ast_node_t *
new_code_block();

void
code_block_add_statment(ast_node_t *code_block, 
                        ast_node_t *statment);
#endif /* AST_INC_X */
