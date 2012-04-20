#include "errors.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

void
compile_error(compilation_status_t *compile_status,
              void *error_node,
              const char *errmsg,
              ...)
{
    assert(compile_status);
    assert(compile_status->source_file);
    assert(IR_IS_NODE(error_node) || AST_IS_NODE(error_node));

    guint line_num;

    if (IR_IS_NODE(error_node))
    {
        line_num = ir_node_get_line_num(error_node);
    }
    else if (AST_IS_NODE(error_node))
    {
        line_num = ast_node_get_line_num(error_node);
    }
    else
    {
        /* unexpected error node type */
        assert(false);
    }

    /*
     * if line number is 0, which is a default value,
     * it have not been set properly
     */
    assert(line_num != 0);

    va_list argp;

    fprintf(stderr,
            "%s:%u: error: ",
            compile_status->source_file,
            line_num);

    va_start(argp, errmsg);
    vfprintf(stderr, errmsg, argp);
    va_end(argp);

    compile_status->errors_count += 1;
}

