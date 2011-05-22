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

int
call_states_get_def_as_int();

int
call_states_get_idle_as_int();

int
call_states_get_stop_as_int();

int
call_states_get_run_as_int();

int
call_get_state(char arg);

unsigned
ustates_get_sizeof();

unsigned
call_ustates_get_def_as_uint();

unsigned
call_ustates_get_off_as_uint();

unsigned
call_ustates_get_as_uint(enum ustates arg);

char
call_nums_get_def_as_char();

char
call_nums_get_SIX_as_char();

unsigned
nato_get_sizeof();

char
call_nato_get_def_as_char();

char
call_nato_get_DELTA_as_char();

enum nato
call_get_next_char(enum nato arg);

bool
call_logic_get_def_as_bool();

bool
call_logic_get_F_as_bool();

bool
call_unnamed_enum_param(bool arg1, int arg2);

int
anon_enum_val();

int
get_ae_exp(int arg);

int
get_anon_e();

unsigned
get_u_length(bool arg);

char
get_def_color_c(unsigned);

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

    /* ustates_get_sizeof() test */
    check_uint("ustates_get_sizeof()", ustates_get_sizeof(), 4);

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

    /* nato_get_sizeof() test */
    check_uint("nato_get_sizeof()", nato_get_sizeof(), 1);

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

    /* anon_enum_val() test */
    check_int("anon_enum_val()", anon_enum_val(), 23);

    /* get_ae_exp() tests */
    check_int("get_ae_exp(1)", get_ae_exp(1), (20 + 21 + 22) * 1);
    check_int("get_ae_exp(11)", get_ae_exp(11), (20 + 21 + 22) * 11);
    check_int("get_ae_exp(-2)", get_ae_exp(-2), 20 + 21 + (-2 + 22));

    /* get_anon_e() test */
    check_int("get_anon_e()", get_anon_e(), 100);

    /* get_u_length() test */
    check_uint("get_u_length(true)", get_u_length(true), 6);
    check_uint("get_u_length(false)", get_u_length(false), 6);

    /* get_def_color_c() tests */
    check_char("get_def_color_c(4)", get_def_color_c(4), '?');
    check_char("get_def_color_c(0)", get_def_color_c(0), 'b');
    check_char("get_def_color_c(1)", get_def_color_c(1), 'l');
    check_char("get_def_color_c(2)", get_def_color_c(2), 'u');
    check_char("get_def_color_c(3)", get_def_color_c(3), 'e');

    check_exit();
}
