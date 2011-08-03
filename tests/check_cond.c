#include "check_utils.h"

int
abs(int n);

int
const_cond(int n);

int
cond_statement(bool inc);

int
array_cond(unsigned idx);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* abs() tests */
    check_int("abs(10)", abs(10), 10);
    check_int("abs(-315)", abs(-315), 315);
    check_int("abs(0)", abs(0), 0);

    /* const_cond() tests */
    check_int("const_cond(-1)", const_cond(-1), 0);
    check_int("const_cond(0)", const_cond(0), 100);
    check_int("const_cond(1)", const_cond(1), 200);
    check_int("const_cond(2)", const_cond(2), 300);
    check_int("const_cond(3)", const_cond(3), 400);
    check_int("const_cond(4)", const_cond(4), 0);

    /* cond_statement() tests */
    check_int("cond_statement(true)", cond_statement(true), 11);
    check_int("cond_statement(false)", cond_statement(false), 9);

    /* array_cond() tests */
    check_int("array_cond(0)", array_cond(0), 0);
    check_int("array_cond(1)", array_cond(1), 1);
    check_int("array_cond(2)", array_cond(2), 21);
    check_int("array_cond(3)", array_cond(3), 321);
    check_int("array_cond(4)", array_cond(4), 4321);
    check_int("array_cond(5)", array_cond(5), 54321);
    check_int("array_cond(6)", array_cond(6), 54321);

    check_exit();
}
