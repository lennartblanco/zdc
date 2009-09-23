#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_init_int_array(int in)
{
   int res;

   asm ("    call init_int_array\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_init_bool_array(int in)
{
   bool res;

   asm ("    call init_bool_array\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_def_init_int_array(int in)
{
   int res;

   asm ("    call def_init_int_array\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_def_init_bool_array(int in)
{
   bool res;

   asm ("    call def_init_bool_array\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_init_exp_tst(int in)
{
   int res;

   asm ("    call init_exp_tst\n"
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
        "    call intops\n"
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
    int i;

    /* init_int_array() tests */
    check_int("init_int_array(0)", call_init_int_array(0), 10);
    check_int("init_int_array(1)", call_init_int_array(1), 11);
    check_int("init_int_array(2)", call_init_int_array(2), 12);
    check_int("init_int_array(3)", call_init_int_array(3), 13);

    /* init_bool_array() tests */
    check_bool("init_bool_array(0)", call_init_bool_array(0), true);
    check_bool("init_bool_array(1)", call_init_bool_array(1), true);
    check_bool("init_bool_array(2)", call_init_bool_array(2), false);
    check_bool("init_bool_array(3)", call_init_bool_array(3), false);

    /* def_init_int_array() tests */
    for (i = 0; i < 6; i++)
    {
        check_int("def_init_int_array(i)", call_def_init_int_array(i), 0);
    }

    /* def_init_bool_array() tests */
    for (i = 0; i < 8; i++)
    {
        check_bool("def_init_bool_array(i)", call_def_init_bool_array(i), 0);
    }

    /* init_exp_tst() tests */
    check_int("init_exp_tst(2)", call_init_exp_tst(2), (2 + 2 - 15) * 2);
    check_int("init_exp_tst(2)", call_init_exp_tst(100), (100 + 2 - 15) * 100);

    /* intops() tests */
    check_int("intops(3, 1)", call_intops(3, 1), 3 + 20 + 30 + 20);
    check_int("intops(-12, 2)", call_intops(-12, 2), -12 + 20 + 30 + 30);

    check_exit();
}

