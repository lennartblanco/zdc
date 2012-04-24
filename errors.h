/**
 * Compile error reporting function
 */

#ifndef ERRORS_INC_X
#define ERRORS_INC_X

#include "sem_analyze_validate.h"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Report a compile time error.
 *
 * @param compile_status compilation status to use for source file name and
 *                       errors count,
 *                       if NULL then no line number will be printed
 * @param error_node     report error on this node's line number
 * @param errmsg         error message with printf-style formatting
 */
void
compile_error(compilation_status_t *compile_status,
              void *error_node,
              const char *errmsg,
              ...);

#endif /* ERRORS_INC_X */
