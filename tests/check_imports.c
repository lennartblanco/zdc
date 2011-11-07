#include "check_utils.h"

int
call_run_fact(bool arg);

int
call_run_misc();

int
call_run_hex_int(bool arg);

int
call_get_hex_bit_mask();

unsigned
call_lemon_sizeof();

bool
call_age_lemon();

unsigned
call_orange_alias(unsigned a);

int
call_run_imp_func1(bool flag);

bool
call_run_invert(bool arg);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* run_fact() tests */
    check_int("run_fact(true)", call_run_fact(true), 126);
    check_int("run_fact(false)", call_run_fact(false), 120);

    /* run_misc() test */
    check_int("run_misc()", call_run_misc(), -10);

    /* run_hex_int() tests */
    check_int("run_hex_int(true)", call_run_hex_int(true), 0);
    check_int("run_hex_int(false)", call_run_hex_int(false), -1);

    /* get_hex_bit_mask() test */
    check_int("get_hex_bit_mask()", call_get_hex_bit_mask(), 0x00000F0F);

    /* lemon_sizeof() test */
    check_uint("lemon_sizeof()", call_lemon_sizeof(), 8);

    /* age_lemon() test */
    check_bool("age_lemon()", call_age_lemon(), true);

    /* orange_alias() tests */
    check_uint("orange_alias(5)", call_orange_alias(5), 100 - 5);
    check_uint("orange_alias(43)", call_orange_alias(43), 100 - 43);

    /* run_imp_func1() tests */
    check_int("run_imp_func1(true)", call_run_imp_func1(true), 314);
    check_int("run_imp_func1(false)", call_run_imp_func1(false), 0);

    /* run_invert() tests */
    check_bool("run_invert(true)", call_run_invert(true), !true);
    check_bool("run_invert(false)", call_run_invert(false), !false);

    check_exit();
}
