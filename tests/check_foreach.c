#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_foreach_slice()
{
   int res;

   asm ("    call foreach_slice\n"
        : "=a"(res)
        : );

   return res;
}

int
call_foreach_slice_params(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call foreach_slice_params\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_foreach_shorthand_slice(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call foreach_shorthand_slice\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

bool
call_foreach_value_auto_type(int in)
{
   bool res;

   asm ("    call foreach_value_auto_type\n"
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

// TODO: add tests on foreach_index_auto_type(), foreach_body_loc_vars(),
//                    foreach_bool_arry()
abort();

    check_exit();
}
