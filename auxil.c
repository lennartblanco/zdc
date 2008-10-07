#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "gen_java.h"
#include "sym_table.h"
#include "sem_analyze.h"

sym_table_t *global_sym_table;

int main()
{
   global_sym_table = sym_table_new(NULL);
   yyparse();

   semantic_analyze(global_sym_table);
   //gen_java_prelude();
   gen_java_code();
   //gen_java_epilog();

   return 0;
}

yyerror(msg)
   char *msg;
{
   extern long yypos;

   printf("line %d: %s\n", yypos, msg);
   exit(1);
}

yywrap()
{
   return 1;
}
