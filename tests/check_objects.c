#include <stdlib.h>

#include "check_utils.h"

void *
def_init_orange();

unsigned
orange_set_get(unsigned inc, bool eaten);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* def_init_orange() test */
    check_pointer("def_init_orange()", def_init_orange(), NULL);

    /* orange_set_get() tests */
    check_uint("orange_set_get(2, true)",
               orange_set_get(2, true),
               87 + 2);
    check_uint("orange_set_get(7, false)",
               orange_set_get(7, false),
               100 - (87 + 7));

    check_exit();
}
