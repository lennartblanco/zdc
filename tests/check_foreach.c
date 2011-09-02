#include "check_utils.h"

int
call_foreach_slice();

int
call_foreach_slice_params(int arg1, int arg2);

int
call_foreach_shorthand_slice(int arg1, int arg2);

bool
call_foreach_value_auto_type(int arg);

int
call_run_foreach_index_auto_type(int arg);

unsigned
foreach_break(int num);

int
call_foreach_body_loc_vars(unsigned int testno);

unsigned int
call_foreach_bool_arry(unsigned int testno);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    int res;

    /* foreach_slice() test */
    check_int("foreach_slice()", call_foreach_slice(), (1 + 2 + 3) + 2);

    /* foreach_slice_params() tests */
    check_int("foreach_slice_params(1, 2)",
              call_foreach_slice_params(1, 2),
              11 + 0);

    check_int("foreach_slice_params(0, 6)",
              call_foreach_slice_params(0, 6),
              (10 + 11 + 12 + 13 + 14 + 15) +
              (0 + 1 + 2 + 3 + 4 + 5));

    check_int("foreach_slice_params(3, 5)",
              call_foreach_slice_params(3, 5),
              (13 + 14) + (0 + 1));

    /* foreach_shorthand_slice() tests */
    check_int("foreach_shorthand_slice(0, -1)",
              call_foreach_shorthand_slice(0, -1),
              -1 * 4 * 6 * 8 * 10 * 12 * 14 * 16);

    check_int("foreach_shorthand_slice(4, 3)",
              call_foreach_shorthand_slice(4, 3),
              2 * 4 * 6 * 8 * 3 * 12 * 14 * 16);

    /* foreach_value_auto_type() tests */
    check_bool("foreach_value_auto_type(13)",
               call_foreach_value_auto_type(13), true);

    check_bool("foreach_value_auto_type(14)",
               call_foreach_value_auto_type(14), true);

    check_bool("foreach_value_auto_type(15)",
               call_foreach_value_auto_type(15), false);

    check_bool("foreach_value_auto_type(16)",
               call_foreach_value_auto_type(16), false);

    /* 
     * foreach_index_auto_type() and
     * run_foreach_index_auto_type() tests
     */
    check_int("run_foreach_index_auto_type(97)",
              call_run_foreach_index_auto_type(97),
              0);

    check_int("run_foreach_index_auto_type(211)",
              call_run_foreach_index_auto_type(211),
              2);

    check_int("run_foreach_index_auto_type(36)",
              call_run_foreach_index_auto_type(36),
              5);

    check_int("run_foreach_index_auto_type(-15)",
              call_run_foreach_index_auto_type(-15),
              -1);

    check_int("run_foreach_index_auto_type(4)",
              call_run_foreach_index_auto_type(4),
              -1);

    /* foreach_body_loc_vars() tests */
    check_int("foreach_body_loc_vars([0 0 0 0])",
              call_foreach_body_loc_vars(0),
              19341 + 19341 + 19341 + 19341);

    check_int("foreach_body_loc_vars([2 4 5 3])",
              call_foreach_body_loc_vars(1),
              48352 + 49081 + 23112 + 28360);

    check_int("foreach_body_loc_vars([15 14 13 12])",
              call_foreach_body_loc_vars(2),
              41092 + 56732 + 38528 + 37889);

    check_int("foreach_body_loc_vars([0 1 18 3])",
              call_foreach_body_loc_vars(3),
              -1);

    check_int("call_foreach_body_loc_vars(4)",
              call_foreach_body_loc_vars(4),
              -2);

    /* foreach_bool_arry() tests */
    check_int("foreach_bool_arry([false, false, false, false, false, false])",
              call_foreach_bool_arry(0), 0);

    check_int("foreach_bool_arry([true, true, true, true, true, true])",
              call_foreach_bool_arry(1),
              11 + 12 + 13 + 14 + 15 + 16);

    check_int("foreach_bool_arry([false, false, true, false, false, true])",
              call_foreach_bool_arry(2),
              13 + 16);

    check_int("foreach_bool_arry([true, false, false, true, false, true])",
              call_foreach_bool_arry(3),
              11 + 14 + 16);

    check_int("foreach_bool_arry([true, false, true, false, false, false])",
              call_foreach_bool_arry(4),
              11 + 13);

    check_int("call_foreach_bool_arry(5)", call_foreach_bool_arry(5), -1);

    /* foreach_break() tests */
    check_uint("foreach_break(0)", foreach_break(0), 4);
    check_uint("foreach_break(8)", foreach_break(8), 3);
    check_uint("foreach_break(1)", foreach_break(1), 2);
    check_uint("foreach_break(7)", foreach_break(7), 1);
    check_uint("foreach_break(2)", foreach_break(2), 0);
    check_uint("foreach_break(-1)", foreach_break(-1), 4);

    check_exit();
}
