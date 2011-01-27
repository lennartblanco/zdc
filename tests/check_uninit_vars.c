#include "check_utils.h"

bool
call_uninit_bool();

unsigned char
call_uninit_char();

unsigned char
call_uninit_int();

unsigned char
call_uninit_uint();

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* uninit_bool() test */
    check_bool("uninit_bool()", call_uninit_bool(), false);

    /* uninit_char() test */
    check_char("uninit_char()", call_uninit_char(), 255);

    /* uninit_int() test */
    check_int("uninit_int()", call_uninit_int(), 0);

    /* uninit_uint() test */
    check_int("uninit_uint()", call_uninit_uint(), 0);

    check_exit();
}
