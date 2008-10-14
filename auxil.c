#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "gen_java.h"
#include "sym_table.h"
#include "sem_analyze.h"

extern ast_node_t *root_node;

int main()
{
   yyparse();

   semantic_analyze(root_node);
   //gen_java_prelude();
   //gen_java_code();
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
