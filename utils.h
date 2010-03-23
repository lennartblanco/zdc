/**
 * Misc utility functions.
 */

#ifndef UTILS_INC_X
#define UTILS_INC_X

#include "ir_node.h"

/**
 * Print the formatted string with indention to the file stream.
 *
 * @param stream    the stream to print to
 * @param indention the number of space to prepend each line
 * @param format    the formatted string to print in the same format as
 *                  supported by fprintf(3)
 */
void
fprintf_indent(FILE *stream, int indention, const char *format, ...);

#endif /* UTILS_INC_X */
