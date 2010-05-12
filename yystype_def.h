#ifndef YYSTYPE_DEF_H
#define YYSTYPE_DEF_H

#include "ast_module.h"
#include "ast_enum_member.h"
#include "ast_variable_ref.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_if_else.h"

union yystype_u
{
    char                       *text;
    guint8                     character;
    gint32                     integer;
    guint32                    uinteger;
    DtDataType                 *data_type;
    AstModule                  *module;
    AstEnumMember              *enum_member;
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
