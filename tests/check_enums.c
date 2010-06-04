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

bool
call_states_get_idle_as_int()
{
   bool res;

   asm ("    call _D5enums22states_get_idle_as_intFZi\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_states_get_stop_as_int()
{
   bool res;

   asm ("    call _D5enums22states_get_stop_as_intFZi\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_states_get_run_as_int()
{
   bool res;

   asm ("    call _D5enums21states_get_run_as_intFZi\n"
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

unsigned
call_ustates_get_off_as_int()
{
   bool res;

   asm ("    call _D5enums22ustates_get_off_as_intFZk\n"
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
call_nums_get_SIX_as_char()
{
   bool res;

   asm ("    call _D5enums20nums_get_SIX_as_charFZa\n"
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

char
call_nato_get_DELTA_as_char()
{
   bool res;

   asm ("    call _D5enums22nato_get_DELTA_as_charFZa\n"
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

bool
call_logic_get_F_as_bool()
{
   bool res;

   asm ("    call _D5enums19logic_get_F_as_boolFZb\n"
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

    /* states_get_idle_as_int() test */
    check_int("states_get_idle_as_int()",
              call_states_get_idle_as_int(), 0);

    /* states_get_run_as_int() test */
    check_int("states_get_run_as_run()",
              call_states_get_run_as_int(), 1);

    /* states_get_stop_as_int() test */
    check_int("states_get_stop_as_int()",
              call_states_get_stop_as_int(), 2);

    /* ustates_get_def_as_int() test */
    check_uint("ustates_get_def_as_int()",
               call_ustates_get_def_as_int(), 10);

    /* ustates_get_off_as_int() test */
    check_uint("ustates_get_off_as_int()",
               call_ustates_get_off_as_int(), 11);


    /* nums_get_def_as_char() test */
    check_char("nums_get_def_as_char()",
               call_nums_get_def_as_char(), '0');

    /* nums_get_SIX_as_char() test */
    check_char("nums_get_SIX_as_char()",
               call_nums_get_SIX_as_char(), '6');

    /* nato_get_def_as_char() test */
    check_char("nato_get_def_as_char()",
               call_nato_get_def_as_char(), '\0');

    /* nato_get_DELTA_as_char() test */
    check_char("nato_get_DELTA_as_char()",
               call_nato_get_DELTA_as_char(), 'd');


    /* logic_get_def_as_bool() test */
    check_bool("logic_get_def_as_bool()",
               call_logic_get_def_as_bool(), true);

    /* logic_get_F_as_bool() test */
    check_bool("logic_get_F_as_bool()",
               call_logic_get_F_as_bool(), false);

    check_exit();
}
