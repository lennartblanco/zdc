#include "check_utils.h"

enum ustates // base type uint
{
  USTATES_ON = 10u,
  USTATES_OFF,
  USTATES_DISABLED
};

enum nato
{
  NATO_UNKNOW,      //   0
  NATO_ABORT,       //   1
  NATO_ALPHA = 'a', //   97
  NATO_BRAVO,       //   98
  NATO_CHARLIE,     //   99
  NATO_DELTA,       //   100
  NATO_ECHO         //   101
};

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

int
call_get_state(char arg)
{
    int res;

    asm ("    call _D5enums9get_stateFaZE5enums6states\n"
         : "=a"(res)
         : "a"(arg));

    return res;
}

unsigned
call_ustates_get_def_as_uint()
{
   unsigned res;

   asm ("    call _D5enums23ustates_get_def_as_uintFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_ustates_get_off_as_uint()
{
   unsigned res;

   asm ("    call _D5enums23ustates_get_off_as_uintFZk\n"
        : "=a"(res)
        : );

   return res;
}

unsigned
call_ustates_get_as_uint(enum ustates arg)
{
    unsigned res;

    asm ("    call _D5enums19ustates_get_as_uintFE5enums7ustatesZk\n"
         : "=a"(res)
         : "a"(arg));

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

enum nato
call_get_next_char(enum nato arg)
{
    enum nato res;

    asm ("    call _D5enums13get_next_charFE5enums4natoZE5enums4nato\n"
         : "=a"(res)
         : "a"(arg));

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

bool
call_unnamed_enum_param(int arg1, char arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    call _D5enums18unnamed_enum_paramFbE5enums4numsZb\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          "a"(arg2));

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

    /* get_state() tests */
    check_int("get_state('x')",
              call_get_state('x'), 0);
    check_int("get_state('c')",
              call_get_state('c'), 0);
    check_int("get_state('r')",
              call_get_state('r'), 1);
    check_int("get_state('s')",
              call_get_state('s'), 2);

    /* ustates_get_def_as_uint() test */
    check_uint("ustates_get_def_as_uint()",
               call_ustates_get_def_as_uint(), 10);

    /* ustates_get_off_as_uint() test */
    check_uint("ustates_get_off_as_uint()",
               call_ustates_get_off_as_uint(), 11);

    /* ustates_get_as_uint() test */
    check_uint("ustates_get_as_uint(ustates.on)",
               call_ustates_get_as_uint(USTATES_ON), 10u);
    check_uint("ustates_get_as_uint(ustates.off)",
               call_ustates_get_as_uint(USTATES_OFF), 11u);
    check_uint("ustates_get_as_uint(ustates.disabled)",
               call_ustates_get_as_uint(USTATES_DISABLED), 12u);

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

    /* get_next_char() test */
    check_char("get_next_char(nato.UNKNOW)",
               call_get_next_char(NATO_UNKNOW), '\0');
    check_char("get_next_char(nato.ABORT)",
               call_get_next_char(NATO_ABORT), '\0');
    check_char("get_next_char(nato.ALPHA)",
               call_get_next_char(NATO_ALPHA), 'b');
    check_char("get_next_char(nato.BRAVO)",
               call_get_next_char(NATO_BRAVO), 'c');
    check_char("get_next_char(nato.CHARLIE)",
               call_get_next_char(NATO_CHARLIE), 'd');
    check_char("get_next_char(nato.DELTA)",
               call_get_next_char(NATO_DELTA), 'e');
    check_char("get_next_char(nato.ECHO)",
               call_get_next_char(NATO_ECHO), '\0');

    /* logic_get_def_as_bool() test */
    check_bool("logic_get_def_as_bool()",
               call_logic_get_def_as_bool(), true);

    /* logic_get_F_as_bool() test */
    check_bool("logic_get_F_as_bool()",
               call_logic_get_F_as_bool(), false);

    /* unnamed_enum_param() tests */
    check_bool("unnamed_enum_param(true)",
               call_unnamed_enum_param(true, '\0'), false);
    check_bool("unnamed_enum_param(false)",
               call_unnamed_enum_param(false, '\0'), true);


    check_exit();
}
