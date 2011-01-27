#include "check_utils.h"

bool
call_dummy1(int in);

int
call_dummy2();

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

