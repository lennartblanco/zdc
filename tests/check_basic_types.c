#include "check_utils.h"

short
call_short_vars(unsigned);

unsigned short
call_ushort_vars(unsigned);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* short_vars() tests */
    check_short("short_vars(0)", call_short_vars(0), 0);
    check_short("short_vars(1)", call_short_vars(1), 0);
    check_short("short_vars(2)", call_short_vars(2), 10);
    check_short("short_vars(3)", call_short_vars(3), 23);
    check_short("short_vars(4)", call_short_vars(4), 32762);
    check_short("short_vars(5)", call_short_vars(5), 10);
    check_short("short_vars(6)", call_short_vars(6), 32762);
    check_short("short_vars(7)", call_short_vars(7), 20);
    check_short("short_vars(8)", call_short_vars(8), 0);

    /* ushort_vars() tests */
    check_ushort("ushort_vars(0)", call_ushort_vars(0), 23);
    check_ushort("ushort_vars(1)", call_ushort_vars(1), 2);
    check_ushort("ushort_vars(2)", call_ushort_vars(2), 100);
    check_ushort("ushort_vars(3)", call_ushort_vars(3), 2);
    check_ushort("ushort_vars(4)", call_ushort_vars(4), 100);
    check_ushort("ushort_vars(5)", call_ushort_vars(5), 10);
    check_ushort("ushort_vars(6)", call_ushort_vars(6), 0);
    check_ushort("ushort_vars(7)", call_ushort_vars(7), 0);

    check_exit();
}
