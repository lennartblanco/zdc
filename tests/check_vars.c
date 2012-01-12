#include "check_utils.h"

bool
call_uninit_bool();

unsigned char
call_uninit_char();

unsigned char
call_uninit_int();

unsigned char
call_uninit_uint();

int
var_decls(int);

int
init_order(int v1, int v2);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* uninit_bool() test */
    check_bool("uninit_bool()", call_uninit_bool(), false);

    /* uninit_char() test */
    check_char("uninit_char()", call_uninit_char(), 255);

    /* uninit_int() test */
    check_int("uninit_int()", call_uninit_int(), 0);

    /* uninit_uint() test */
    check_int("uninit_uint()", call_uninit_uint(), 0);

    /* var_decls() tests */
    check_int("var_decls(0)", var_decls(0), -1);
    check_int("var_decls(1)", var_decls(1), 0 + 1 + 12 + 0 + 0);
    check_int("var_decls(2)", var_decls(2), 0 + 0 + 10 + 2 + 100);
    check_int("var_decls(3)", var_decls(3), 0);
    check_int("var_decls(4)", var_decls(4), -1);

    /* init_order() tests */
    check_int("init_order(-1, 1)", init_order(-1, 1), 1);
    check_int("init_order(100, 2)", init_order(100, 2), 2);

    check_exit();
}
