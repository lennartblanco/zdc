#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define MAX_INDENTED_STRING_SIZE 256

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Report a compile time error.
 *
 * @param filename the name of the source file where error is found.
 * @errmsg error message with printf-style formatting
 */
void
compile_error(compilation_status_t *compile_status,
              IrNode *error_node,
              const char *errmsg,
              ...)
{
//printf("error_node %s\n", g_type_name(G_TYPE_FROM_INSTANCE(error_node)));

    assert(compile_status);
    assert(compile_status->source_file);
    assert(IR_IS_NODE(error_node));
    /*
     * if line number is 0, which is a default value,
     * it have not been set propertly
     */
    assert(ir_node_get_line_num(error_node) != 0);

    va_list argp;

    fprintf(stderr,
            "%s:%u: error: ",
            compile_status->source_file,
            ir_node_get_line_num(error_node));

    va_start(argp, errmsg);
    vfprintf(stderr, errmsg, argp);
    va_end(argp);

    compile_status->errors_count += 1;
}


void
old_compile_error(compilation_status_t *compile_status, const char *errmsg, ...)
{
    va_list argp;

    fprintf(stderr, "%s: error: ", compile_status->source_file);
    va_start(argp, errmsg);
    vfprintf(stderr, errmsg, argp);
    va_end(argp);

    compile_status->errors_count += 1;
}


void
fprintf_indent(FILE *stream, int indention, const char *format, ...)
{
    char buf[MAX_INDENTED_STRING_SIZE];
    char indent[indention+1];
    int i;
    va_list argp;

    va_start(argp, format);
    vsnprintf(buf, MAX_INDENTED_STRING_SIZE, format, argp);
    va_end(argp);

    for (i = 0; i < indention; i++)
    {
        indent[i] = ' ';
    }
    indent[indention] = '\0';

    char *strp = buf;
    while (true)
    {
        char *lineend = strchr(strp, '\n');
        if (lineend != NULL)
        {
            *lineend = '\0';
            fprintf(stream, "%s%s\n", indent, strp);
            strp = lineend + 1;
        }
        else
        {
            if (strlen(strp) > 0)
            {
                fprintf(stream, "%s%s", indent, strp);
            }
            break;
        }
    }

}
