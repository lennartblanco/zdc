#ifndef YYSTYPE_DEF_H
#define YYSTYPE_DEF_H

#include "ast.h"

union yystype_u
{
    char *text;
    int integer;
    ast_node_t* node;
    data_type_t type;
    func_args_list_t *args_list;
    func_parameters_t *func_params;
};

#define YYSTYPE union yystype_u

#endif
