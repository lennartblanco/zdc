#include "check_utils.h"

void
call_depl(unsigned arg);

bool
call_boom(int arg1, int arg2);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* depl() test */
    call_depl(23);
    check("depl(23)");

    /* boom() tests */
    check_bool("boom(2, 3)", call_boom(2, 3), (2 - 3) == 0);
    check_bool("boom(-12, -12)", call_boom(-12, -12), true);


    check_exit();
}
