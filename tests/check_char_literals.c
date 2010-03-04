#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

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

unsigned char
call_char_array_literal(unsigned in)
{
   unsigned char res;

   asm ("    call _D13char_literals18char_array_literalFkZa\n"
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
    /* char_assigment() test */
    check_char("char_assigment()", call_char_assigment(), (unsigned char)'X');

    /* char_init() test */
    check_char("char_init()", call_char_init(), (unsigned char)'l');

    /* char_return() */
    check_char("char_return()", call_char_return(), (unsigned char)'r');

    /* char_array_literal() tests */
    check_char("char_array_literal()",
               call_char_array_literal(10),
               (unsigned char)' ');
    check_char("char_array_literal()",
               call_char_array_literal(0),
               (unsigned char)'T');
    check_char("char_array_literal()",
               call_char_array_literal(12),
               (unsigned char)'o');
    check_char("char_array_literal()",
               call_char_array_literal(22),
               (unsigned char)'?');

    check_exit();
}
