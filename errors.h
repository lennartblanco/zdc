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
 * @param filename the name of the source file where error is found.
 * @errmsg error message with printf-style formatting
 */
void
compile_error(compilation_status_t *compile_status,
              IrNode *error_node,
              const char *errmsg,
              ...);

#endif /* ERRORS_INC_X */
