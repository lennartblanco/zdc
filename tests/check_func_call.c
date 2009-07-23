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

    check_exit();
}
