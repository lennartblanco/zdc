#include "check_utils.h"

unsigned
string_tst(bool testflag);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* string_tst1() tests */
    check_uint("string_tst(true)", string_tst(true), 7);
    check_uint("string_tst(false)", string_tst(false), 0);

    check_exit();
}
