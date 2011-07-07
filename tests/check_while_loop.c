#include "check_utils.h"

int
call_while_tst1(int arg);

int
call_while_tst2(int arg);

unsigned
call_while_loc_vars(int arg1, unsigned arg2);

unsigned
while_true(unsigned a, unsigned b);

int
while_false(int a, int b);

bool
invoke_while_nested(unsigned test_num);

int
while_break(int break_val);

int
while_nested_break(bool do_break);


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

    /* while_true() tests */
    check_uint("while_true(20,5)", while_true(20, 5), 20/5);
    check_uint("while_true(2, 7)", while_true(2, 7), 2/7);
    check_uint("while_true(16, 3)", while_true(16, 3), 16/3);

    /* while_false() tests */
    check_int("while_false(2, 7)", while_false(2,7), 2);
    check_int("while_false(0, 2000)", while_false(0, 2000), 0);

    /* while_nested() tests */
    check_bool("invoke_while_nested(0)", invoke_while_nested(0), true);
    check_bool("invoke_while_nested(1)", invoke_while_nested(1), true);
    check_bool("invoke_while_nested(2)", invoke_while_nested(2), false);
    check_bool("invoke_while_nested(3)", invoke_while_nested(3), false);
    check_bool("invoke_while_nested(4)", invoke_while_nested(4), true);

    /* while_break() tests */
    check_int("while_break(0)", while_break(0), 10);
    check_int("while_break(20)", while_break(20), 10);
    check_int("while_break(6)", while_break(6), 6);

    /* while_nested_break() tests */
    check_int("while_nested_break(false)", while_nested_break(false), 1);
    check_int("while_nested_break(true)", while_nested_break(true), 2);

    check_exit();
}
