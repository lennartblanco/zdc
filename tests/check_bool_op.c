#include "check_utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

bool
call_return_true()
{
   bool res;

   asm ("    call return_true\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_return_false()
{
   bool res;

   asm ("    call return_false\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_invert(bool in)
{
   bool res;

   asm ("    call invert\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_is_equal(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call is_equal\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_not_equal(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call not_equal\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_is_less_or_equal(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call is_less_or_equal\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_is_greater_or_equal(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call is_greater_or_equal\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_is_greater(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call is_greater\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_is_less(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call is_less\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* return_true() test */
    check_bool("return_true()", call_return_true(), true);

    /* return_true() test */
    check_bool("return_false()", call_return_false(), false);

    /* invert() tests */
    check_bool("invert(true)", call_invert(true), false);
    check_bool("invert(false)", call_invert(false), true);

    /* is_equal() tests */
    check_bool("is_equal(20, 45)", call_is_equal(20, 45), false);
    check_bool("is_equal(983, 983)", call_is_equal(983, 983), true);

    /* not_equal() tests */
    check_bool("not_equal(57, -35)", call_not_equal(57, -35), true);
    check_bool("not_equal(0, 0)", call_not_equal(0, 0), false);

    /* is_less_or_equal() tests */
    check_bool("is_less_or_equal(1, 2)", call_is_less_or_equal(1, 2), true);
    check_bool("is_less_or_equal(2, 2)", call_is_less_or_equal(2, 2), true);
    check_bool("is_less_or_equal(1, -2)", call_is_less_or_equal(1, -2), false);

    /* is_greater_or_equal() tests */
    check_bool("is_greater_or_equal(1, 2)", call_is_greater_or_equal(1, 2), false);
    check_bool("is_greater_or_equal(2, 2)", call_is_greater_or_equal(2, 2), true);
    check_bool("is_greater_or_equal(1, -2)", call_is_greater_or_equal(1, -2), true);

    /* is_greater() tests */
    check_bool("is_greater(1, 2)", call_is_greater(1, 2), false);
    check_bool("is_greater(10, 10)", call_is_greater(10, 10), false);
    check_bool("is_greater(100, 8)", call_is_greater(100, 8), true);

    /* is_less() tests */
    check_bool("is_less(1, 2)", call_is_less(1, 2), true);
    check_bool("is_less(100, 8)", call_is_less(100, 8), false);
    check_bool("is_less(0, 0)", call_is_less(0, 0), false);

/*
 * todo add test on:
 *
 * greater_tst1()
 * greater_tst2(int x)
 * greater_tst3(int x)
 * less_tst1()
 * less_tst2(int x)
 * less_tst3(int x)
 * tripple_and_op(bool left, bool right, bool last)
 * andor_ops(bool a, bool b, bool c)
 * and_oper(bool left, bool right)
 * or_oper(bool l, bool r)
 * nested_eq(int arg)
 *  test cases:
 * nested_eq(10) == 2
 * nested_eq(1) == 1
 *  nested_eq() == 2
 */
   assert(false);
   check_exit();
}
