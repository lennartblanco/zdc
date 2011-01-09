#include <stdlib.h>
#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_signed_div_vars(int arg1, int arg2)
{
    int res;

    asm ("    pushl %[arg1]\n"
         "    movl %[arg2], %%eax\n"
         "    call _D6arithm15signed_div_varsFiiZi\n"
         : "=a"(res)
         : [arg1]"m"(arg1),
           [arg2]"m"(arg2));

    return res;
}

unsigned
call_unsigned_div_vars(unsigned arg1, unsigned arg2)
{
    unsigned res;

    asm ("    pushl %[arg1]\n"
         "    movl %[arg2], %%eax\n"
         "    call _D6arithm17unsigned_div_varsFkkZk\n"
         : "=a"(res)
         : [arg1]"m"(arg1),
           [arg2]"m"(arg2));

    return res;
}

int
call_signed_div_lconst(int in)
{
    int res;

    asm ("    call _D6arithm17signed_div_lconstFiZi\n"
         : "=a"(res)
         : "a"(in));

    return res;
}

unsigned
call_unsigned_div_rconst(unsigned in)
{
    unsigned res;

    asm ("    call _D6arithm19unsigned_div_rconstFkZk\n"
         : "=a"(res)
         : "a"(in));

    return res;
}

   return res;
}

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

    check_exit();
}

