#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

bool
call_if_isless(int in)
{
   bool res;

   asm ("    call if_isless\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_if_isgreater(int in)
{
   bool res;

   asm ("    call if_isgreater\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_if_isless_eq(int in)
{
   bool res;

   asm ("    call if_isless_eq\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_if_isgreater_eq(int in)
{
   bool res;

   asm ("    call if_isgreater_eq\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_if_iseq(int arg1, int arg2)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call if_iseq\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

bool
call_if_isnoteq(int arg1, int arg2)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call if_isnoteq\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_abs(int in)
{
   int res;

   asm ("    call abs\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_if_else_test(int in)
{
   int res;

   asm ("    call if_else_test\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_get_sign(int in)
{
   int res;

   asm ("    call get_sign\n"
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
    

    check_exit();
}

