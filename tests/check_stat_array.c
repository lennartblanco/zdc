#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_init_int_array(int in)
{
   int res;

   asm ("    call _D10stat_array14init_int_arrayFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_init_bool_array(int in)
{
   bool res;

   asm ("    call _D10stat_array15init_bool_arrayFiZb\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_def_init_int_array(int in)
{
   int res;

   asm ("    call _D10stat_array18def_init_int_arrayFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_def_init_bool_array(int in)
{
   bool res;

   asm ("    call _D10stat_array19def_init_bool_arrayFiZb\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_init_exp_tst(int in)
{
   int res;

   asm ("    call _D10stat_array12init_exp_tstFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_intops(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D10stat_array6intopsFiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

bool
call_boolops(bool arg1, bool arg2, bool arg3, int arg4)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    pushl %[arg3]\n"
        "    movl %[arg4], %%eax\n"
        "    call _D10stat_array7boolopsFbbbiZb\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3),
          [arg4]"m"(arg4));

   return res;
}

bool
call_boolops2(bool arg1, bool arg2, bool arg3, int arg4)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    pushl %[arg3]\n"
        "    movl %[arg4], %%eax\n"
        "    call _D10stat_array8boolops2FbbbiZb\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3),
          [arg4]"m"(arg4));

   return res;
}

int
call_slice_assig_sum()
{
   int res;

   asm ("    call _D10stat_array15slice_assig_sumFZi\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_bslice_assig(int arg)
{
   bool res;

   asm ("    call _D10stat_array12bslice_assigFiZb\n"
        : "=a"(res)
        : "a"(arg));

   return res;
}

int
call_slices_ops(int arg)
{
   int res;

   asm ("    call _D10stat_array10slices_opsFiZi\n"
        : "=a"(res)
        : "a"(arg));

   return res;
}

int
call_int_slice_to_shorthand_slice(int arg)
{
   int res;

   asm ("    call _D10stat_array28int_slice_to_shorthand_sliceFiZi\n"
        : "=a"(res)
        : "a"(arg));

   return res;
}

int
call_int_slice_to_slice(int arg)
{
   int res;

   asm ("    call _D10stat_array18int_slice_to_sliceFiZi\n"
        : "=a"(res)
        : "a"(arg));

   return res;
}

bool
call_bool_slice_to_slice(bool in)
{
   bool res;

   asm ("    call _D10stat_array19bool_slice_to_sliceFbZb\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

unsigned
call_uint_slice_to_slice_idx(int in)
{
   unsigned res;

   asm ("    call _D10stat_array23uint_slice_to_slice_idxFiZk\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_call_sum_stat()
{
   int res;

   asm ("    call _D10stat_array13call_sum_statFZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_run_stat_array_last_arg()
{
   int res;

   asm ("    call _D10stat_array23run_stat_array_last_argFZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_run_stat_array_first_arg()
{
   int res;

   asm ("    call _D10stat_array24run_stat_array_first_argFZi\n"
        : "=a"(res)
        : );

   return res;
}

int
call_run_stat_array_bool(bool arg)
{
   int res;

   asm ("    call _D10stat_array19run_stat_array_boolFbZi\n"
        : "=a"(res)
        : "a" (arg));

   return res;
}

int
call_scalar_to_arry_slice_assig(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D10stat_array26scalar_to_arry_slice_assigFiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_stat_array_bool_var_init(int arg1, int arg2, bool arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    movl %[arg3], %%eax\n"
        "    call _D10stat_array24stat_array_bool_var_initFiibZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_implicit_arry_lit_casts(int arg)
{
   int res;

   asm ("    call _D10stat_array23implicit_arry_lit_castsFiZi\n"
        : "=a"(res)
        : "a"(arg));

   return res;
}


/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    int i;

    /* init_int_array() tests */
    check_int("init_int_array(0)", call_init_int_array(0), 10);
    check_int("init_int_array(1)", call_init_int_array(1), 11);
    check_int("init_int_array(2)", call_init_int_array(2), 12);
    check_int("init_int_array(3)", call_init_int_array(3), 13);

    /* init_bool_array() tests */
    check_bool("init_bool_array(0)", call_init_bool_array(0), true);
    check_bool("init_bool_array(1)", call_init_bool_array(1), true);
    check_bool("init_bool_array(2)", call_init_bool_array(2), false);
    check_bool("init_bool_array(3)", call_init_bool_array(3), false);

    /* def_init_int_array() tests */
    for (i = 0; i < 6; i++)
    {
        check_int("def_init_int_array(i)", call_def_init_int_array(i), 0);
    }

    /* def_init_bool_array() tests */
    for (i = 0; i < 8; i++)
    {
        check_bool("def_init_bool_array(i)", call_def_init_bool_array(i), 0);
    }

    /* init_exp_tst() tests */
    check_int("init_exp_tst(2)", call_init_exp_tst(2), (2 + 2 - 15) * 2);
    check_int("init_exp_tst(2)", call_init_exp_tst(100), (100 + 2 - 15) * 100);

    /* intops() tests */
    check_int("intops(3, 1)", call_intops(3, 1), 3 + 20 + 30 + 20);
    check_int("intops(-12, 2)", call_intops(-12, 2), -12 + 20 + 30 + 30);

    /* boolops() tests */
    check_bool("boolops(true, true, false, 2)",
               call_boolops(true, true, false, 2),
               true && true);

    check_bool("boolops(false, true, false, 1)",
               call_boolops(false, true, false, 1),
               true && false);

    /* boolops2() tests */
    check_bool("boolops2(true, true, false, 2)",
               call_boolops2(true, true, false, 2),
               true && true);

    check_bool("boolops2(false, true, false, 1)",
               call_boolops2(false, true, false, 1),
               true && false);

    /* slice_assig_sum() test */
    check_int("slice_assig_sum()", call_slice_assig_sum(), 6);

    /* bslice_assig() tests */
    check_bool("bslice_assig(0)", call_bslice_assig(0), true);
    check_bool("bslice_assig(1)", call_bslice_assig(1), false);
    check_bool("bslice_assig(2)", call_bslice_assig(2), true);
    check_bool("bslice_assig(3)", call_bslice_assig(3), true);
    check_bool("bslice_assig(4)", call_bslice_assig(4), false);

    /* slices_ops() test */
    check_int("slices_ops(85)", call_slices_ops(85), -85);

    /* int_slice_to_shorthand_slice() tests */
    check_int("int_slice_to_shorthand_slice(0)",
              call_int_slice_to_shorthand_slice(0), 9 + 7 + 6);
    check_int("int_slice_to_shorthand_slice(1)",
              call_int_slice_to_shorthand_slice(1), 7 + 6 + 5);
    check_int("int_slice_to_shorthand_slice(2)",
              call_int_slice_to_shorthand_slice(2), 6 + 5 + 4);
    check_int("int_slice_to_shorthand_slice(3)",
              call_int_slice_to_shorthand_slice(3), 5 + 4 + 3);

    /* int_slice_to_slice() tests */
    check_int("int_slice_to_slice(0)",
              call_int_slice_to_slice(0), 15 + 24);

    check_int("int_slice_to_slice(2)",
              call_int_slice_to_slice(2), -33 + 42);

    check_int("int_slice_to_slice(3)",
              call_int_slice_to_slice(3), 42 + 51);

    check_int("int_slice_to_slice(4)",
              call_int_slice_to_slice(4), 51 + 60);

    /* bool_slice_to_slice() tests */
    check_bool("bool_slice_to_slice(true)",
               call_bool_slice_to_slice(true), true);

    check_bool("bool_slice_to_slice(false)",
               call_bool_slice_to_slice(false), false);

    /* uint_slice_to_slice_idx() tests */
    check_uint("uint_slice_to_slice_idx(0)",
               call_uint_slice_to_slice_idx(0), 3 + 1 + 4);

    check_uint("uint_slice_to_slice_idx(1)",
               call_uint_slice_to_slice_idx(1), 1 + 4 + 1);

    check_uint("uint_slice_to_slice_idx(2)",
               call_uint_slice_to_slice_idx(2), 4 + 1 + 5);

    check_uint("uint_slice_to_slice_idx(3)",
               call_uint_slice_to_slice_idx(3), 1 + 5 + 9);

    /* call_sum_stat() and sum_stat() test */
    check_int("call_sum_stat()", 
              call_call_sum_stat(),
              (1 + 2 + 3) + ((-6) + 6 + 7));

    /* run_stat_array_last_arg() and stat_array_last_arg() test */
    check_int("run_stat_array_last_arg()",
              call_run_stat_array_last_arg(), 40 * (10 + 13));

    /* run_stat_array_first_arg() and stat_array_first_arg() test */
    check_int("run_stat_array_first_arg()",
              call_run_stat_array_first_arg(), 
               (0 - ((-20) + (-30) + (-40))) * 1 +
                ((-20) + (-30) + (-40)) * 10);

    /* run_stat_array_bool() and stat_array_bool() tests */
    check_int("run_stat_array_bool(true)",
              call_run_stat_array_bool(true), (2 + 1) * 3);

    check_int("run_stat_array_bool(false)",
              call_run_stat_array_bool(false), 0);

    /* scalar_to_arry_slice_assig() tests */
    check_int("scalar_to_arry_slice_assig(2, 5)",
               call_scalar_to_arry_slice_assig(2, 5), 2 * 3 + 5 * 3);
    check_int("scalar_to_arry_slice_assig(-100, 99)",
               call_scalar_to_arry_slice_assig(-100, 99), -100 * 3 + 99 * 3);

    /* stat_array_bool_var_init() tests */
    check_int("stat_array_bool_var_init(3, 100, true)",
               call_stat_array_bool_var_init(3, 100, true), 103);

    check_int("stat_array_bool_var_init(2, 20, false)",
               call_stat_array_bool_var_init(2, 20, false), 20);

    check_int("stat_array_bool_var_init(3, 100, true)",
               call_stat_array_bool_var_init(0, 0, true), 3);

    /* implicit_arry_lit_casts() tests */
    check_int("implicit_arry_lit_casts(0)",
              call_implicit_arry_lit_casts(0), 0);

    check_int("implicit_arry_lit_casts(1)",
              call_implicit_arry_lit_casts(1), 0);

    check_int("implicit_arry_lit_casts(2)",
              call_implicit_arry_lit_casts(2), 0);

    check_int("implicit_arry_lit_casts(3)",
              call_implicit_arry_lit_casts(3), 1);

    check_exit();
}

