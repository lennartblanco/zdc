#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_neg0(int in)
{
   int res;

   asm ("    call _D3neg4neg0FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_neg1(int in)
{
   int res;

   asm ("    call _D3neg4neg1FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_neg2(int in)
{
   int res;

   asm ("    call _D3neg4neg2FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_neg3()
{
   int res;

   asm ("    call _D3neg4neg3FZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_neg4()
{
   int res;

   asm ("    call _D3neg4neg4FZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_neg5(int in)
{
   int res;

   asm ("    call _D3neg4neg5FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_neg51(int in)
{
   int res;

   asm ("    call _D3neg5neg51FiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_neg6(int in)
{
   int res;

   asm ("    call _D3neg4neg6FiZi\n"
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
    /* neg0() tests */
    check_int("neg0(25)", call_neg0(25), -25);
    check_int("neg0(-84)", call_neg0(-84), 84);
    check_int("neg0(0)", call_neg0(0), 0);

    /* neg1() tests */
    check_int("neg1(256)", call_neg1(256), -256);
    check_int("neg1(-1962)", call_neg1(-1962), 1962);
    check_int("neg1(0)", call_neg1(0), 0);

    /* neg2() tests */
    check_int("neg2(128)", call_neg2(128), -128);
    check_int("neg2(-1)", call_neg2(-1), 1);
    check_int("neg2(0)", call_neg2(0), 0);

    /* neg3() test */
    check_int("neg3()", call_neg3(), 3);

    /* neg4() test */
    check_int("neg4()", call_neg4(), 1);

    /* neg5() tests */
    check_int("neg5(6)", call_neg5(6), 10 * 4 + 6);
    check_int("neg5(-40)", call_neg5(-40), 10 * 4 + (-40));

    /* neg51() tests */
    check_int("neg51(6)", call_neg51(6), 10 * 4 + 6);
    check_int("neg51(0)", call_neg51(0), 10 * 4 + 0);
 
    /* neg6() tests */
    check_int("neg6(4)", call_neg6(4), 10 * (4 + (-4)));
    check_int("neg6(1000)", call_neg6(1000), 10 * (4 + (-1000)));

    check_exit();
}
