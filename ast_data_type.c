/**
 * @file ast_data_type.c
 * @brief ast data type node operations implementation
 */

#include <stdbool.h>

#include "nast.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

typedef enum ast_data_type_type_s
{
    ast_basic_type,
    ast_static_array_type,
    ast_dynamic_array_type,
}  ast_data_type_type_t;

typedef union ast_data_type_node_u
{
    basic_data_type_t basic_type;
}  ast_data_type_node_t;

struct ast_data_type_s
{
    ast_data_type_type_t type;
    ast_data_type_node_t node;
};

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

ast_data_type_t *
ast_data_type_new_scalar(basic_data_type_t type)
{
    ast_data_type_t *data_type;

    data_type = g_malloc(sizeof(*data_type));

    data_type->type = ast_basic_type;
    data_type->node.basic_type = type;

    return data_type;
}

void
ast_data_type_print(ast_data_type_t *data_type, FILE *stream)
{
    assert(data_type);
    assert(stream);

    switch (data_type->type)
    {
        case ast_basic_type:
            switch (data_type->node.basic_type)
            {
                case void_type:
                    fprintf(stream, "void");
                    break;
                case int_type:
                    fprintf(stream, "int");
                    break;
                case bool_type:
                    fprintf(stream, "bool");
                    break;
                default:
                    /* unexpected basic data type */
                    assert(false);
            }
            break;
        default:
            /* unexpected ast data type type */
            break;
    }
}
