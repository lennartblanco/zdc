#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "lex.h"
#include "ast_compile_unit.h"
#include "java_trgt.h"
#include "sym_table.h"
#include "sem_analyze.h"

#include <assert.h>

extern AstCompileUnit *compile_unit;

typedef struct compile_options_s
{
    bool print_ast;
} compile_options_t;

/*---------------------------------------------------------------------------*
 *                  local functions forward declaration                      *
 *---------------------------------------------------------------------------*/

/**
 * Convert a path to a class name. The filename without the extension becomes
 * the filename, e.g.
 *     foo.d > foo
 *     roo/ma.d > ma
 *
 * This function assumes that the provided filename ends with '.d' string.
 *
 * @param file_name the file name to use as template for the class name
 * @param class_name the resulting class name will be written here, this
 *                   argument must point to an array that has place for
                     the file name plus extension
 */
void
get_class_name(const char *file_name, char *class_name);

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/ 

/**
 * Compile the D source file and write generated java assembly 
 * to provided output file.
 *
 * @param input_file the file name of the D source file to compile
 * @param output_file the file name where to write generated java assmebly
 *
 * @return 0 if the file was succcessfully compile, -1 on errors
 */
int
compile_file(const char* input_file, 
             const char* output_file,
             compile_options_t options)
{
   YY_BUFFER_STATE yy_buffer;
   FILE *input_stream;
   FILE *output_stream;
   ir_compile_unit_t *ir_compile_unit;
   char klass_name[strlen(output_file)];

   /* open the input source file */
   input_stream = fopen(input_file, "r");
   if (input_stream == NULL)
   {
       fprintf(stderr, 
               "error opening file '%s' for reading: %m\n",
               input_file);
       return -1;        
   }

   /* setup token parser to read the opened file */
   yy_buffer = yy_create_buffer(input_stream, YY_BUF_SIZE);
   yy_switch_to_buffer(yy_buffer);

   /* open the output file */
   output_stream = fopen(output_file, "w");
   if (output_stream == NULL)
   {
       fprintf(stderr, 
               "error opening file '%s' for writing: %m\n",
               output_file);
       fclose(input_stream);
       return -1;        
   }
   

   fprintf(output_stream, "; compiling %s\n", input_file);
   yyparse();

   if (options.print_ast)
   {
       ast_node_print(XDP_AST_NODE(compile_unit), stdout);
   }

   ir_compile_unit = semantic_analyze(compile_unit);

   /* use the output file name as the basis for class name */
   get_class_name(output_file, klass_name);
   java_trgt_code(ir_compile_unit, output_stream, klass_name);
//   java_trgt_code(ir_compile_unit, stdout, klass_name);

   /* clean up */
   fclose(output_stream);
   yy_delete_buffer(yy_buffer);
   fclose(input_stream);
   

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
   /* stop token parser when EOF is reached */
   return 1;
}

/*---------------------------------------------------------------------------*
 *                             local functions                               *
 *---------------------------------------------------------------------------*/
void
get_class_name(const char *file_name, char *class_name)
{
    const char *last_slash;
    char *last_dot;

    last_slash = strrchr(file_name, '/');
    if (last_slash == NULL)
    {
        last_slash = file_name;
    }
    else
    {
        last_slash += 1;
    }

    strcpy(class_name, last_slash);

    last_dot = strrchr(class_name, '.');
    assert(last_dot != NULL);

    last_dot[0] = '\0';
}

