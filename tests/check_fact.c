#include "check_utils.h"

int
call_fact_rec(int arg);

int
call_fact_while(int arg);

unsigned
call_fact_while_postinc(unsigned arg);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* fact_rec() tests */
    check_int("fact_rec(0)", call_fact_rec(0), 1);
    check_int("fact_rec(1)", call_fact_rec(1), 1);
    check_int("fact_rec(2)", call_fact_rec(2), 1*2);
    check_int("fact_rec(3)", call_fact_rec(3), 1*2*3);
    check_int("fact_rec(6)", call_fact_rec(6), 1*2*3*4*5*6);

    /* fact_while() tests */
    check_int("fact_while(0)", call_fact_while(0), 1);
    check_int("fact_while(1)", call_fact_while(1), 1);
    check_int("fact_while(2)", call_fact_while(2), 1*2);
    check_int("fact_while(3)", call_fact_while(3), 1*2*3);
    check_int("fact_while(6)", call_fact_while(6), 1*2*3*4*5*6);

    /* call_fact_while_postinc() tests */
    check_uint("fact_while_postinc(0)", call_fact_while_postinc(0), 1);
    check_uint("fact_while_postinc(1)", call_fact_while_postinc(1), 1);
    check_uint("fact_while_postinc(4)", call_fact_while_postinc(4), 1*2*3*4);
    check_uint("fact_while_postinc(5)", call_fact_while_postinc(5), 1*2*3*4*5);
    check_uint("fact_while_postinc(7)",
               call_fact_while_postinc(7),
               1*2*3*4*5*6*7);

    check_exit();
}
