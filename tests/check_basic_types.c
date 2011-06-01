#include "check_utils.h"

short
call_short_vars(unsigned);

unsigned short
call_ushort_vars(unsigned);

char
call_byte_vars(unsigned);

unsigned char
call_ubyte_vars(unsigned);

int
misc_to_int(unsigned);

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

    /* byte_vars() tests */
    check_byte("byte_vars(0)", call_byte_vars(0), 0);
    check_byte("byte_vars(1)", call_byte_vars(1), 0);
    check_byte("byte_vars(2)", call_byte_vars(2), 10);
    check_byte("byte_vars(3)", call_byte_vars(3), 27);
    check_byte("byte_vars(4)", call_byte_vars(4), -120);
    check_byte("byte_vars(5)", call_byte_vars(5), 10);
    check_byte("byte_vars(6)", call_byte_vars(6), -120);
    check_byte("byte_vars(7)", call_byte_vars(7), 20);

    /* ubyte_vars() tests */
    check_ubyte("ubyte_vars(0)", call_ubyte_vars(0), 0);
    check_ubyte("ubyte_vars(1)", call_ubyte_vars(1), 0);
    check_ubyte("ubyte_vars(2)", call_ubyte_vars(2), 3);
    check_ubyte("ubyte_vars(3)", call_ubyte_vars(3), 111);
    check_ubyte("ubyte_vars(4)", call_ubyte_vars(4), 3);
    check_ubyte("ubyte_vars(5)", call_ubyte_vars(5), 111);
    check_ubyte("ubyte_vars(6)", call_ubyte_vars(6), 20);

    /* misc_to_int() tests */
    check_int("misc_to_int(-1)", misc_to_int(-1), -1);
    check_int("misc_to_int(0)", misc_to_int(0), 3);
    check_int("misc_to_int(1)", misc_to_int(1), 1);
    check_int("misc_to_int(2)", misc_to_int(2), 4);
    check_int("misc_to_int(3)", misc_to_int(3), 1);
    check_int("misc_to_int(4)", misc_to_int(4), 5);
    check_int("misc_to_int(5)", misc_to_int(5), -1);

    check_exit();
}
