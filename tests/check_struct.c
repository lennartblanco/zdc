#include "check_utils.h"

unsigned
orange_sizeof();

unsigned
list_sizeof();

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* orange_sizeof() test */
    check_uint("orange_sizeof()", orange_sizeof(), 9);

    /* list_sizeof() test */
    check_uint("list_sizeof()", list_sizeof(), 13);

    check_exit();
}
