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

int
call_invoke_array_var_find(unsigned char in)
{
   int res;

   asm ("    call _D13char_literals21invoke_array_var_findFaZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_invoke_array_lit_find(unsigned char in)
{
   int res;

   asm ("    call _D13char_literals21invoke_array_lit_findFaZi\n"
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
    check_char("char_array_literal()",
               call_char_array_literal(23),
               (unsigned char)'\'');
    check_char("char_array_literal()",
               call_char_array_literal(24),
               (unsigned char)'\"');
    check_char("char_array_literal()",
               call_char_array_literal(25),
               (unsigned char)'\?');
    check_char("char_array_literal()",
               call_char_array_literal(26),
               (unsigned char)'\\');
    check_char("char_array_literal()",
               call_char_array_literal(27),
               (unsigned char)'\a');
    check_char("char_array_literal()",
               call_char_array_literal(28),
               (unsigned char)'\b');
    check_char("char_array_literal()",
               call_char_array_literal(29),
               (unsigned char)'\f');
    check_char("char_array_literal()",
               call_char_array_literal(30),
               (unsigned char)'\n');
    check_char("char_array_literal()",
               call_char_array_literal(31),
               (unsigned char)'\r');
    check_char("char_array_literal()",
               call_char_array_literal(32),
               (unsigned char)'\t');
    check_char("char_array_literal()",
               call_char_array_literal(33),
               (unsigned char)'\v');


    /* test my_find() via invoke_array_var_find() */
    check_int("invoke_array_var_find('a')", 
              call_invoke_array_var_find('a'), -1);
    check_int("invoke_array_var_find('l')",
              call_invoke_array_var_find('l'), 2);
    check_int("invoke_array_var_find('w')",
              call_invoke_array_var_find('w'), 6);
    check_int("invoke_array_var_find('h')", 
              call_invoke_array_var_find('h'), 0);

    /* test my_find() via invoke_array_lit_find() */
    check_int("invoke_array_lit_find('q')", 
              call_invoke_array_lit_find('q'), -1);
    check_int("invoke_array_lit_find('x')", 
              call_invoke_array_lit_find('x'), 1);
    check_int("invoke_array_lit_find('s')", 
              call_invoke_array_lit_find('s'), 5);

    check_exit();
}
