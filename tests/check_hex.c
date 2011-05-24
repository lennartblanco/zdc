#include "check_utils.h"

int
hex_int(int);

unsigned
hex_uint(unsigned idx);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* hex_int() tests */
    check_int("hex_int(-1)", hex_int(-1), -1);
    check_int("hex_int(0)", hex_int(0), 0);
    check_int("hex_int(1)", hex_int(1), 0xfa);
    check_int("hex_int(2)", hex_int(2), 0x7fffffff);
    check_int("hex_int(3)", hex_int(3), 0x4000);
    check_int("hex_int(4)", hex_int(4), 0xfa - 0xf0);
    check_int("hex_int(5)", hex_int(5), 0x0f0f);
    check_int("hex_int(6)", hex_int(6), -1);

    /* hex_uint() tests */
    check_uint("hex_uint(0)", hex_uint(0), 0xa3u);
    check_uint("hex_uint(1)", hex_uint(1), 0x1ffu);
    check_uint("hex_uint(2)", hex_uint(2), 0x80000000u + 1);
    check_uint("hex_uint(3)", hex_uint(3), 0x80001230u);
    check_uint("hex_uint(4)", hex_uint(4), 0xfafu);
    check_uint("hex_uint(5)", hex_uint(5), 0x0);

    check_exit();
}
