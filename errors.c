#include "errors.h"

#include <assert.h>

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

