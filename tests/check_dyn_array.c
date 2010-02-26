#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_int_array_lit_assigment(int in)
{
   int res;

   asm ("    call _D9dyn_array23int_array_lit_assigmentFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_int_array_lit_init(int in)
{
   int res;

   asm ("    call _D9dyn_array18int_array_lit_initFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_intops(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D9dyn_array6intopsFiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

bool
call_bool_array_lit_assigment(int in)
{
   bool res;

   asm ("    call _D9dyn_array24bool_array_lit_assigmentFiZb\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_bool_array_lit_init(bool in)
{
   bool res;

   asm ("    call _D9dyn_array19bool_array_lit_initFbZb\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_boolops(int arg1, int arg2)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D9dyn_array7boolopsFiiZb\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_invoke_dyn_array_sum(int in)
{
   int res;

   asm ("    call _D9dyn_array20invoke_dyn_array_sumFkZi\n"
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
    /* int_array_lit_assigment() tests */
    check_int("int_array_lit_assigment(0)",
              call_int_array_lit_assigment(0), 3 + 3);
    check_int("int_array_lit_assigment(1)",
              call_int_array_lit_assigment(1), 3 + 1);
    check_int("int_array_lit_assigment(2)",
              call_int_array_lit_assigment(2), 3 + 4);

    /* int_array_lit_init() tests */
    check_int("int_array_lit_init(0)",
              call_int_array_lit_init(0), 2);
    check_int("int_array_lit_init(1)",
              call_int_array_lit_init(1), 7);
    check_int("int_array_lit_init(2)",
              call_int_array_lit_init(2), 1);
    check_int("int_array_lit_init(3)",
              call_int_array_lit_init(3), 8);
    check_int("int_array_lit_init(3)",
              call_int_array_lit_init(4), 2);

    /* intops() tests */
    check_int("intops(0, 20)", call_intops(0, 20), 20 + 2 + 3);
    check_int("intops(1, 0)", call_intops(1, 0), 1 + 0 + 3);
    check_int("intops(2, -20)", call_intops(2, -20), 1 + 2 + (-20));

    /* bool_array_lit_assigment() tests */
    check_bool("bool_array_lit_assigment(0)",
               call_bool_array_lit_assigment(0), true);
    check_bool("bool_array_lit_assigment(1)",
               call_bool_array_lit_assigment(1), false);
    check_bool("bool_array_lit_assigment(2)",
               call_bool_array_lit_assigment(2), true);
    check_bool("bool_array_lit_assigment(3)",
               call_bool_array_lit_assigment(3), false);

    /* bool_array_lit_assigment() tests */
    check_bool("bool_array_lit_init(true)",
               call_bool_array_lit_init(true), true);
    check_bool("bool_array_lit_init(false)",
               call_bool_array_lit_init(false), false);

    /* boolops() tests */
    check_bool("boolops(0, -1)", call_boolops(0, -1), false);
    check_bool("boolops(1, 10)", call_boolops(1, 10), true);
    check_bool("boolops(2, 0)", call_boolops(2, 0), false);

    /* dyn_array_sum() tests */
    check_int("invoke_dyn_array_sum(0)",
              call_invoke_dyn_array_sum(0), 1 + 10 + 15 + 0 + 5);

    check_int("invoke_dyn_array_sum(1)",
              call_invoke_dyn_array_sum(1), 5 -12);

    check_int("invoke_dyn_array_sum(2)",
              call_invoke_dyn_array_sum(2), 0);

    check_int("invoke_dyn_array_sum(3)",
              call_invoke_dyn_array_sum(3), -1);

    check_exit();
}
