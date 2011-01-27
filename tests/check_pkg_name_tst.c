#include "check_utils.h"

int
call_first_func();

bool
call_second_func(int arg);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* first_func() test */
    check_int("first_func()", call_first_func(), 1969);

    /* second_func() tests */
    check_bool("second_func(true)", call_second_func(true), false);
    check_bool("second_func(false)", call_second_func(false), true);

    check_exit();
}
