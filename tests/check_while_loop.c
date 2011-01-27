#include "check_utils.h"

int
call_while_tst1(int arg);

int
call_while_tst2(int arg);

unsigned
call_while_loc_vars(int arg1, unsigned arg2);

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

    /* while_tst2() tests */
    check_int("while_tst2(10)", call_while_tst2(10), 20);
    check_int("while_tst2(3)", call_while_tst2(3), 21);
    check_int("while_tst2(19)", call_while_tst2(19), 19*2);
    check_int("while_tst2(123)", call_while_tst2(123), 123);

    /* while_loc_vars() tests */
    check_uint("while_loc_vars(0, 0)", call_while_loc_vars(0, 0), 0);
    check_uint("while_loc_vars(10, 1)", call_while_loc_vars(10, 1), 10);
    check_uint("while_loc_vars(15, 6)", call_while_loc_vars(15, 6), 18);

    check_exit();
}
