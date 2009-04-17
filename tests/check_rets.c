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
   asm("call void_return");
}

int
call_int_ret()
{
   int res;

   asm ("    call int_ret\n"
        : "=a"(res)
        : );

   return res;
}

int
call_iret_val(int in)
{
   int res;

   asm ("    call iret_val\n"
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
        "    call iret_arg1\n"
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
        "    call iret_arg1_with_ret_var\n"
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
        "    call iret_arg2\n"
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
        "    call iret_arg3\n"
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


    check_exit();
}
