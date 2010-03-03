#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

bool
call_uninit_bool()
{
   bool res;

   asm ("    call _D11uninit_vars11uninit_boolFZb\n"
        : "=a"(res)
        : );

   return res;
}

unsigned char
call_uninit_char()
{
   unsigned char res;

   asm ("    call _D11uninit_vars11uninit_charFZa\n"
        : "=a"(res)
        : );

   return res;
}

unsigned char
call_uninit_int()
{
   unsigned char res;

   asm ("    call _D11uninit_vars10uninit_intFZi\n"
        : "=a"(res)
        : );

   return res;
}

unsigned char
call_uninit_uint()
{
   unsigned char res;

   asm ("    call _D11uninit_vars11uninit_uintFZk\n"
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
    /* uninit_bool() test */
    check_bool("uninit_bool()", call_uninit_bool(), false);

    /* uninit_char() test */
    check_char("uninit_char()", call_uninit_char(), 255);

    /* uninit_int() test */
    check_int("uninit_int()", call_uninit_int(), 0);

    /* uninit_uint() test */
    check_int("uninit_uint()", call_uninit_uint(), 0);

    check_exit();
}
