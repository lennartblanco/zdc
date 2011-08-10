#include "check_utils.h"

int
foreach_range(int lower, int upper);

int
foreach_range_break(int init, int max);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* foreach_range() tests */
    check_int("foreach_range(2, 5)", foreach_range(2, 5), 2 + 3 + 4);
    check_int("foreach_range(-3, 2)", foreach_range(-3, 2),
              -3 + -2 + -1 + 0 + 1);
    check_int("foreach_range(-10, -2)", foreach_range(-10, -2),
              -10 + -9 + -8 + -7 + -6 + -5 + -4 + -3);
    check_int("foreach_range(23, 23)", foreach_range(23, 23), 0);
    check_int("foreach_range(100, 43)", foreach_range(100, 43), 0);

    /* foreach_range_break() tests */
    check_int("foreach_range_break(40, 100)",
              foreach_range_break(40, 90), 50);
    check_int("foreach_range_break(-20, 0)",
              foreach_range_break(-20, 0), -10);
    check_int("foreach_range_break(200, 205)",
              foreach_range_break(200, 205), 205);
    check_int("foreach_range_break(300, 0)",
              foreach_range_break(300, 0), 300);

    check_exit();
}
