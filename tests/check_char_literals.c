#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

/*00000000 T _D13char_literals11char_returnFZa*/
/*00000000 D _D13char_literals12__ModuleInfoZ*/
/*00000000 T _D13char_literals14char_assigmentFZa*/
/*00000000 T _D13char_literals9char_initFZa*/

unsigned char
call_char_assigment()
{
   unsigned char res;

   asm ("    call _D13char_literals14char_assigmentFZa\n"
        : "=a"(res)
        : );

   return res;
}

unsigned char
call_char_init()
{
   unsigned char res;

   asm ("    call _D13char_literals9char_initFZa\n"
        : "=a"(res)
        : );

   return res;
}

unsigned char
call_char_return()
{
   unsigned char res;

   asm ("    call _D13char_literals11char_returnFZa\n"
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
    /* char_assigment() test */
    check_char("char_assigment()", call_char_assigment(), (unsigned char)'X');

    /* char_init() test */
    check_char("char_init()", call_char_init(), (unsigned char)'l');

    /* char_return() */
    check_char("char_return()", call_char_return(), (unsigned char)'r');

    check_exit();
}
