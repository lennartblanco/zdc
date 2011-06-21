#include <stdio.h>
#include <stdlib.h>

#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *                         module local variable                             *
 *---------------------------------------------------------------------------*/

static int passed = 0;
static int failed = 0;

#define BOOL_TO_STR(x) x ? "true" : "false"

/*---------------------------------------------------------------------------*
 *                           exported functions                              *
 *---------------------------------------------------------------------------*/


void check(char *function_called)
{
    printf("%s [ok]\n", function_called);
    passed += 1;
}

void check_cond(char *function_called, bool cond)
{
    printf("%s ", function_called);
    if (cond)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_int(char *function_called, int res, int expected_res)
{
    printf("%s = %d", function_called, res);
    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_uint(char *function_called, unsigned res, unsigned expected_res)
{
    printf("%s = %u", function_called, res);
    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_short(char *function_called, short res, short expected_res)
{
    printf("%s = %d", function_called, res);
    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_ushort(char *function_called,
             unsigned short res,
             unsigned short expected_res)
{
    printf("%s = %u", function_called, res);
    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_byte(char *function_called, char res, char expected_res)
{
    printf("%s = %d", function_called, res);
    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_ubyte(char *function_called,
            unsigned char res,
            unsigned char expected_res)
{
    printf("%s = %u", function_called, res);
    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_bool(char *function_called, bool res, bool expected_res)
{
    printf("%s = %s", function_called, BOOL_TO_STR(res));
    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_char(char *function_called,
           unsigned char res,
           unsigned char expected_res)
{
    printf("%s = ", function_called);

    if (isascii(res) != 0 && iscntrl(res) == 0)
    {
      printf("'%c'", res);
    }
    else
    {
      /* for non-ascii characters, print the decimal value instead */
      printf("%u", res);
    }

    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_pointer(char *function_called, void *res, void *expected_res)
{
    printf("%s = %p", function_called, res);

    if (res == expected_res)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_non_null_ptr(char *function_called, void *res)
{
    printf("%s = %p", function_called, res);

    if (res != NULL)
    {
        printf(" [ok]\n");
        passed += 1;
    }
    else
    {
        printf(" unexpected result!\n");
        failed += 1;
    }
}

void
check_exit()
{
    printf("passed: %d failed: %d\n", passed, failed);
    if (failed != 0)
    {
        exit(-1);
    }
    exit(0);
}

