/**
 * @file ast_variable_declaration.c
 * @brief ast variable declaration node operations implementation
 */

#include <string.h>
#include <stdio.h>

#include "nast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct ast_variable_declaration_s
{
    char            *name;
    ast_data_type_t *type;
};


/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

ast_variable_declaration_t *
ast_variable_declaration_new(char *name, ast_data_type_t *type)
{
    ast_variable_declaration_t *var_dec;

    var_dec = g_malloc(sizeof(*var_dec));

    var_dec->name = strdup(name);
    var_dec->type = type;

    return var_dec;
}

void
ast_variable_declaration_print(ast_variable_declaration_t *variable_decl,
                               FILE *stream)
{
    assert(variable_decl);
    ast_data_type_print(variable_decl->type, stream);
    fprintf(stream, " %s", variable_decl->name);
}
