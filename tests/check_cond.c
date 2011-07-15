#include "check_utils.h"

int
abs(int n);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* abs() tests */
    check_int("abs(10)", abs(10), 10);
    check_int("abs(-315)", abs(-315), 315);
    check_int("abs(0)", abs(0), 0);

    check_exit();
}
