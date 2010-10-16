#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_hej()
{
   int res;

   asm ("    call _D9func_call3hejFZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_get_13()
{
   int res;

   asm ("    call _D9func_call6get_13FZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_foo()
{
   int res;

   asm ("    call _D9func_call3fooFZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_add(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D9func_call3addFiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_sum(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call _D9func_call3sumFiiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_ind_sum(int in)
{
   int res;

   asm ("    call _D9func_call7ind_sumFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_subst3(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    call _D9func_call6subst3FiiiZi\n"
        : "=a"(res)
        : "a" (arg3),
          [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_subst4(int arg1, int arg2, int arg3, int arg4)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    pushl %[arg3]\n"
        "    call _D9func_call6subst4FiiiiZi\n"
        : "=a"(res)
        : "a" (arg4),
          [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_add_13(int in)
{
   int res;

   asm ("    call _D9func_call6add_13FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_invoke_subst3(int in)
{
   int res;

   asm ("    call _D9func_call13invoke_subst3FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_unnamed_arg1(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D9func_call12unnamed_arg1FiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_unnamed_arg2(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D9func_call12unnamed_arg2FiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_unnamed_arg3(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call _D9func_call12unnamed_arg3FibkZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_unnamed_arg4(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call _D9func_call12unnamed_arg4FiiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

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

    check_exit();
}
