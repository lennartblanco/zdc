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

    check_exit();
}
