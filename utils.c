#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

#include "utils.h"

/*---------------------------------------------------------------------------*
 *                             type definitions                              *
 *---------------------------------------------------------------------------*/

#define MAX_INDENTED_STRING_SIZE 256

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
compile_error(compilation_status_t *compile_status, const char *errmsg, ...)
{
    va_list argp;

    printf("%s: error: ", compile_status->source_file);
    va_start(argp, errmsg);
    vprintf(errmsg, argp);
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
