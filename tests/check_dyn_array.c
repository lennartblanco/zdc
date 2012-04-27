#include "check_utils.h"

int
call_int_array_lit_assigment(int arg);

int
call_int_array_lit_init(int arg);

int
call_intops(int arg1, int arg2);

bool
call_bool_array_lit_assigment(int arg);

bool
call_bool_array_lit_init(bool arg);

bool
call_boolops(int arg1, int arg2);

char
call_char_array(char arg1, int arg2);

int
call_invoke_dyn_array_sum_handle(int arg);

int
call_invoke_dyn_array_sum_lit(int arg);

int
call_dyn_array_slice_assigment(bool arg1, unsigned arg2);

unsigned
call_dyn_array_slice_assigment_length(unsigned arg);

int
call_dyn_array_slice_shorthand(bool arg1, unsigned arg2);

int
call_invoke_find_int(int arg);

unsigned
array_null_assign(bool assign_null);

int
array_null_arg(bool use_null);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    unsigned idx;

    /* int_array_lit_assigment() tests */
    check_int("int_array_lit_assigment(0)",
              call_int_array_lit_assigment(0), 3 + 3);
    check_int("int_array_lit_assigment(1)",
              call_int_array_lit_assigment(1), 3 + 1);
    check_int("int_array_lit_assigment(2)",
              call_int_array_lit_assigment(2), 3 + 4);

    /* int_array_lit_init() tests */
    check_int("int_array_lit_init(0)",
              call_int_array_lit_init(0), 2);
    check_int("int_array_lit_init(1)",
              call_int_array_lit_init(1), 7);
    check_int("int_array_lit_init(2)",
              call_int_array_lit_init(2), 1);
    check_int("int_array_lit_init(3)",
              call_int_array_lit_init(3), 8);
    check_int("int_array_lit_init(3)",
              call_int_array_lit_init(4), 2);

    /* intops() tests */
    check_int("intops(0, 20)", call_intops(0, 20), 20 + 2 + 3);
    check_int("intops(1, 0)", call_intops(1, 0), 1 + 0 + 3);
    check_int("intops(2, -20)", call_intops(2, -20), 1 + 2 + (-20));

    /* bool_array_lit_assigment() tests */
    check_bool("bool_array_lit_assigment(0)",
               call_bool_array_lit_assigment(0), true);
    check_bool("bool_array_lit_assigment(1)",
               call_bool_array_lit_assigment(1), false);
    check_bool("bool_array_lit_assigment(2)",
               call_bool_array_lit_assigment(2), true);
    check_bool("bool_array_lit_assigment(3)",
               call_bool_array_lit_assigment(3), false);

    /* bool_array_lit_init() tests */
    check_bool("bool_array_lit_init(true)",
               call_bool_array_lit_init(true), true);
    check_bool("bool_array_lit_init(false)",
               call_bool_array_lit_init(false), false);

    /* boolops() tests */
    check_bool("boolops(0, -1)", call_boolops(0, -1), false);
    check_bool("boolops(1, 10)", call_boolops(1, 10), true);
    check_bool("boolops(2, 0)", call_boolops(2, 0), false);

    /* char_array() tests */
    check_char("char_array('X', 0)", call_char_array('X', 0), 'X');
    check_char("char_array('X', 1)", call_char_array('X', 1), 'b');
    check_char("char_array('X', 2)", call_char_array('X', 2), 'c');
    check_char("char_array('X', 3)", call_char_array('X', 3), 'd');
    check_char("char_array('X', 4)", call_char_array('X', 4), 'e');
    check_char("char_array('X', 5)", call_char_array('X', 5), 'X');

    /* dyn_array_sum() tests */
    check_int("invoke_dyn_array_sum_handle(0)",
              call_invoke_dyn_array_sum_handle(0), 1 + 10 + 15 + 0 + 5);

    check_int("invoke_dyn_array_sum_handle(1)",
              call_invoke_dyn_array_sum_handle(1), 5 -12);

    check_int("invoke_dyn_array_sum_handle(2)",
              call_invoke_dyn_array_sum_handle(2), 0);

    check_int("invoke_dyn_array_sum_handle(3)",
              call_invoke_dyn_array_sum_handle(3), -1);

    check_int("invoke_dyn_array_sum_lit(0)",
              call_invoke_dyn_array_sum_lit(0), 3 + 1 + 4 + 1 + 5);

    check_int("invoke_dyn_array_sum_lit(1)",
              call_invoke_dyn_array_sum_lit(1), 50 + 23);

    check_int("invoke_dyn_array_sum_lit(2)",
              call_invoke_dyn_array_sum_lit(2), 0);

    check_int("invoke_dyn_array_sum_lit(3)",
              call_invoke_dyn_array_sum_lit(3), -1);

    /* dyn_array_slice_assigment() tests */
    for (idx = 0; idx < 5; idx += 1)
    {
      check_int("dyn_array_slice_assigment(false, idx)",
                  call_dyn_array_slice_assigment(false, idx), (int)(idx + 1));
    }
    check_int("dyn_array_slice_assigment(true, 0)",
              call_dyn_array_slice_assigment(true, 0), 1);
    check_int("dyn_array_slice_assigment(true, 1)",
              call_dyn_array_slice_assigment(true, 1), 2);
    check_int("dyn_array_slice_assigment(true, 2)",
              call_dyn_array_slice_assigment(true, 2), 11);
    check_int("dyn_array_slice_assigment(true, 3)",
              call_dyn_array_slice_assigment(true, 3), 22);
    check_int("dyn_array_slice_assigment(true, 4)",
              call_dyn_array_slice_assigment(true, 4), 5);

    /* dyn_array_slice_assigment_length() tests */
    check_uint("dyn_array_slice_assigment_length(0)",
               call_dyn_array_slice_assigment_length(0), 0);
    check_uint("dyn_array_slice_assigment_length(1)",
               call_dyn_array_slice_assigment_length(1), 0);
    check_uint("dyn_array_slice_assigment_length(2)",
               call_dyn_array_slice_assigment_length(2), 0);
    check_uint("dyn_array_slice_assigment_length(3)",
               call_dyn_array_slice_assigment_length(3), 99);
    check_uint("dyn_array_slice_assigment_length(4)",
               call_dyn_array_slice_assigment_length(4), 98);
    check_uint("dyn_array_slice_assigment_length(5)",
               call_dyn_array_slice_assigment_length(5), 97);

    /* dyn_array_slice_shorthand() tests */
    for (idx = 0; idx < 5; idx += 1)
    {
      check_int("dyn_array_slice_shorthand(false, idx)",
                call_dyn_array_slice_shorthand(false, idx), -1);
    }
    check_int("dyn_array_slice_shorthand(true, 0)",
              call_dyn_array_slice_shorthand(true, 0), 11);
    check_int("dyn_array_slice_shorthand(true, 1)",
              call_dyn_array_slice_shorthand(true, 1), 22);
    check_int("dyn_array_slice_shorthand(true, 2)",
              call_dyn_array_slice_shorthand(true, 2), 33);
    check_int("dyn_array_slice_shorthand(true, 3)",
              call_dyn_array_slice_shorthand(true, 3), 44);
    check_int("dyn_array_slice_shorthand(true, 4)",
              call_dyn_array_slice_shorthand(true, 4), 55);

    /* find_int() tests */
    check_bool("invoke_find_int(0)",
               call_invoke_find_int(0), true);
    check_bool("invoke_find_int(1)",
               call_invoke_find_int(1), false);
    check_bool("invoke_find_int(2)",
               call_invoke_find_int(2), false);
    check_bool("invoke_find_int(3)",
               call_invoke_find_int(3), false);
    check_bool("invoke_find_int(4)",
               call_invoke_find_int(4), true);

    /* array_null_assign() tests */
    check_uint("array_null_assign(true)", array_null_assign(true), 0);
    check_uint("array_null_assign(false)", array_null_assign(false), 4);

    /* array_null_arg() tests */
    check_int("array_null_arg(true)", array_null_arg(true), 0);
    check_int("array_null_arg(false)", array_null_arg(false), 10 + 2 + 3);

    check_exit();
}
