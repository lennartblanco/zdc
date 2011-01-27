#include "check_utils.h"

void
call_void_return();

int
call_int_ret();

int
call_iret_val(int in);

int
call_iret_arg1(int arg1, int arg2, int arg3);

int
call_iret_arg1_with_ret_var(int arg1, int arg2, int arg3);

int
call_iret_arg2(int arg1, int arg2, int arg3);

int
call_iret_arg3(int arg1, int arg2, int arg3);

void
call_printNum(int arg);

void
call_no_ret_void(int arg1, int arg2);

void
call_empty_func();

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* void_return() test */
    call_void_return();
    check("void_return");

    /* int_ret() test */
    check_int("int_ret()", call_int_ret(), 10);

    /* iret_val() tests */
    check_int("iret_val(-2)", call_iret_val(-2), -2);
    check_int("iret_val(1984)", call_iret_val(1984), 1984);

    /* iret_arg1() tests */
    check_int("iret_arg1(11, 21, 31)", call_iret_arg1(11, 21, 31), 11);
    check_int("iret_arg1(-876, 1234, 789)", call_iret_arg1(-876, 1234, 789), -876);

    /* iret_arg1_with_ret_var() tests */
    check_int("iret_arg1_with_ret_var(11, 21, 31)", 
              call_iret_arg1_with_ret_var(11, 21, 31), 11);
    check_int("iret_arg1_with_ret_var(9876, 1234, 789)",
              call_iret_arg1_with_ret_var(9876, 1234, 789), 9876);

    /* iret_arg2() tests */
    check_int("iret_arg2(11, 221, 31)", call_iret_arg2(11, 221, 31), 221);
    check_int("iret_arg2(76, 1234, 789)", call_iret_arg2(76, 1234, 789), 1234);

    /* iret_arg3() tests */
    check_int("iret_arg3(11, 221, 31)", call_iret_arg3(11, 221, 31), 31);
    check_int("iret_arg3(76, 1234, 789)", call_iret_arg3(76, 1234, 789), 789);

    /* printNum() tests */
    call_printNum(10);
    check("printNum(10)");

    call_printNum(-2048);
    check("printNum(-2048)");

    /* no_ret_void() tests */
    call_no_ret_void(1, 2);
    check("no_ret_void(1, 2)");

    call_no_ret_void(200, -100);
    check("no_ret_void(200, -100)");

    /* empty_func() test */
    call_empty_func();
    check("empty_func()");

    check_exit();
}
