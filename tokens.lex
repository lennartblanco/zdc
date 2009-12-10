%{
#include <stdlib.h>

#include "yystype_def.h"
#include "yygrammar.h"
#include "auxil.h"

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
"uint"   { return TOK_UINT; }
"auto"   { return TOK_AUTO; }
"else"   { return TOK_ELSE; }
"void"   { return TOK_VOID; }
"bool"   { return TOK_BOOL; }
"true"   { return TOK_TRUE; }
"false"  { return TOK_FALSE; }
"while"  { return TOK_WHILE; }
"return" { return TOK_RETURN; }
"extern" { return TOK_EXTERN; }
"module" { return TOK_MODULE; }
"foreach" { return TOK_FOREACH; }
("C"|"D") { yylval.text = strdup(yytext); return TOK_LINKAGE_TYPE; }
[[:alpha:]][[:alnum:]_]* { yylval.text = strdup(yytext); return TOK_IDENT; }
[0-9]+   { yylval.integer = atoi(yytext); return TOK_INT_CONST; }
[0-9]+("u"|"U") {  yylval.uinteger = atoi(yytext); return TOK_UINT_CONST; }
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
