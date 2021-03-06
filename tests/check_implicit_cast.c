#include <stddef.h>
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

void*
call_int_ptr_to_void_ptr(int *p);

int
misc_to_int(unsigned);

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

    /* call_int_ptr_to_void_ptr() tests */
    check_pointer("int_ptr_to_void_ptr(NULL)",
                  call_int_ptr_to_void_ptr(NULL),
                  NULL);
    check_pointer("int_ptr_to_void_ptr(0xf0f1)",
                  call_int_ptr_to_void_ptr(0xf0f1),
                  0xf0f1);

    /* misc_to_int() tests */
    check_int("misc_to_int(-1)", misc_to_int(-1), -1);
    check_int("misc_to_int(100)", misc_to_int(100), -1);
    check_int("misc_to_int(0)", misc_to_int(0), 3);
    check_int("misc_to_int(1)", misc_to_int(1), 1);
    check_int("misc_to_int(2)", misc_to_int(2), 4);
    check_int("misc_to_int(3)", misc_to_int(3), 1);
    check_int("misc_to_int(4)", misc_to_int(4), 5);
    check_int("misc_to_int(5)", misc_to_int(5), 112);
    check_int("misc_to_int(6)", misc_to_int(6), 112 + 2);

    check_exit();
}
