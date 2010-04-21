#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_bool_to_int(bool in)
{
    int res;

    asm ("    call _D13implicit_cast11bool_to_intFbZi\n"
         : "=a"(res)
         : "a"(in));

    return res;
}

bool
call_int0_to_bool()
{
    bool res;

    asm ("    call _D13implicit_cast12int0_to_boolFZb\n"
         : "=a"(res)
         : );

    return res;
}

bool
call_int1_to_bool()
{
    bool res;

    asm ("    call _D13implicit_cast12int1_to_boolFZb\n"
         : "=a"(res)
         : );

    return res;
}

int
call_add_bools(bool arg1, bool arg2)
{
    int res;

    asm ("    pushl %[arg1]\n"
         "    movl %[arg2],%%eax\n"
         "    call _D13implicit_cast9add_boolsFbbZi\n"
         : "=a"(res)
         : [arg1]"m"(arg1),
           [arg2]"m"(arg2));

    return res;
}

int
call_char_to_int(char arg1)
{
    int res;

    asm ("    call _D13implicit_cast11char_to_intFaZi\n"
         : "=a"(res)
         : "a"(arg1));

    return res;
}

int
char_lit_to_int()
{
    int res;

    asm ("    call _D13implicit_cast15char_lit_to_intFZi\n"
         : "=a"(res)
         : );

    return res;
}

unsigned
call_char_to_uint(char arg1)
{
    unsigned res;

    asm ("    call _D13implicit_cast12char_to_uintFaZk\n"
         : "=a"(res)
         : "a"(arg1));

    return res;
}

unsigned
char_lit_to_uint()
{
    unsigned res;

    asm ("    call _D13implicit_cast16char_lit_to_uintFZk\n"
         : "=a"(res)
         : );

    return res;
}


/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* bool_to_int() tests */
    check_int("bool_to_int(false)", call_bool_to_int(false), 0);
    check_int("bool_to_int(true)", call_bool_to_int(true), 1);

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
    check_int("char_lit_to_int()", char_lit_to_int(), (int)'h');

    /* char_to_uint() tests */
    check_uint("char_to_uint('M')", call_char_to_uint('M'), (unsigned)'M');
    check_uint("char_to_uint('l')", call_char_to_uint('l'), (unsigned)'l');
    check_uint("char_to_uint('\\0')", call_char_to_uint('\0'), 0u);

    /* char_lit_to_uint() test */
    check_uint("char_lit_to_uint()", char_lit_to_uint(), (unsigned)'Z');

    check_exit();
}
