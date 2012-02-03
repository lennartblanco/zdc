#ifndef YYSTYPE_DEF_H
#define YYSTYPE_DEF_H

#include "ast_module.h"
#include "ast_enum_member.h"
#include "ast_ident.h"
#include "ast_variable_declaration.h"
#include "ast_variable_definition.h"
#include "ast_if_else.h"
#include "ast_declaration_block.h"

union yystype_u
{
    char                       *text;
    guint8                     character;
    guint64                    integer;
    DtDataType                 *data_type;
    AstModule                  *module;
    AstEnumMember              *enum_member;
    AstCodeBlock               *code_block;
    AstDeclaration             *decl;
    AstDeclarationBlock        *decl_block;
    AstFunctionDecl            *function_decl;
    AstFunctionDef             *function_def;
    AstStatment                *statment;
    AstIdent                   *ident;
    AstExpression              *expression;
    AstVariableDeclaration     *variable_declaration;
    AstVariableDefinition      *variable_definition;
    AstIfElse                  *if_else;
    AstIfBlock                 *if_block;
    AstAttributes              *attrs;
    AstAttribute               *attr;
    GSList                     *list;
};

#define YYSTYPE union yystype_u

#endif
