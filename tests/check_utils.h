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
void check(char *function_called);

/**
 * Log that function was calles successfully if cond is true, otherwise
 * log failure.
 */
void check_cond(char *function_called, bool cond);

/**
 * Check if functions with integer return value have returned expected result.
 */
void
check_int(char *function_called, int res, int expected_res);

/**
 * Check if functions with unsigned integer return value have 
 * returned expected result.
 */
void
check_uint(char *function_called, unsigned res, unsigned expected_res);

/**
 * Check if functions with boolean return value have returned expected result.
 */
void
check_bool(char *function_called, bool res, bool expected_res);

/**
 * Check if functions with boolean return value have returned expected result.
 */
void
check_char(char *function_called,
           unsigned char res,
           unsigned char expected_res);

/**
 * Check if functions with pointer return value have returned expected result.
 */
void
check_pointer(char *function_called, void *res, void *expected_res);

/**
 * Check that function have returned a non-null pointer.
 */
void
check_non_null_ptr(char *function_called, void *res);

/**
 * Exit function test program, printing test result summary
 * and returning success code (0) if all tests passed or 
 * fail code (-1) if any test have failed.
 */
void
check_exit();

#endif /* CHECK_UTILS_INX_X */
