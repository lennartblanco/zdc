#include "check_utils.h"

int
call_run_fact(bool arg);

int
call_run_misc();

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
