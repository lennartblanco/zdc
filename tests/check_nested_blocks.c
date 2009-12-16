#include "check_utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_nested_blocks1()
{
   int res;

   asm ("    call _D13nested_blocks14nested_blocks1FZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_nested_blocks2(int in)
{
   int res;

   asm ("    call _D13nested_blocks14nested_blocks2FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_nested_blocks_with_if(bool in)
{
   int res;

   asm ("    call _D13nested_blocks21nested_blocks_with_ifFbZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_nested_blocks_with_if2(int in)
{
   int res;

   asm ("    call _D13nested_blocks22nested_blocks_with_if2FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{

    /* nested_blocks1() test */
    check_int("nested_blocks1()", call_nested_blocks1(), 30);

    /* nested_blocks() test */
    check_int("nested_blocks2(8)", call_nested_blocks2(8), -1000);

    /* nested_blocks_with_if() tests */
    check_int("nested_blocks_with_if(true)",
              call_nested_blocks_with_if(true), 30);
    check_int("nested_blocks_with_if(false)", 
              call_nested_blocks_with_if(false), 1123);

    /* nested_blocks_with_if2() tests */
    check_int("nested_blocks_with_if2(-20)",
              call_nested_blocks_with_if2(-20),
              -1);

    check_int("nested_blocks_with_if2(36)",
              call_nested_blocks_with_if2(36),
              1);

    check_int("nested_blocks_with_if2(0)",
              call_nested_blocks_with_if2(0),
              0);

    check_exit();
}
