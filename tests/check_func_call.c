#include "check_utils.h"

int
call_hej();

int
call_get_13();

int
call_foo();

int
call_add(int arg1, int arg2);

int
call_sum(int arg1, int arg2, int arg3);

int
call_ind_sum(int arg);

int
call_subst3(int arg1, int arg2, int arg3);

int
call_subst4(int arg1, int arg2, int arg3, int arg4);

int
call_add_13(int arg);

int
call_invoke_subst3(int arg);

int
call_unnamed_arg1(int arg1, int arg2);

int
call_unnamed_arg2(int arg1, int arg2);

int
call_unnamed_arg3(int arg1, int arg2, int arg3);

int
call_unnamed_arg4(int arg1, int arg2, int arg3);

int
call_func_5args(int a, int b, int x, int z, int n);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{

    /* sum() tests */
    check_int("sum(-1, 0, 1)", call_sum(-1, 0, 1), -1 + 0 + 1);
    check_int("sum(10, 11, 12)", call_sum(10, 11, 12), 10 + 11 + 12);

    /* add() tests */
    check_int("add(-10, 1)", call_add(-10, 1), -9);
    check_int("add(99, 1)", call_add(99, 1), 100);

    /* get_13() test */
    check_int("get_13()", call_get_13(), 13);

    /* foo() test */
    check_int("foo()", call_foo(), 14);

    /* hej() tests */
    check_int("hej()", call_hej(), 24);

    /* ind_sum() tests */
    check_int("ind_sum(5)", call_ind_sum(5), 5 + 13 + 10);
    check_int("ind_sum(-5)", call_ind_sum(-5), -5 + 13 + 10);

    /* subst3() tests */
    check_int("subst3(1, 2, 3)", call_subst3(1, 2, 3), 1 - 2 - 3);
    check_int("subst3(6, 7, 8)", call_subst3(6, 7, 8), 6 - 7 - 8);

    /* subst4() tests */
    check_int("subst4(1, 2, 3, 100)",
              call_subst4(1, 2, 3, 100), 1 - 2 - 3 - 100);
    check_int("subst4(6, 7, 8, 9)",
              call_subst4(6, 7, 8, 9), 6 - 7 - 8 - 9);
    check_int("subst4(1, 2, 3, -4)",
              call_subst4(1, 2, 3, -4), 1 - 2 - 3 - (-4));

    /* add_13() tests */
    check_int("add_13(10)", call_add_13(10), 10 + 13);
    check_int("add_13(0)", call_add_13(0), 0 + 13);
    check_int("add_13(-14)", call_add_13(-14), -14 + 13);

    /* invoke_subst3() tests */
    check_int("invoke_subst3(5)", call_invoke_subst3(5), 5 - 10 - 20);
    check_int("invoke_subst3(200)", call_invoke_subst3(200), 200 - 10 - 20);

    /* unnamed_arg1() tests */
    check_int("unnamed_arg1(5, 11)", call_unnamed_arg1(5, 11), 11);
    check_int("unnamed_arg1(0, -11)", call_unnamed_arg1(0, -11), -11);

    /* unnamed_arg2() tests */
    check_int("unnamed_arg2(5, 11)", call_unnamed_arg2(5, 11), 5);
    check_int("unnamed_arg2(0, -11)", call_unnamed_arg2(0, -11), 0);

    /* unnamed_arg3() tests */
    check_int("unnamed_arg3(10, true, 13)",
              call_unnamed_arg3(10, true, 13),
              -10);
    check_int("unnamed_arg3(12, false, 13)",
              call_unnamed_arg3(12, false, 13),
              12);

    /* unnamed_arg4() tests */
    check_int("unnamed_arg4(1, 2, 3)", call_unnamed_arg4(1, 2, 3), 10);
    check_int("unnamed_arg4(0, 0, 1)", call_unnamed_arg4(0, 0, 1), 10);

    /* func_5args() tests */
    check_int("func_5args(2, 5, 3, 4, 1)",
              call_func_5args(2, 5, 3, 4, 1),
              2 * 5 + 3 * 4 - 1);

    check_int("func_5args(0, 5, 3, 1, 6)",
              call_func_5args(0, 5, 3, 1, 6),
              0 * 5 + 3 * 1 - 6);

    check_exit();
}
