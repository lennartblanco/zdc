/**
 * @file ast_function.c
 * @brief ast function node operations implementation
 */

#include <stdbool.h>
#include <string.h>

#include "nast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct ast_function_s
{
    char             *name;
    GSList           *parameters;
    ast_data_type_t  *return_type;
    ast_code_block_t *body;
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

ast_function_t *
ast_function_new(char *name, 
                 GSList *parameters,
                 ast_data_type_t *return_type,
                 ast_code_block_t *body)
{
    assert(name);
    ast_function_t *func;

    func = g_malloc(sizeof(*func));

    func->name = strdup(name);
    func->parameters = parameters;
    func->return_type = return_type;
    func->body = body;

    return func;
}

void
ast_function_print(ast_function_t *function, FILE *stream)
{
    assert(function);
    assert(stream);

    fprintf(stream, 
            " function (%p)\n" \
            "  name '%s' return type '",
           function, function->name);
    ast_data_type_print(function->return_type, stream);
    fprintf(stream, "'\n  parameters: ");

    /* print function parameters */
    GSList *p;
    ast_variable_declaration_t *param;
    for (p = function->parameters; p != NULL; p = p->next)
    {
        param = p->data;
        ast_variable_declaration_print(param, stream);
        printf("%s", p->next == NULL ? "" : ", ");
    }

    fprintf(stream, "\n");
}
