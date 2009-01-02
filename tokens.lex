%{
#include <stdlib.h>

#include "yystype_def.h"
#include "yygrammar.h"
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
";"      { return ';'; }
","      { return ','; }
"="      { return '='; }
"{"      { return '{'; }
"}"      { return '}'; }
"|"      { return '|'; }
"&"      { return '&'; }
"if"     { return TOK_IF; }
"int"    { return TOK_INT; }
"else"   { return TOK_ELSE; }
"void"   { return TOK_VOID; }
"bool"   { return TOK_BOOL; }
"true"   { return TOK_TRUE; }
"false"  { return TOK_FALSE; }
"return" { return TOK_RETURN; }
[[:alpha:]][[:alnum:]_]* { yylval.text = strdup(yytext); return TOK_IDENT; } 
[0-9]+   { yylval.integer = atoi(yytext); return TOK_NUMBER; }
" "      { /* skip blank */ }
"//".*   { /* consume comment */ }
\n       { yypos++; /* adjust linenumber and skip newline */ }
.        { yyerror("illegal token"); }
