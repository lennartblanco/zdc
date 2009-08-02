#include "check_utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_nested_blocks1()
{
   int res;

   asm ("    call nested_blocks1\n"
        : "=a"(res)
        : );

   return res;
}

int
call_nested_blocks2(int in)
{
   int res;

   asm ("    call nested_blocks2\n"
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

    /* nested_blocks2() tests */
    check_int("nested_blocks2(8)", call_nested_blocks2(8), -1000);

    /* todo: port the tests below to c/x86 */
    assert(false);
//        /* test nested_blocks_with_if() */
//        res = nested_blocks.nested_blocks_with_if(true);
//        check("nested_blocks.nested_blocks_with_if(true)", res, 30);

//        res = nested_blocks.nested_blocks_with_if(false);
//        check("nested_blocks.nested_blocks_with_if(false)", res, 1123);

//        /* test nested_blocks_with_if2() */
//        res = nested_blocks.nested_blocks_with_if2(-20);
//        check("nested_blocks.nested_blocks_with_if2(-20)", res, -1);

//        res = nested_blocks.nested_blocks_with_if2(36);
//        check("nested_blocks.nested_blocks_with_if2(-20)", res, 1);

//        res = nested_blocks.nested_blocks_with_if2(0);
//        check("nested_blocks.nested_blocks_with_if2(-20)", res, 0);

    check_exit();
}
