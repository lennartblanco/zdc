#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

bool
call_dummy1(int in)
{
   bool res;

   asm ("    call _D8comments6dummy1FbZb\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_dummy2()
{
   int res;

   asm ("    call _D8comments6dummy2FZi\n"
        : "=a"(res)
        : );

   return res;
}

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* dummy1() tests */
    check_bool("dummy1(true)", call_dummy1(true), true);
    check_bool("dummy1(false)", call_dummy1(false), false);

    /* dummy2() test */
    check_int("dummy2()", call_dummy2(), 3);

    check_exit();
}

