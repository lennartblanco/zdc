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

bool
call_bool_array_lit_assigment(int in)
{
   bool res;

   asm ("    call _D9dyn_array24bool_array_lit_assigmentFiZb\n"
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

    /* bool_array_lit_assigment() tests */
    check_bool("bool_array_lit_assigment(0)",
               call_bool_array_lit_assigment(0), true);
    check_bool("bool_array_lit_assigment(1)",
               call_bool_array_lit_assigment(1), false);
    check_bool("bool_array_lit_assigment(2)",
               call_bool_array_lit_assigment(2), true);
    check_bool("bool_array_lit_assigment(3)",
               call_bool_array_lit_assigment(3), false);

    check_exit();
}
