/* New ast */

#ifndef NAST_INC_X
#define NAST_INC_X

#include <glib.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef struct ast_compile_unit_s ast_compile_unit_t;
typedef struct ast_function_s ast_function_t;
typedef struct ast_data_type_s ast_data_type_t;
typedef struct ast_code_block_s ast_code_block_t;

typedef enum basic_data_type_e
{
    void_type, 
    bool_type,               /* boolean */
    int_type                 /* int 32-bit signed */
} basic_data_type_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/


/*----------------------------------*
 * ast compile unit node operations *
 *----------------------------------*/

ast_compile_unit_t *
ast_compile_unit_new();

void
ast_compile_unit_add_function(ast_compile_unit_t *compile_unit,
                              ast_function_t *function);

/*------------------------------*
 * ast function node operations *
 *------------------------------*/

ast_function_t *
ast_function_new(char *name, 
                 GSList *arguments, 
                 ast_data_type_t *return_type,
                 ast_code_block_t *body);

/*-------------------------------*
 * ast data type node operations *
 *-------------------------------*/

ast_data_type_t *
ast_data_type_new_scalar(basic_data_type_t type);

/*--------------------------------*
 * ast code block node operations *
 *--------------------------------*/

#endif /* NAST_INC_X */
