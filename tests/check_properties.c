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

unsigned
call_stat_int_arry_var_sizeof()
{
   unsigned res;

   asm ("    call _D10properties24stat_int_arry_var_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_stat_bool_arry_var_sizeof()
{
   unsigned res;

   asm ("    call _D10properties25stat_bool_arry_var_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_dyn_int_arry_var_sizeof()
{
   unsigned res;

   asm ("    call _D10properties23dyn_int_arry_var_sizeofFZk\n"
        : "=a"(res)
        : );

   return res;
}

int
call_int_var_init()
{
   int res;

   asm ("    call _D10properties12int_var_initFZi\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_uint_var_init()
{
   unsigned res;

   asm ("    call _D10properties13uint_var_initFZk\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_bool_var_init()
{
   bool res;

   asm ("    call _D10properties13bool_var_initFZb\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_stat_int_arry_length()
{
   unsigned res;

   asm ("    call _D10properties20stat_int_arry_lengthFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_stat_bool_arry_length()
{
   unsigned res;

   asm ("    call _D10properties21stat_bool_arry_lengthFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_dyn_int_arry_length(bool in)
{
   unsigned res;

   asm ("    call _D10properties19dyn_int_arry_lengthFbZk\n"
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

    /* stat_int_arry_var_sizeof() test */
    check_uint("stat_int_arry_var_sizeof()",
               call_stat_int_arry_var_sizeof(), 16);

    /* stat_bool_arry_var_sizeof() test */
    check_uint("stat_bool_arry_var_sizeof()",
               call_stat_bool_arry_var_sizeof(), 7);

    /* dyn_int_arry_var_sizeof() test */
    check_uint("dyn_int_arry_var_sizeof()",
               call_dyn_int_arry_var_sizeof(), 8);

    /* int_var_init() test */
    check_int("int_var_init()", call_int_var_init(), 0);

    /* uint_var_init() test */
    check_uint("uint_var_init()", call_uint_var_init(), 0);

    /* bool_var_init() test */
    check_bool("bool_var_init()", call_bool_var_init(), false);

    /* stat_int_arry_length() test */
    check_uint("stat_int_arry_length()", call_stat_int_arry_length(), 4);

    /* stat_bool_arry_length() test */
    check_uint("stat_bool_arry_length()", call_stat_bool_arry_length(), 12);

    /* dyn_int_arry_length() tests */
    check_uint("dyn_int_arry_length(true)",
               call_dyn_int_arry_length(true), 5);
    check_uint("dyn_int_arry_length(false)",
               call_dyn_int_arry_length(false), 0);
              

    check_exit();
}
