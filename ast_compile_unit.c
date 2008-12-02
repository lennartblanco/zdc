/**
 * @file ast_compile_unit.c
 * @brief ast compile unit node operations implementation
 *
 */

#include <stdbool.h>

#include "nast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

struct ast_compile_unit_s
{
    GSList *functions;
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

ast_compile_unit_t *
ast_compile_unit_new()
{
    ast_compile_unit_t *comp_unit;

    comp_unit = g_malloc(sizeof(*comp_unit));
    comp_unit->functions = NULL;

    return comp_unit;
}

void
ast_compile_unit_add_function(ast_compile_unit_t *compile_unit,
                              ast_function_t *function)
{
    assert(compile_unit);
    assert(function);

    compile_unit->functions = 
        g_slist_append(compile_unit->functions, function);
}


