#include "check_utils.h"

int
for_tst1(int start, int stop);

int
for_tst2(int init, int min);

int
for_tst3(int step);

int
for_div(int a, int b);

int
for_break(int start);

int
invoke_for_tst4(int test_case);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* for_tst1() tests */
    check_int("for_tst1(3, 14)", for_tst1(3, 14), 11);
    check_int("for_tst1(100, 0)", for_tst1(100, 0), 0);
    check_int("for_tst1(-10, 10)", for_tst1(-10, 10), 20);

    /* for_tst2() tests */
    check_int("for_tst2(20, 5)", for_tst2(20, 5), 15);
    check_int("for_tst2(0, -15)", for_tst2(0, -15), 15);
    check_int("for_tst2(1, 2)", for_tst2(1, 2), 0);

    /* for_tst3() tests */
    check_int("for_tst3(1)", for_tst3(1), 10);
    check_int("for_tst3(2)", for_tst3(2), 5);
    check_int("for_tst3(11)", for_tst3(11), 1);

    /* for_div() tests */
    check_int("for_div(12, 4)", for_div(12, 4), 12 / 4);
    check_int("for_div(55, 10)", for_div(55, 10), 55 / 10);
    check_int("for_div(21, 1)", for_div(21, 1), 21 / 1);

    /* for_break() tests */
    check_int("for_break(1)", for_break(1), 5);
    check_int("for_break(9)", for_break(9), 10);
    check_int("for_break(15)", for_break(15), 15);

    /* for_tst4 tests */
    check_int("invoke_for_tst4(-1)", invoke_for_tst4(-1), -1);
    check_int("invoke_for_tst4(0)", invoke_for_tst4(0), 1);
    check_int("invoke_for_tst4(1)", invoke_for_tst4(1), 0);
    check_int("invoke_for_tst4(2)", invoke_for_tst4(2), 3);
    check_int("invoke_for_tst4(3)", invoke_for_tst4(3), 2);
    check_int("invoke_for_tst4(4)", invoke_for_tst4(4), -1);

    check_exit();
}
