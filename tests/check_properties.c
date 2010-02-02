#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

unsigned
call_int_var_sizeof()
{
   unsigned res;

   asm ("    call _D10properties14int_var_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_uint_var_sizeof()
{
   unsigned res;

   asm ("    call _D10properties15uint_var_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_bool_var_sizeof()
{
   unsigned res;

   asm ("    call _D10properties15bool_var_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_int_exp_sizeof()
{
   unsigned res;

   asm ("    call _D10properties14int_exp_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_bool_exp_sizeof()
{
   unsigned res;

   asm ("    call _D10properties15bool_exp_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

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

    /* int_exp_sizeof() test */
    check_uint("int_exp_sizeof()", call_int_exp_sizeof(), 4);

    /* bool_exp_sizeof() test */
    check_uint("bool_exp_sizeof()", call_bool_exp_sizeof(), 1);

    check_exit();
}
