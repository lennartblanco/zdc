#include <stdlib.h>

#include "check_utils.h"

void *
def_init_orange();

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* def_init_orange() test */
    check_pointer("def_init_orange()", def_init_orange(), NULL);

    check_exit();
}
