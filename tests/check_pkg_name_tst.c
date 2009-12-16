#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_first_func()
{
   int res;

   asm ("    call _D1a4test3pkg4name10first_funcFZi\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_second_func(int in)
{
   bool res;

   asm ("    call _D1a4test3pkg4name11second_funcFiZb\n"
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
    /* first_func() test */
    check_int("first_func()", call_first_func(), 1969);

    /* second_func() tests */
    check_bool("second_func(true)", call_second_func(true), false);
    check_bool("second_func(false)", call_second_func(false), true);

    check_exit();
}
