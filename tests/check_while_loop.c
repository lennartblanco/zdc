#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_while_tst1(int in)
{
   int res;

   asm ("    call while_tst1\n"
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
    /* while_tst1() tests */
    check_int("while_tst1(10)", call_while_tst1(10), 20);
    check_int("while_tst1(3)", call_while_tst1(3), 21);
    check_int("while_tst1(19)", call_while_tst1(19), 19*2);
    check_int("while_tst1(123)", call_while_tst1(123), 123);

    check_exit();
}
