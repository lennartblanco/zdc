#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_hej()
{
   int res;

   asm ("    call hej\n"
        : "=a"(res)
        : );

   return res;
}

int
call_get_13()
{
   int res;

   asm ("    call get_13\n"
        : "=a"(res)
        : );

   return res;
}

int
call_foo()
{
   int res;

   asm ("    call foo\n"
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
        "    call add\n"
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
        "    call sum\n"
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

   asm ("    call ind_sum\n"
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
        "    call subst3\n"
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
        "    call subst4\n"
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

   asm ("    call add_13\n"
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

    check_exit();
}
