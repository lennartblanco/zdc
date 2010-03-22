#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_cinv_wrap(int in)
{
   int res;

   asm ("    call _D8extern_c9cinv_wrapFiZi\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

int
call_cadd_wrap(int arg1, int arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D8extern_c9cadd_wrapFiiZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_csubst3_wrap(int arg1, int arg2, int arg3)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    call _D8extern_c12csubst3_wrapFiiiZi\n"
        : "=a"(res)
        : "a" (arg3),
          [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

int
call_cexpr_wrap(int arg1, int arg2, int arg3, int arg4)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    pushl %[arg3]\n"
        "    call _D8extern_c10cexpr_wrapFiiiiZi\n"
        : "=a"(res)
        : "a" (arg4),
          [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));

   return res;
}

int
call_cget20_wrap()
{
   int res;

   asm ("    call _D8extern_c11cget20_wrapFZi\n"
        : "=a"(res)
        : );

   return res;
}

void
call_c_store_num_wrap(int in)
{
   asm ("    call _D8extern_c16c_store_num_wrapFiZv\n"
        :
        : "a"(in));
}

int
call_c_get_num_wrap()
{
   int res;

   asm ("    call _D8extern_c14c_get_num_wrapFZi\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_c_is_equal_wrap(int arg1, int arg2)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D8extern_c15c_is_equal_wrapFiiZb\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

/*---------------------------------------------------------------------------*
 *               extern (C) functions called from tests                      *
 *---------------------------------------------------------------------------*/

int
cinv(int c)
{
    return -c;
}

int
cadd(int left, int right)
{
    return left + right;
}

int
csubst3(int a, int b, int c)
{
    return a - b - c;
}

int
cexpr(int a, int b, int c, int d)
{
    return (a - b) * c + d;
}

int
cget20()
{
    return 20;
}

static int stored_num = 0;

void
c_store_num(int num)
{
    stored_num = num;
}

int
c_get_num()
{
    return stored_num;
}

bool
c_is_equal(int left, int right)
{
    return left == right;
}

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* cinv_wrap() tests */
    check_int("cinv_wrap(0)", call_cinv_wrap(0), 0);
    check_int("cinv_wrap(-20)", call_cinv_wrap(-20), -(-20));
    check_int("cinv_wrap(235)", call_cinv_wrap(235), -235);

    /* cadd_wrap() tests */
    check_int("cadd_wrap(1, 2)", call_cadd_wrap(1, 2), 1 + 2);
    check_int("cadd_wrap(100, 0)", call_cadd_wrap(100, 0), 100 + 0);
    check_int("cadd_wrap(23, 42)", call_cadd_wrap(23, 42), 23 + 42);
    check_int("cadd_wrap(-22, 34)", call_cadd_wrap(-22, 34), (-22) + 34);

    /* csubst3_wrap() tests */
    check_int("csubst3_wrap(1, 2, 3)", call_csubst3_wrap(1, 2, 3), 1 - 2 - 3);
    check_int("csubst3_wrap(3, 2, 1)", call_csubst3_wrap(3, 2, 1), 3 - 2 - 1);
    check_int("csubst3_wrap(25, 5, -5)",
              call_csubst3_wrap(25, 5, -5),
              25 - 5 - (-5));

    /* cexpr_wrap() tests */
    check_int("cexpr_wrap(10, 200, 3, 4)",
              call_cexpr_wrap(10, 200, 3, 4),
              (10 - 200) * 3 + 4);
    check_int("cexpr_wrap(10, 20, 30, 40)",
              call_cexpr_wrap(10, 20, 30, 40),
              (10 - 20) * 30 + 40);
    check_int("cexpr_wrap(-1000, -1001, -200, 300)",
              call_cexpr_wrap(-1000, -1001, -200, 300),
              (-1000 - (-1001)) * (-200) + 300);

    /* cget20_wrap() test */
    check_int("cget20_wrap()", call_cget20_wrap(), 20);

    /* c_store_num_wrap() and c_get_num_wrap() tests */
    call_c_store_num_wrap(10);
    check("c_store_num_wrap(10)");
    check_int("c_get_num_wrap()", call_c_get_num_wrap(), 10);

    call_c_store_num_wrap(20);
    check("c_store_num_wrap(20)");
    check_int("c_get_num_wrap()", call_c_get_num_wrap(), 20);

    /* c_is_equal_wrap() tests */
    check_bool("c_is_equal_wrap(5, 10)",
               call_c_is_equal_wrap(5, 10), false);

    check_bool("c_is_equal_wrap(34, 34)",
               call_c_is_equal_wrap(34, 34), true);

    check_bool("c_is_equal_wrap(0, 0)",
               call_c_is_equal_wrap(0, 0), true);


    check_bool("c_is_equal_wrap(-10, 10)",
               call_c_is_equal_wrap(-10, 10), false);


    check_exit();
}

