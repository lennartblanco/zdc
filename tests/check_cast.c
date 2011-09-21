#include "check_utils.h"

bool
cast_to_bool(unsigned int);

char
cast_to_char(unsigned int);

signed char
cast_to_byte(unsigned int);

unsigned char
cast_to_ubyte(unsigned int);

short
cast_to_short(unsigned int);

unsigned short
cast_to_ushort(unsigned int);

int
cast_to_int(unsigned int);

unsigned int
cast_to_uint(unsigned int);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* cast_to_booll() tests */
    check_bool("cast_to_bool(0)", cast_to_bool(0), true);
    check_bool("cast_to_bool(1)", cast_to_bool(1), false);
    check_bool("cast_to_bool(2)", cast_to_bool(2), false);
    check_bool("cast_to_bool(3)", cast_to_bool(3), true);
    check_bool("cast_to_bool(4)", cast_to_bool(4), true);
    check_bool("cast_to_bool(5)", cast_to_bool(5), false);
    check_bool("cast_to_bool(6)", cast_to_bool(6), false);
    check_bool("cast_to_bool(7)", cast_to_bool(7), true);
    check_bool("cast_to_bool(8)", cast_to_bool(8), true);
    check_bool("cast_to_bool(9)", cast_to_bool(9), true);
    check_bool("cast_to_bool(10)", cast_to_bool(10), false);
    check_bool("cast_to_bool(11)", cast_to_bool(11), true);
    check_bool("cast_to_bool(12)", cast_to_bool(12), false);
    check_bool("cast_to_bool(13)", cast_to_bool(13), false);
    check_bool("cast_to_bool(14)", cast_to_bool(14), true);
    check_bool("cast_to_bool(15)", cast_to_bool(15), false);
    check_bool("cast_to_bool(16)", cast_to_bool(16), true);
    check_bool("cast_to_bool(100)", cast_to_bool(100), false);

    /* cast_to_char() tests */
    check_char("cast_to_char(0)", cast_to_char(0), '\1');
    check_char("cast_to_char(1)", cast_to_char(1), '\0');
    check_char("cast_to_char(2)", cast_to_char(2), 255);
    check_char("cast_to_char(3)", cast_to_char(3), '\n');
    check_char("cast_to_char(4)", cast_to_char(4), 'b');
    check_char("cast_to_char(5)", cast_to_char(5), '*');
    check_char("cast_to_char(6)", cast_to_char(6), 255);
    check_char("cast_to_char(7)", cast_to_char(7), 127);
    check_char("cast_to_char(8)", cast_to_char(8), '\'');
    check_char("cast_to_char(9)", cast_to_char(9), '@');
    check_char("cast_to_char(10)", cast_to_char(10), 'l');
    check_char("cast_to_char(11)", cast_to_char(11), ' ');
    check_char("cast_to_char(12)", cast_to_char(12), 253);
    check_char("cast_to_char(13)", cast_to_char(13), '0');
    check_char("cast_to_char(100)", cast_to_char(100), 255);

    /* cast_to_byte() tests */
    check_byte("cast_to_byte(0)", cast_to_byte(0), 1);
    check_byte("cast_to_byte(1)", cast_to_byte(1), 0);
    check_byte("cast_to_byte(2)", cast_to_byte(2), 35);
    check_byte("cast_to_byte(3)", cast_to_byte(3), 109);
    check_byte("cast_to_byte(4)", cast_to_byte(4), -9);
    check_byte("cast_to_byte(5)", cast_to_byte(5), 102);
    check_byte("cast_to_byte(6)", cast_to_byte(6), 67);
    check_byte("cast_to_byte(7)", cast_to_byte(7), -57);
    check_byte("cast_to_byte(8)", cast_to_byte(8), 0);
    check_byte("cast_to_byte(9)", cast_to_byte(9), 0x34);
    check_byte("cast_to_byte(10)", cast_to_byte(10), 127);
    check_byte("cast_to_byte(11)", cast_to_byte(11), -126);
    check_byte("cast_to_byte(12)", cast_to_byte(12), -1);
    check_byte("cast_to_byte(13)", cast_to_byte(13), 42);
    check_byte("cast_to_byte(14)", cast_to_byte(14), 120);
    check_byte("cast_to_byte(15)", cast_to_byte(15), 0x44);
    check_byte("cast_to_byte(100)", cast_to_byte(100), 0);

    /* cast_to_ubyte() tests */
    check_ubyte("cast_to_ubyte(0)", cast_to_ubyte(0), 0);
    check_ubyte("cast_to_ubyte(1)", cast_to_ubyte(1), 1);
    check_ubyte("cast_to_ubyte(2)", cast_to_ubyte(2), 123);
    check_ubyte("cast_to_ubyte(3)", cast_to_ubyte(3), 248);
    check_ubyte("cast_to_ubyte(4)", cast_to_ubyte(4), 97);
    check_ubyte("cast_to_ubyte(5)", cast_to_ubyte(5), 0);
    check_ubyte("cast_to_ubyte(6)", cast_to_ubyte(6), 0);
    check_ubyte("cast_to_ubyte(7)", cast_to_ubyte(7), 42);
    check_ubyte("cast_to_ubyte(8)", cast_to_ubyte(8), 253);
    check_ubyte("cast_to_ubyte(9)", cast_to_ubyte(9), 31);
    check_ubyte("cast_to_ubyte(10)", cast_to_ubyte(10), 0);
    check_ubyte("cast_to_ubyte(11)", cast_to_ubyte(11), 240);
    check_ubyte("cast_to_ubyte(12)", cast_to_ubyte(12), 243);
    check_ubyte("cast_to_ubyte(13)", cast_to_ubyte(13), 2);
    check_ubyte("cast_to_ubyte(14)", cast_to_ubyte(14), 123);
    check_ubyte("cast_to_ubyte(15)", cast_to_ubyte(15), 6);
    check_ubyte("cast_to_ubyte(100)", cast_to_ubyte(100), 255);

    /* cast_to_short() tests */
    check_short("cast_to_short(0)", cast_to_short(0), -10);
    check_short("cast_to_short(1)", cast_to_short(1), 10);
    check_short("cast_to_short(2)", cast_to_short(2), 100);
    check_short("cast_to_short(3)", cast_to_short(3), -2);
    check_short("cast_to_short(4)", cast_to_short(4), 1);
    check_short("cast_to_short(5)", cast_to_short(5), 0);
    check_short("cast_to_short(6)", cast_to_short(6), 0xff);
    check_short("cast_to_short(7)", cast_to_short(7), 109);
    check_short("cast_to_short(8)", cast_to_short(8), 0);
    check_short("cast_to_short(9)", cast_to_short(9), 110);
    check_short("cast_to_short(10)", cast_to_short(10), -200);
    check_short("cast_to_short(11)", cast_to_short(11), 0xcdef);
    check_short("cast_to_short(12)", cast_to_short(12), 14234);
    check_short("cast_to_short(13)", cast_to_short(13), -32767);
    check_short("cast_to_short(100)", cast_to_short(100), -1);

    /* cast_to_ushort() tests */
    check_ushort("cast_to_ushort(100)", cast_to_ushort(100), 65535);
    check_ushort("cast_to_ushort(13)", cast_to_ushort(13), 0xf);
    check_ushort("cast_to_ushort(12)", cast_to_ushort(12), 67);
    check_ushort("cast_to_ushort(11)", cast_to_ushort(11), 2);
    check_ushort("cast_to_ushort(10)", cast_to_ushort(10), 23000);
    check_ushort("cast_to_ushort(9)", cast_to_ushort(9), 123);
    check_ushort("cast_to_ushort(8)", cast_to_ushort(8), 23);
    check_ushort("cast_to_ushort(7)", cast_to_ushort(7), 32);
    check_ushort("cast_to_ushort(6)", cast_to_ushort(6), 0xff);
    check_ushort("cast_to_ushort(5)", cast_to_ushort(5), 0);
    check_ushort("cast_to_ushort(4)", cast_to_ushort(4), 1);
    check_ushort("cast_to_ushort(3)", cast_to_ushort(3), 257);
    check_ushort("cast_to_ushort(2)", cast_to_ushort(2), 0);
    check_ushort("cast_to_ushort(1)", cast_to_ushort(1), 123);
    check_ushort("cast_to_ushort(0)", cast_to_ushort(0), 65532);

    /* cast_to_int() tests */
    check_int("cast_to_int(0)", cast_to_int(0), 17002);
    check_int("cast_to_int(1)", cast_to_int(1), -302);
    check_int("cast_to_int(2)", cast_to_int(2), 7);
    check_int("cast_to_int(3)", cast_to_int(3), -2);
    check_int("cast_to_int(4)", cast_to_int(4), 1);
    check_int("cast_to_int(5)", cast_to_int(5), 0);
    check_int("cast_to_int(6)", cast_to_int(6), 10);
    check_int("cast_to_int(7)", cast_to_int(7), 0xff);
    check_int("cast_to_int(8)", cast_to_int(8), 3);
    check_int("cast_to_int(9)", cast_to_int(9), 254);
    check_int("cast_to_int(10)", cast_to_int(10), 300);
    check_int("cast_to_int(11)", cast_to_int(11), 257);
    check_int("cast_to_int(100)", cast_to_int(100), -1);

    /* cast_to_uint() tests */
    check_uint("cast_to_uint(0)", cast_to_uint(0), 32000);
    check_uint("cast_to_uint(1)", cast_to_uint(1), 4294967293);
    check_uint("cast_to_uint(2)", cast_to_uint(2), 3);
    check_uint("cast_to_uint(3)", cast_to_uint(3), 2147483650);
    check_uint("cast_to_uint(4)", cast_to_uint(4), 1);
    check_uint("cast_to_uint(5)", cast_to_uint(5), 0);
    check_uint("cast_to_uint(6)", cast_to_uint(6), 0xff);
    check_uint("cast_to_uint(7)", cast_to_uint(7), 102);
    check_uint("cast_to_uint(8)", cast_to_uint(8), 254);
    check_uint("cast_to_uint(9)", cast_to_uint(9), 56);
    check_uint("cast_to_uint(10)", cast_to_uint(10), 1234);
    check_uint("cast_to_uint(11)", cast_to_uint(11), 0);
    check_uint("cast_to_uint(100)", cast_to_uint(100), 0);

    check_exit();
}
