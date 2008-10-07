%{
#include <stdlib.h>

#include "yystype_def.h"
#include "yygrammar.h"
%}
%%
"+"    { return '+'; }
"-"    { return '-'; }
"*"    { return '*'; }
"/"    { return '/'; }
"("    { return '('; }
")"    { return ')'; }
";"    { return ';'; }
","    { return ','; }
"="    { return '='; }
"{"    { return '{'; }
"}"    { return '}'; }
"int"  { return INT; }
"void" { return VOID; }
"return" { return RETURN; }
[[:alpha:]][[:alnum:]_]* { yylval.text = strdup(yytext); return IDENT; } 
[0-9]+ { yylval.integer = atoi(yytext); return NUMBER; }
" "    { /* skip blank */ }
\n     { yypos++; /* adjust linenumber and skip newline */ }
.      { yyerror("illegal token"); }
