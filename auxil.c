#include <stdlib.h>
#include <stdio.h>

#include "ast.h"
#include "gen_java.h"
#include "sym_table.h"
#include "sem_analyze.h"

extern ast_node_t *root_node;

int main()
{
   ir_compile_unit_t *comp_unit;

   yyparse();

   comp_unit = semantic_analyze(root_node);
   gen_java_code(comp_unit);

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
