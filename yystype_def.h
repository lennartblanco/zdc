#ifndef YYSTYPE_DEF_H
#define YYSTYPE_DEF_H

#include "ast_compile_unit.h"
#include "ast_variable_ref.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_if_else.h"

union yystype_u
{
    char                       *text;
    int                        integer;
    DtDataType                 *data_type;
    basic_data_type_t          basic_data_type;
    AstCompileUnit             *compile_unit;
    AstCodeBlock               *code_block;
    AstFunctionDecl            *function_decl;
    AstFunctionDef             *function_def;
    AstStatment                *statment;
    AstVariableRef             *variable_ref;
    AstExpression              *expression;
    AstVariableDeclaration     *variable_declaration;
    AstVariableDefinition      *variable_definition;
    AstIfElse                  *if_else;
    AstIfBlock                 *if_block;
    GSList                     *list;
};

#define YYSTYPE union yystype_u

#endif
