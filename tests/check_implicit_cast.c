#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_bool_to_int(bool in)
{
   int res;

   asm ("    call _D13implicit_cast11bool_to_intFbZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_int0_to_bool()
{
   bool res;

   asm ("    call _D13implicit_cast12int0_to_boolFZb\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_int1_to_bool()
{
   bool res;

   asm ("    call _D13implicit_cast12int1_to_boolFZb\n"
        : "=a"(res)
        : );

   return res;
}

int
call_add_bools(bool arg1, bool arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D13implicit_cast9add_boolsFbbZi\n"
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
    /* bool_to_int() tests */
    check_int("bool_to_int(false)", call_bool_to_int(false), 0);
    check_int("bool_to_int(true)", call_bool_to_int(true), 1);

    /* int0_to_bool() test */
    check_bool("int0_to_bool()", call_int0_to_bool(), false);

    /* int1_to_bool() test */
    check_bool("int1_to_bool()", call_int1_to_bool(), true);

    /* add_bools() tests */
    check_int("add_bools(false, false)", call_add_bools(false, false), 0);
    check_int("add_bools(false, true)", call_add_bools(false, true), 1);
    check_int("add_bools(true, false)", call_add_bools(true, false), 2);
    check_int("add_bools(true, true)", call_add_bools(true, true), 3);


    check_exit();
}
