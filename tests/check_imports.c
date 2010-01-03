#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_run_fact(bool in)
{
   int res;

   asm ("    call _D7imports8run_factFbZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_run_misc()
{
   int res;

   asm ("    call _D7imports8run_miscFZi\n"
        : "=a"(res)
        : );

   return res;
}

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* run_fact() tests */
    check_int("run_fact(true)", call_run_fact(true), 126);
    check_int("run_fact(false)", call_run_fact(false), 120);

    /* run_misc() test */
    check_int("run_misc()", call_run_misc(), -10);

    check_exit();
}
