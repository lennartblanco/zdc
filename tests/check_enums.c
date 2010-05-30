#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

bool
call_states_get_def_as_int()
{
   bool res;

   asm ("    call _D5enums21states_get_def_as_intFZi\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_ustates_get_def_as_int()
{
   bool res;

   asm ("    call _D5enums22ustates_get_def_as_intFZk\n"
        : "=a"(res)
        : );

   return res;
}

char
call_nums_get_def_as_char()
{
   bool res;

   asm ("    call _D5enums20nums_get_def_as_charFZa\n"
        : "=a"(res)
        : );

   return res;
}

char
call_nato_get_def_as_char()
{
   bool res;

   asm ("    call _D5enums20nato_get_def_as_charFZa\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_logic_get_def_as_bool()
{
   bool res;

   asm ("    call _D5enums21logic_get_def_as_boolFZb\n"
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
    /* states_get_def_as_int() test */
    check_int("states_get_def_as_int()",
              call_states_get_def_as_int(), 0);

    /* ustates_get_def_as_int() test */
    check_uint("ustates_get_def_as_int()",
               call_ustates_get_def_as_int(), 10);

    /* nums_get_def_as_char() test */
    check_char("nums_get_def_as_char()",
               call_nums_get_def_as_char(), '0');

    /* nato_get_def_as_char() test */
    check_char("nato_get_def_as_char()",
               call_nato_get_def_as_char(), '\0');

    /* logic_get_def_as_bool() test */
    check_bool("logic_get_def_as_bool()",
               call_logic_get_def_as_bool(), true);
    check_exit();
}
