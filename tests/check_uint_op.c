#include "check_utils.h"

bool
call_uint_assig();

bool
call_uint_arg(unsigned arg);

unsigned
call_uint_ret(bool arg);

unsigned
call_uint_arrys(unsigned arg);

unsigned
call_signed_unsigned_const_add();

unsigned
call_unsigned_signed_const_div();

int
call_signed_unsigned_add(int arg1, unsigned arg2);

int
call_unsigned_signed_mult(int arg1, unsigned arg2);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* uint_assig() test */
    check_bool("uint_assig()", call_uint_assig(), true);
    
    /* uint_arg() tests */
    check_bool("uint_arg(0)", call_uint_arg(0), false);
    check_bool("uint_arg(3)", call_uint_arg(3), true);
    check_bool("uint_arg(4)", call_uint_arg(4), false);

    /* uint_ret() tests */
    check_uint("uint_ret(true)", call_uint_ret(true), 42);
    check_uint("uint_ret(false)", call_uint_ret(false), 0);

    /* uint_arrys() tests */
    check_uint("uint_arrys(0)", call_uint_arrys(0), 1111);
    check_uint("uint_arrys(1)", call_uint_arrys(1), 2220);
    check_uint("uint_arrys(2)", call_uint_arrys(2), 3331);
    check_uint("uint_arrys(3)", call_uint_arrys(3), 4440);

    /* signed_unsigned_const_add() test */
    check_uint("signed_unsigned_const_add()",
               call_signed_unsigned_const_add(),
               3);

    /* unsigned_signed_const_div() test */
    check_uint("unsigned_signed_const_div()",
               call_unsigned_signed_const_div(), 15 / 3);

    /* signed_unsigned_add() tests */
    check_uint("signed_unsigned_add(20, 3)",
               call_signed_unsigned_add(20, 3), 20 + 3);
    check_uint("signed_unsigned_add(0, 11)",
               call_signed_unsigned_add(0, 11), 0 + 11);

    /* unsigned_signed_mult() tests */
    check_uint("unsigned_signed_mult(11, 5)",
               call_unsigned_signed_mult(11, 5), 11 * 5);
    check_uint("unsigned_signed_mult(1, 42)",
               call_unsigned_signed_mult(1, 42), 1 * 42);

    check_exit();
}
