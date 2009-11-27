#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

bool
call_uint_assig()
{
   bool res;

   asm ("    call uint_assig\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_uint_arg(unsigned in)
{
   bool res;

   asm ("    call uint_arg\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

unsigned
call_uint_ret(bool in)
{
   unsigned res;

   asm ("    call uint_ret\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

unsigned
call_uint_arrys(unsigned in)
{
   unsigned res;

   asm ("    call uint_arrys\n"
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

    check_exit();
}
