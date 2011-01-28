#include "check_utils.h"

int
call_bool_to_int(bool arg);

unsigned char
call_bool_to_char(bool arg);

bool
call_int0_to_bool();

bool
call_int1_to_bool();

int
call_add_bools(int arg1, int arg2);

int
call_char_to_int(char arg1);

int
call_char_lit_to_int();

unsigned
call_char_to_uint(char arg1);

unsigned
call_char_lit_to_uint();


/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* bool_to_int() tests */
    check_int("bool_to_int(false)", call_bool_to_int(false), 0);
    check_int("bool_to_int(true)", call_bool_to_int(true), 1);

    /* bool_to_char() tests */
    check_char("bool_to_char(false)", call_bool_to_char(false), '\0');
    check_char("bool_to_char(true)", call_bool_to_char(true), '\1');

    /* int0_to_bool() test */
    check_bool("int0_to_bool()", call_int0_to_bool(), false);

    /* int1_to_bool() test */
    check_bool("int1_to_bool()", call_int1_to_bool(), true);

    /* add_bools() tests */
    check_int("add_bools(false, false)", call_add_bools(false, false), 0);
    check_int("add_bools(false, true)", call_add_bools(false, true), 1);
    check_int("add_bools(true, false)", call_add_bools(true, false), 2);
    check_int("add_bools(true, true)", call_add_bools(true, true), 3);

    /* char_to_int() tests */
    check_int("char_to_int('x')", call_char_to_int('x'), (int)'x');
    check_int("char_to_int('Q')", call_char_to_int('Q'), (int)'Q');
    check_int("char_to_int('\\0')", call_char_to_int('\0'), 0);

    /* char_lit_to_int() test */
    check_int("char_lit_to_int()", call_char_lit_to_int(), (int)'h');

    /* char_to_uint() tests */
    check_uint("char_to_uint('M')", call_char_to_uint('M'), (unsigned)'M');
    check_uint("char_to_uint('l')", call_char_to_uint('l'), (unsigned)'l');
    check_uint("char_to_uint('\\0')", call_char_to_uint('\0'), 0u);

    /* char_lit_to_uint() test */
    check_uint("char_lit_to_uint()", call_char_lit_to_uint(), (unsigned)'Z');

    check_exit();
}
