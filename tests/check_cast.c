#include "check_utils.h"

bool
bool_to_bool(bool);

char
bool_to_char(bool);

char
char_to_char(char);

char
byte_to_char(signed char);

char
ubyte_to_char(unsigned char);

signed char
bool_to_byte(bool);

signed char
char_to_byte(char);

signed char
byte_to_byte(signed char);

signed char
ubyte_to_byte(unsigned char);

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
    /* bool_to_bool() tests */
    check_bool("bool_to_bool(true)", bool_to_bool(true), true);
    check_bool("bool_to_bool(false)", bool_to_bool(false), false);

    /* bool_to_char() tests */
    check_char("bool_to_char(true)", bool_to_char(true), '\1');
    check_char("bool_to_char(false)", bool_to_char(false), '\0');

    /* char_to_char() tests */
    check_char("char_to_char('\\0')", char_to_char('\0'), '\0');
    check_char("char_to_char('b')", char_to_char('b'), 'b');
    check_char("char_to_char('\\n')", char_to_char('\n'), '\n');

    /* byte_to_char() tests */
    check_char("byte_to_char(42)", byte_to_char(42), '*');
    check_char("byte_to_char(130)", byte_to_char(130), (char)130);

    /* ubyte_to_char() tests */
    check_char("ubyte_to_char(0)", ubyte_to_char(0), '\0');
    check_char("ubyte_to_char(32)", ubyte_to_char(32), ' ');

    /* bool_to_byte() tests */
    check_byte("bool_to_byte(true)", bool_to_byte(true), 1);
    check_byte("bool_to_byte(false)", bool_to_byte(false), 0);

    /* char_to_byte() tests */
    check_byte("char_to_byte('#')", '#', 35);
    check_byte("char_to_byte('m')", 'm', 109);

    /* byte_to_byte() tests */
    check_byte("byte_to_byte(-9)", byte_to_byte(-9), -9);
    check_byte("byte_to_byte(102)", byte_to_byte(102), 102);

    /* ubyte_to_byte() tests */
    check_byte("ubyte_to_byte(67)", ubyte_to_byte(67), 67);
    check_byte("ubyte_to_byte(199)", ubyte_to_byte(199), -57);

    /* cast_to_ubyte() tests */
    check_ubyte("cast_to_ubyte(0)", cast_to_ubyte(0), 0);
    check_ubyte("cast_to_ubyte(1)", cast_to_ubyte(1), 1);
    check_ubyte("cast_to_ubyte(2)", cast_to_ubyte(2), 123);
    check_ubyte("cast_to_ubyte(3)", cast_to_ubyte(3), 248);
    check_ubyte("cast_to_ubyte(4)", cast_to_ubyte(4), 97);
    check_ubyte("cast_to_ubyte(5)", cast_to_ubyte(5), 0);
    check_ubyte("cast_to_ubyte(6)", cast_to_ubyte(6), 0);
    check_ubyte("cast_to_ubyte(7)", cast_to_ubyte(7), 42);
    check_ubyte("cast_to_ubyte(8)", cast_to_ubyte(8), 255);

    /* cast_to_short() tests */
    check_short("cast_to_short(0)", cast_to_short(0), -10);
    check_short("cast_to_short(1)", cast_to_short(1), 10);
    check_short("cast_to_short(2)", cast_to_short(2), 100);
    check_short("cast_to_short(3)", cast_to_short(3), -2);
    check_short("cast_to_short(4)", cast_to_short(4), -1);

    /* cast_to_ushort() tests */
    check_ushort("cast_to_ushort(4)", cast_to_ushort(4), 65535);
    check_ushort("cast_to_ushort(3)", cast_to_ushort(3), 257);
    check_ushort("cast_to_ushort(2)", cast_to_ushort(2), 0);
    check_ushort("cast_to_ushort(1)", cast_to_ushort(1), 123);
    check_ushort("cast_to_ushort(0)", cast_to_ushort(0), 65532);

    /* cast_to_int() tests */
    check_int("cast_to_int(0)", cast_to_int(0), 17002);
    check_int("cast_to_int(1)", cast_to_int(1), -302);
    check_int("cast_to_int(2)", cast_to_int(2), 7);
    check_int("cast_to_int(3)", cast_to_int(3), -2);
    check_int("cast_to_int(4)", cast_to_int(4), -1);

    /* cast_to_uint() tests */
    check_uint("cast_to_uint(0)", cast_to_uint(0), 32000);
    check_uint("cast_to_uint(1)", cast_to_uint(1), 4294967293);
    check_uint("cast_to_uint(2)", cast_to_uint(2), 3);
    check_uint("cast_to_uint(3)", cast_to_uint(3), 2147483650);
    check_uint("cast_to_uint(4)", cast_to_uint(4), 0);

    check_exit();
}
