#ifndef PARSER_INC_X
#define PARSER_INC_X

#include "ast_module.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

/** parser errors domain */
#define PARSER_ERROR parser_error_quark()

/** Error codes returned by parse_file() */
typedef enum
{
    PARSER_FILE_READ_ERROR,
    PARSER_SYNTAX_ERROR
} parse_error_t;

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Parse D source file into an Abstract Syntax Tree.
 * If the source file contains any parse errors, they will be printed to stderr.
 *
 * @param source_file the source file to parse
 * @param error the error are passed via this argument
 *
 * @return the AST representation of compile file or NULL if there was errors
 *         when parsing.
 */
AstModule *
parse_file(const char* source_file, GError **error);

void
yyerror(char *msg, ...);

int
yywrap(void);

#endif /* PARSER_INC_X */
