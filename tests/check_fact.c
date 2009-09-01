#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_fact_rec(int in)
{
   int res;

   asm ("    call fact_rec\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_fact_while(int in)
{
   int res;

   asm ("    call fact_while\n"
        : "=a"(res)
        : "a"(in));

   return res;
}


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

    check_exit();
}