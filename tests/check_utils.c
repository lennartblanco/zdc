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


void check(char *func_called)
{
    printf("%s [ok]\n", func_called);
    passed += 1;
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
check_exit()
{
    printf("passed: %d failed: %d\n", passed, failed);
    if (failed != 0)
    {
        exit(-1);
    }
    exit(0);
}

