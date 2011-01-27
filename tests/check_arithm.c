#include <stdlib.h>
#include "check_utils.h"

int
call_signed_div_vars(int arg1, int arg2);

unsigned
call_unsigned_div_vars(unsigned arg1, unsigned arg2);

int
call_signed_div_lconst(int arg);

unsigned
call_unsigned_div_rconst(unsigned arg);

int
call_const_mod();

int
call_signed_mod_vars(int arg1, int arg2);

unsigned
call_unsigned_mod_vars(unsigned arg1, unsigned arg2);

int
call_signed_mod_lconst(int arg);

unsigned
call_unsigned_mod_rconst(unsigned arg);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* signed_div_vars() tests */
    check_int("signed_div_vars(20, 5)",
              call_signed_div_vars(20, 5), 20/5);
    check_int("signed_div_vars(-60, 19)",
              call_signed_div_vars(-60, 19), -60 / 19);
    check_int("signed_div_vars(16, -3)",
              call_signed_div_vars(16, -3), 16/-3);
    check_int("signed_div_vars(-1000, -100)",
              call_signed_div_vars(-1000, -100), -1000 / -100);
    check_int("signed_div_vars(0, -1)",
              call_signed_div_vars(0, -1), 0 /-1);

    /* unsigned_div_vars() tests */
    check_uint("unsigned_div_vars(200, 10)",
               call_unsigned_div_vars(200, 10), 200 / 10);
    check_uint("unsigned_div_vars(13, 20)",
               call_unsigned_div_vars(13, 20), 13 / 20);
    check_uint("unsigned_div_vars(1000, 88)",
               call_unsigned_div_vars(1000, 88), 1000 / 88);

    /* signed_div_lconst() tests */
    check_int("signed_div_lconst(3)", call_signed_div_lconst(3), 60/3);
    check_int("signed_div_lconst(7)", call_signed_div_lconst(7), 60/7);
    check_int("signed_div_lconst(-5)", call_signed_div_lconst(-5), 60/-5);

    /* unsigned_div_rconst() tests */
    check_uint("unsigned_div_rconst(297)",
               call_unsigned_div_rconst(297), 297/100);
    check_uint("unsigned_div_rconst(423)",
               call_unsigned_div_rconst(423), 423/100);
    check_uint("unsigned_div_rconst(3141500)",
               call_unsigned_div_rconst(3141500), 3141500/100);

    /* const_mod() test */
    check_int("const_mod()", call_const_mod(), 49 % 10);

    /* signed_mod_vars() tests */
    check_int("signed_mod_vars(20, 5)",
              call_signed_mod_vars(20, 5), 20 % 5);
    check_int("signed_mod_vars(-60, 19)",
              call_signed_mod_vars(-60, 19), -60 % 19);
    check_int("signed_mod_vars(16, -3)",
              call_signed_mod_vars(16, -3), 16 % -3);
    check_int("signed_mod_vars(-1034, -100)",
              call_signed_mod_vars(-1034, -100), -1034 % -100);

    /* unsigned_div_vars() tests */
    check_uint("unsigned_mod_vars(203, 10)",
               call_unsigned_mod_vars(203, 10), 203 % 10);
    check_uint("unsigned_mod_vars(13, 20)",
               call_unsigned_mod_vars(13, 20), 13 % 20);
    check_uint("unsigned_mod_vars(615, 5)",
               call_unsigned_mod_vars(615, 5), 615 % 5);

    /* signed_mod_lconst() tests */
    check_int("signed_mod_lconst(3)", call_signed_mod_lconst(3), 60 % 3);
    check_int("signed_mod_lconst(7)", call_signed_mod_lconst(7), 60 % 7);

    /* unsigned_div_rconst() tests */
    check_uint("unsigned_mod_rconst(297)",
               call_unsigned_mod_rconst(297), 297 % 100);
    check_uint("unsigned_mod_rconst(500)",
               call_unsigned_mod_rconst(500), 500 % 100);

    check_exit();
}

