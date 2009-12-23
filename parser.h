#ifndef PARSER_INC_X
#define PARSER_INC_X

#include "ast_module.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Parse D source file into an Abstract Syntax Tree.
 *
 * @param source_file the source file to parse
 *
 * @return the AST representation of compile file or NULL if there was errors
 *         when parsing.
 */
AstModule *
parse_file(const char* source_file);

void
yyerror(char *msg);

int
yywrap(void);

#endif /* PARSER_INC_X */
