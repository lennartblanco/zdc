#include "check_utils.h"

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

bool
call_greater_tst1()
{
   bool res;

   asm ("    call greater_tst1\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_greater_tst2(int in)
{
   bool res;

   asm ("    call greater_tst2\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_greater_tst3(int in)
{
   bool res;

   asm ("    call greater_tst3\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_less_tst1()
{
   bool res;

   asm ("    call less_tst1\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_less_tst2(int in)
{
   bool res;

   asm ("    call less_tst2\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_less_tst3(int in)
{
   bool res;

   asm ("    call less_tst3\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_and_oper(bool arg1, bool arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call and_oper\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

bool
call_or_oper(bool arg1, bool arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call or_oper\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

bool
call_tripple_and_op(bool arg1, bool arg2, bool arg3)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call tripple_and_op\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

bool
call_andor_ops(bool arg1, bool arg2, bool arg3)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call andor_ops\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_nested_eq(int in)
{
   int res;

   asm ("    call nested_eq\n"
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

    /* greater_tst1() test */
    check_bool("greater_tst1()", call_greater_tst1(), false);

    /* greater_tst2() tests */
    check_bool("greater_tst2(0)", call_greater_tst2(0), true);
    check_bool("greater_tst2(-1)", call_greater_tst2(-1), true);
    check_bool("greater_tst2(2)", call_greater_tst2(2), false);

    /* greater_tst3() tests */
    check_bool("greater_tst3(2)", call_greater_tst3(2), false);
    check_bool("greater_tst3(-1)", call_greater_tst3(-1), true);

    /* less_tst1() test */
    check_bool("less_tst1()", call_less_tst1(), true);

    /* less_tst2() tests */
    check_bool("less_tst2(2)", call_less_tst2(2), true);
    check_bool("less_tst2(-1)", call_less_tst2(-1), false);

    /* less_tst3() tests */
    check_bool("less_tst3(2)", call_less_tst3(2), true);
    check_bool("less_tst3(-1)", call_less_tst3(-1), false);

    /* and_oper() tests */
    check_bool("and_oper(false, false)", call_and_oper(false, false), false);
    check_bool("and_oper(true, false)", call_and_oper(true, false), false);
    check_bool("and_oper(false, true)", call_and_oper(false, true), false);
    check_bool("and_oper(true, true)", call_and_oper(true, true), true);

    /* or_oper() tests */
    check_bool("or_oper(false, false)", call_or_oper(false, false), false);
    check_bool("or_oper(true, false)", call_or_oper(true, false), true);
    check_bool("or_oper(false, true)", call_or_oper(false, true), true);
    check_bool("or_oper(true, true)", call_or_oper(true, true), true);

    /* tripple_and_op() tests */
    check_bool("tripple_and_op(false, false, false)",
               call_tripple_and_op(false, false, false),
               false);

    check_bool("tripple_and_op(false, true, false)",
               call_tripple_and_op(false, true, false),
               false);

    check_bool("tripple_and_op(true, true, false)",
               call_tripple_and_op(true, true, false),
               false);

    check_bool("tripple_and_op(true, true, true)",
               call_tripple_and_op(true, true, true),
               true);

    /* andor_ops() tests */
    check_bool("andor_ops(true, true, true)",
               call_andor_ops(true, true, true),
               true);

    check_bool("andor_ops(false, true, true)",
               call_andor_ops(false, true, true),
               true);

    check_bool("andor_ops(false, false, true)",
               call_andor_ops(false, false, true),
               true);

    check_bool("andor_ops(true, true, false)",
               call_andor_ops(true, true, false),
               true);

    check_bool("andor_ops(false, false, false)",
               call_andor_ops(false, false, false),
               false);

    /* nested_eq() tests */
    check_int("nested_eq(10)", call_nested_eq(10), 2);
    check_int("nested_eq(1)", call_nested_eq(1), 1);
    check_int("nested_eq(0)", call_nested_eq(0), 2);

   check_exit();
}
