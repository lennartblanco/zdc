#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

/*
 * The calling convention for void or int functions without any arguments
 * is identical for C and D (without name mangling, which is not
 * implemented at the moment)
 */
void
call_void_return()
{
   asm("call _D4rets11void_returnFZv");
}

void
call_printNum(int arg)
{
   asm("call _D4rets8printNumFiZv"
       :
       : "a"(arg));
}

int
call_int_ret()
{
   int res;

   asm ("    call _D4rets7int_retFZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_iret_val(int in)
{
   int res;

   asm ("    call _D4rets8iret_valFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_iret_arg1(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call _D4rets9iret_arg1FiiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_iret_arg1_with_ret_var(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call _D4rets22iret_arg1_with_ret_varFiiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}


int
call_iret_arg2(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call _D4rets9iret_arg2FiiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_iret_arg3(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3],%%eax\n"
        "    call _D4rets9iret_arg3FiiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

void
call_no_ret_void(int arg1, int arg2)
{
   asm ("    pushl %[arg1]\n"
        "    call _D4rets11no_ret_voidFiiZv\n"
        "    add $4, %%esp\n" // clear call arguments from the stack
        :
        : [arg1]"m"(arg1),
          "a"(arg2));
}

void
call_empty_func()
{
   asm("call _D4rets10empty_funcFZv");
}

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
    check_int("iret_val(-2)", call_iret_val(1984), 1984);

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
