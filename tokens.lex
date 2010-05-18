%{
#include <stdlib.h>

#include "yystype_def.h"
#include "yygrammar.h"
#include "auxil.h"

guint8
get_escape_code(char c);

char *
unescape_char_literal(char *str, int len);

%}

%%
"!"      { return '!'; }
"+"      { return '+'; }
"-"      { return '-'; }
"*"      { return '*'; }
"<"      { return '<'; }
">"      { return '>'; }
"/"      { return '/'; }
"("      { return '('; }
")"      { return ')'; }
":"      { return ':'; }
";"      { return ';'; }
","      { return ','; }
"."      { return '.'; }
"="      { return '='; }
"{"      { return '{'; }
"}"      { return '}'; }
"["      { return '['; }
"]"      { return ']'; }
"|"      { return '|'; }
"&"      { return '&'; }
"if"     { return TOK_IF; }
"int"    { return TOK_INT; }
"char"   { return TOK_CHAR; }
"uint"   { return TOK_UINT; }
"auto"   { return TOK_AUTO; }
"else"   { return TOK_ELSE; }
"enum"   { return TOK_ENUM; }
"void"   { return TOK_VOID; }
"bool"   { return TOK_BOOL; }
"true"   { return TOK_TRUE; }
"alias"  { return TOK_ALIAS; }
"false"  { return TOK_FALSE; }
"while"  { return TOK_WHILE; }
"return" { return TOK_RETURN; }
"extern" { return TOK_EXTERN; }
"module" { return TOK_MODULE; }
"import" { return TOK_IMPORT; }
"foreach" { return TOK_FOREACH; }
"immutable" { return TOK_IMMUTABLE; }
("C"|"D") { yylval.text = strdup(yytext); return TOK_LINKAGE_TYPE; }
[[:alpha:]][[:alnum:]_]* { yylval.text = strdup(yytext); return TOK_IDENT; }
[0-9]+   { yylval.integer = atoi(yytext); return TOK_INT_CONST; }
[0-9]+("u"|"U") {  yylval.uinteger = atoi(yytext); return TOK_UINT_CONST; }
"'"."'"  { yylval.character = (guint8)yytext[1]; return TOK_CHAR_CONST; }
"'"\\."'" {

    yylval.character = get_escape_code(yytext[2]);
    return TOK_CHAR_CONST; 
}
\".*\"   {
                           /* drop first and last "-character */
             yylval.text = unescape_char_literal(yytext+1, strlen(yytext)-2);
             return TOK_STRING_LITERAL;
         }
" "      { /* skip blank */ }
"//".*   { /* consume comment */ }
"/*"([^*]|[\n]|(\*+([^*/]|[\n])))*\*+"/" {
           /* consume C-style comments */

           int cntr;
           for (cntr = 0; yytext[cntr] != '\0'; cntr++)
           {
               /* count newline characters and update line counter */
               if (yytext[cntr] == '\n')
               {
                   yypos++;
               }
           }
           
         }
\n       { yypos++; /* adjust linenumber and skip newline */ }
.        { yyerror("illegal token"); }

%%

guint8
get_escape_code(char c)
{
    switch (c) {
      case '\'':
        return 39;
      case '"':
        return 34;
      case '\?':
        return 63;
      case '\\':
        return 92;
      case 'a':
        return 7;
      case 'b':
        return 8;
      case 'f':
        return 12;
      case 'n':
        return 10;
      case 'r':
        return 13;
      case 't':
        return 9;
      case 'v':
        return 11;
      case '0':
        return 0;
    }

    /* unexpected escape character */
    yyerror("undefined escape sequence \\%c", c);
    return 0;
}

/**
 * Process a string literal expression. All escape sequences are replaced
 * by the intended character.
 *
 * @return the newly allocated resulting string
 */
char *
unescape_char_literal(char *str, int len)
{
    int i;
    int p;
    char *res;

    res = g_malloc0((sizeof(char*) * len) + 0);

    for (i = 0, p = 0; i < len; i += 1, p += 1)
    {
        if (str[i] == '\\')
        {
            if (i + 1 >= len)
            {
                yyerror("trailing '\\' in string literal");
                break;
            }
            i += 1;
            res[p] = get_escape_code(str[i]);
        }
        else
        {
            res[p] = str[i];
        }
    }

    return res;
}
