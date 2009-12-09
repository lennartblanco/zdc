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

int
call_run_foreach_index_auto_type(int in)
{
   int res;

   asm ("    call run_foreach_index_auto_type\n"
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
    asm ("    pushl $0\n"
         "    pushl $0\n"
         "    pushl $0\n"
         "    pushl $0\n"
         "    call foreach_body_loc_vars\n"
         : "=a"(res));
    check_int("foreach_body_loc_vars([0 0 0 0])",
              res, 19341 + 19341 + 19341 + 19341);

    asm ("    pushl $3\n"
         "    pushl $5\n"
         "    pushl $4\n"
         "    pushl $2\n"
         "    call foreach_body_loc_vars\n"
         : "=a"(res));
    check_int("foreach_body_loc_vars([2 4 5 3])",
              res, 48352 + 49081 + 23112 + 28360);

    asm ("    pushl $12\n"
         "    pushl $13\n"
         "    pushl $14\n"
         "    pushl $15\n"
         "    call foreach_body_loc_vars\n"
         : "=a"(res));
    check_int("foreach_body_loc_vars([15 14 13 12])",
              res, 41092 + 56732 + 38528 + 37889);

    asm ("    pushl $3\n"
         "    pushl $18\n"
         "    pushl $1\n"
         "    pushl $0\n"
         "    call foreach_body_loc_vars\n"
         : "=a"(res));
    check_int("foreach_body_loc_vars([0 1 18 3])",
              res, -1);

    /* foreach_bool_arry() tests */
    asm ("    subl $6, %%esp\n"
         "    movb $0, 0(%%esp)\n"
         "    movb $0, 1(%%esp)\n"
         "    movb $0, 2(%%esp)\n"
         "    movb $0, 3(%%esp)\n"
         "    movb $0, 4(%%esp)\n"
         "    movb $0, 5(%%esp)\n"
         "    call foreach_bool_arry\n"
         : "=a"(res));
    check_int("foreach_bool_arry([false, false, false, false, false, false])",
              res, 0);

    asm ("    subl $6, %%esp\n"
         "    movb $1, 0(%%esp)\n"
         "    movb $1, 1(%%esp)\n"
         "    movb $1, 2(%%esp)\n"
         "    movb $1, 3(%%esp)\n"
         "    movb $1, 4(%%esp)\n"
         "    movb $1, 5(%%esp)\n"
         "    call foreach_bool_arry\n"
         : "=a"(res));
    check_int("foreach_bool_arry([true, true, true, true, true, true])",
              res, 11 + 12 + 13 + 14 + 15 + 16);

    asm ("    subl $6, %%esp\n"
         "    movb $0, 0(%%esp)\n"
         "    movb $0, 1(%%esp)\n"
         "    movb $1, 2(%%esp)\n"
         "    movb $0, 3(%%esp)\n"
         "    movb $0, 4(%%esp)\n"
         "    movb $1, 5(%%esp)\n"
         "    call foreach_bool_arry\n"
         : "=a"(res));
    check_int("foreach_bool_arry([false, false, true, false, false, true])",
              res, 13 + 16);

    asm ("    subl $6, %%esp\n"
         "    movb $1, 0(%%esp)\n"
         "    movb $0, 1(%%esp)\n"
         "    movb $0, 2(%%esp)\n"
         "    movb $1, 3(%%esp)\n"
         "    movb $0, 4(%%esp)\n"
         "    movb $1, 5(%%esp)\n"
         "    call foreach_bool_arry\n"
         : "=a"(res));
    check_int("foreach_bool_arry([true, false, false, true, false, true])",
              res, 11 + 14 + 16);

    asm ("    subl $6, %%esp\n"
         "    movb $1, 0(%%esp)\n"
         "    movb $0, 1(%%esp)\n"
         "    movb $1, 2(%%esp)\n"
         "    movb $0, 3(%%esp)\n"
         "    movb $0, 4(%%esp)\n"
         "    movb $0, 5(%%esp)\n"
         "    call foreach_bool_arry\n"
         : "=a"(res));
    check_int("foreach_bool_arry([true, false, true, false, false, false])",
              res, 11 + 13);

    check_exit();
}
