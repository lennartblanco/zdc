#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_neg0(int in)
{
   int res;

   asm ("    call neg0\n"
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

    /* todo: add test for functions below */
printf("todo: finish tests, failing\n");
    return -1;
//int neg1(int u)

//int neg2(int u)

//int neg3()

//int neg4()

//int neg5(int l)

//int neg51(int l)

//int neg6(int l)
    check_exit();
}
