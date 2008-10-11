#ifndef YYSTYPE_DEF_H
#define YYSTYPE_DEF_H

#include "ast.h"

union yystype_u
{
    char *text;
    int integer;
    ast_node_t* node;
    ast_data_type_t type;
    ast_func_args_list_t *args_list;
    ast_func_parameters_t *func_params;
};

#define YYSTYPE union yystype_u

#endif
