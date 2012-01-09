#include "check_utils.h"

bool
call_if_isless(int arg);

bool
call_if_isgreater(int arg);

bool
call_if_isless_eq(int arg);

bool
call_if_isgreater_eq(int arg);

bool
call_if_iseq(int arg1, int arg2);

bool
call_if_isnoteq(int arg1, int arg2);

int
call_abs(int arg);

int
call_if_else_test(int arg);

int
call_get_sign(int arg);

int
call_iret_n_arg(int arg1, int arg2, int arg3, int arg4);

int const_if_else();

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* if_else_is_less() tests */
    check_bool("if_isless(-1)", call_if_isless(-1), true);
    check_bool("if_isless(0)", call_if_isless(0), false);
    check_bool("if_isless(10)", call_if_isless(10), false);

    /* if_else_isgreater() tests */
    check_bool("if_isgreater(3)", call_if_isgreater(3), true);
    check_bool("if_isgreater(-3)", call_if_isgreater(-3), false);
    check_bool("if_isgreater(0)", call_if_isgreater(0), false);

    /* if_else_isless_eq() tests */
    check_bool("if_isless_eq(3)", call_if_isless_eq(3), false);
    check_bool("if_isless_eq(-3)", call_if_isless_eq(-3), true);
    check_bool("if_isless_eq(0)", call_if_isless_eq(0), true);

    /* if_else_isgreater_eq() tests */
    check_bool("if_isgreater_eq(3)", call_if_isgreater_eq(3), true);
    check_bool("if_isgreater_eq(-3)", call_if_isgreater_eq(-3), false);
    check_bool("if_isgreater_eq(0)", call_if_isgreater_eq(0), true);

    /* if_else_iseq() tests */        
    check_bool("if_iseq(11, 64)", call_if_iseq(11, 64), false);
    check_bool("if_iseq(998, 998)", call_if_iseq(998, 998), true);

    /* if_else_isnoteq() tests */        
    check_bool("if_isnoteq(11, 64)", call_if_isnoteq(11, 64), true);
    check_bool("if_isnoteq(998, 998)", call_if_isnoteq(998, 998), false);

    /* abs() tests */
    check_int("abs(-19)", call_abs(-19), 19);
    check_int("abs(641)", call_abs(641), 641);
    check_int("abs(0)", call_abs(0), 0);

    /* if_else_test() tests */
    check_int("if_else_test(3)", call_if_else_test(3), 1);
    check_int("if_else_test(10)", call_if_else_test(10), 2);
    check_int("if_else_test(15)", call_if_else_test(15), 3);
    check_int("if_else_test(14)", call_if_else_test(14), 4);

    /* get_sign() tests */
    check_int("get_sign(-102)", call_get_sign(-102), -1);
    check_int("get_sign(-1)", call_get_sign(-1), -1);
    check_int("get_sign(18)", call_get_sign(18), 1);
    check_int("get_sign(1)", call_get_sign(1), 1);
    check_int("get_sign(0)", call_get_sign(0), 0);

    /* iret_n_arg() tests */
    check_int("iret_n_arg(0, 100, 101, 102)",
              call_iret_n_arg(0, 100, 101, 102),
              -1);

    check_int("iret_n_arg(1, 200, 201, 202)",
              call_iret_n_arg(1, 200, 201, 202),
              200);

    check_int("iret_n_arg(2, 300, 301, 302)",
              call_iret_n_arg(2, 300, 301, 302),
              301);

    check_int("iret_n_arg(3, 400, 401, 402)",
              call_iret_n_arg(3, 400, 401, 402),
              402);

    check_int("iret_n_arg(4, 100, 101, 102)",
              call_iret_n_arg(4, 100, 101, 102),
              -1);

    check_int("iret_n_arg(12, 100, 101, 102)",
              call_iret_n_arg(12, 100, 101, 102),
              -1);

    /* const_if_else() test */
    check_int("const_if_else()", const_if_else(), -100);


    check_exit();
}

