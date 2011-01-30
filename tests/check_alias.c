#include "check_utils.h"

int
invoke_func1(int arg1, int arg2);

int
func2(int arg1);

unsigned
invoke_strlen(unsigned test_case);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* func1() tests */
    check_int("func1(2, 4)", invoke_func1(2, 4), 2 + 4);
    check_int("func1(-100, 99)", invoke_func1(-100, 99), -100 + 99);

    /* func2() tests */
    check_int("func2(11)", func2(11), (11 + 2) * 3);
    check_int("func2(0)", func2(0), (0 + 2) * 3);

    /* strlen() tests */
    check_uint("invoke_strlen(0)", invoke_strlen(0), 20);
    check_uint("invoke_strlen(1)", invoke_strlen(1), 16);
    check_uint("invoke_strlen(100)", invoke_strlen(100), 0);

    check_exit();
}
