#include "check_utils.h"

unsigned
call_int_var_sizeof();

unsigned
call_uint_var_sizeof();

unsigned
call_bool_var_sizeof();

unsigned
call_char_var_sizeof();

unsigned
call_int_exp_sizeof();

unsigned
call_bool_exp_sizeof();

unsigned
call_stat_int_arry_var_sizeof();

unsigned
call_stat_bool_arry_var_sizeof();

unsigned
call_dyn_int_arry_var_sizeof();

int
call_int_var_init();

unsigned
call_uint_var_init();

bool
call_bool_var_init();

unsigned char
call_char_var_init();

unsigned
call_stat_int_arry_length();

unsigned
call_stat_bool_arry_length();

unsigned
call_dyn_int_arry_length(bool arg);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* int_var_sizeof() test */
    check_uint("int_var_sizeof()", call_int_var_sizeof(), 4);

    /* uint_var_sizeof() test */
    check_uint("uint_var_sizeof()", call_uint_var_sizeof(), 4);

    /* bool_var_sizeof() test */
    check_uint("bool_var_sizeof()", call_bool_var_sizeof(), 1);

    /* char_var_sizeof() test */
    check_uint("char_var_sizeof()", call_char_var_sizeof(), 1);

    /* int_exp_sizeof() test */
    check_uint("int_exp_sizeof()", call_int_exp_sizeof(), 4);

    /* bool_exp_sizeof() test */
    check_uint("bool_exp_sizeof()", call_bool_exp_sizeof(), 1);

    /* stat_int_arry_var_sizeof() test */
    check_uint("stat_int_arry_var_sizeof()",
               call_stat_int_arry_var_sizeof(), 16);

    /* stat_bool_arry_var_sizeof() test */
    check_uint("stat_bool_arry_var_sizeof()",
               call_stat_bool_arry_var_sizeof(), 7);

    /* dyn_int_arry_var_sizeof() test */
    check_uint("dyn_int_arry_var_sizeof()",
               call_dyn_int_arry_var_sizeof(), 8);

    /* int_var_init() test */
    check_int("int_var_init()", call_int_var_init(), 0);

    /* uint_var_init() test */
    check_uint("uint_var_init()", call_uint_var_init(), 0);

    /* bool_var_init() test */
    check_bool("bool_var_init()", call_bool_var_init(), false);

    /* char_var_init() test */
    check_char("char_var_init()", call_char_var_init(), 255);

    /* stat_int_arry_length() test */
    check_uint("stat_int_arry_length()", call_stat_int_arry_length(), 4);

    /* stat_bool_arry_length() test */
    check_uint("stat_bool_arry_length()", call_stat_bool_arry_length(), 12);

    /* dyn_int_arry_length() tests */
    check_uint("dyn_int_arry_length(true)",
               call_dyn_int_arry_length(true), 5);
    check_uint("dyn_int_arry_length(false)",
               call_dyn_int_arry_length(false), 0);
              

    check_exit();
}
