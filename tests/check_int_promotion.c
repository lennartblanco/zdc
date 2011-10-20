#include "check_utils.h"

int
integer_promotions(unsigned testno, int a);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* integer_promotions() tests */
    check_int("integer_promotions(100, 0)", integer_promotions(100, 0), -1);
    check_int("integer_promotions(0, 1)", integer_promotions(0, 1), 1 + 'x');
    check_int("integer_promotions(1, 10)", integer_promotions(1, 10), 9);
    check_int("integer_promotions(2, 5)", integer_promotions(2, 5), 5 * -2);
    check_int("integer_promotions(3, 40)", integer_promotions(3, 40), 40/5);
    check_int("integer_promotions(4, 0)", integer_promotions(4, 0), 10 + 0);
    check_int("integer_promotions(5, 101)",
              integer_promotions(5, 101), 100 - 101);
    check_int("integer_promotions(6, 25)", integer_promotions(6, 25), 8 * 25);
    check_int("integer_promotions(7, 4)", integer_promotions(7, 4), 200 / 4);

    check_exit();
}
