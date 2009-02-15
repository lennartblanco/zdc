/**
 * Misc utility functions.
 */

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
compile_error(const char *filename, const char *errmsg, ...);

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
