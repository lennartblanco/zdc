/**
 * Helper function for function tests of machine code targets (x86)
 */

#include <stdbool.h>

#ifndef CHECK_UTILS_INC_X
#define CHECK_UTILS_INC_X

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/

/**
 * Log that a call to a void function succeded.
 * That is, print an 'func_call OK' message and increase
 * number of passed tests.
 */
void check(char *func_called);

/**
 * Check if functions with integer return value have returned expected result.
 */
void
check_int(char *function_called, int res, int expected_res);

/**
 * Check if functions with boolean return value have returned expected result.
 */
void
check_bool(char *function_called, bool res, bool expected_res);

/**
 * Exit function test program, printing test result summary
 * and returning success code (0) if all tests passed or 
 * fail code (-1) if any test have failed.
 */
void
check_exit();

#endif /* CHECK_UTILS_INX_X */
