#ifndef YYSTYPE_DEF_H
#define YYSTYPE_DEF_H

#include "ast_compile_unit.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_expression.h"

union yystype_u
{
    char                       *text;
    int                        integer;
    AstCompileUnit             *compile_unit;
    AstCodeBlock               *code_block;
    AstFunction                *function;
    AstStatment                *statment;
    AstDataType                *data_type;
    AstExpression              *expression;
    AstVariableDeclaration     *variable_declaration;
    AstVariableDefinition      *variable_definition;
    GSList                     *list;
};

#define YYSTYPE union yystype_u

#endif
