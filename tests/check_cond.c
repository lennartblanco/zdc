#include "check_utils.h"

int
abs(int n);

int
const_cond(int n);

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

    /* const_cond() tests */
    check_int("const_cond(-1)", const_cond(-1), 0);
    check_int("const_cond(0)", const_cond(0), 100);
    check_int("const_cond(1)", const_cond(1), 200);
    check_int("const_cond(2)", const_cond(2), 300);
    check_int("const_cond(3)", const_cond(3), 400);
    check_int("const_cond(4)", const_cond(4), 0);

    check_exit();
}
