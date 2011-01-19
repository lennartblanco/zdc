#include <stdlib.h>
#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int *
call_def_int_ptr()
{
   int *res;

   asm ("    call _D8pointers11def_int_ptrFZPi\n"
        : "=a"(res));

   return res;
}

char *
call_def_char_ptr()
{
   char *res;

   asm ("    call _D8pointers12def_char_ptrFZPa\n"
        : "=a"(res));

   return res;
}

void *
call_malloced_ptr()
{
   void *res;

   asm ("    call _D8pointers12malloced_ptrFZPv\n"
        : "=a"(res));

   return res;
}

void
call_compare(int arg1, int arg2, bool *arg3, bool *arg4)
{
   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    pushl %[arg3]\n"
        "    call _D8pointers7compareFiiPbPbZv\n"
        "    addl $12, %%esp\n"
        :
        : "a" (arg4),
          [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));
}

void
call_divide(int arg1, int arg2, int *arg3, int *arg4)
{
   asm ("    pushl %[arg1]\n"
        "    pushl %[arg2]\n"
        "    pushl %[arg3]\n"
        "    call _D8pointers6divideFiiPiPiZv\n"
        "    addl $12, %%esp\n"
        :
        : "a" (arg4),
          [arg1]"m"(arg1),
          [arg2]"m"(arg2),
          [arg3]"m"(arg3));
}

unsigned
call_uint_ptr_dref(unsigned *arg1, int arg2)
{
    unsigned res;

    asm ("    pushl %[arg1]\n"
         "    call _D8pointers13uint_ptr_drefFPkbZk\n"
         "    addl $4, %%esp\n"
         : "=a"(res)
         : "a" (arg2),
           [arg1]"m"(arg1));

    return res;
}

bool
call_char_ptr_dref(char *arg1)
{
   int res;

   asm ("    call _D8pointers13char_ptr_drefFPaZb\n"
        : "=a"(res)
        : "a"(arg1));

   return res;
}

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* def_int_ptr() test */
    check_pointer("call_def_int_ptr()", call_def_int_ptr(), NULL);

    /* def_char_ptr() test */
    check_pointer("call_def_char_ptr()", call_def_char_ptr(), NULL);

    /* malloced_ptr() test */
    void *res = call_malloced_ptr();
    check_non_null_ptr("call_malloced_ptr()", res);
    free(res);

    /* divide() tests */
    int q = 0, r = 0;
    call_divide(11, 4, &q, &r);
    check_cond("divide(11, 4, &quotient, &reminder)",
               q == (11 / 4) && r == (11 % 4));

    call_divide(-20, 5, &q, &r);
    check_cond("divide(-20, 5, &quotient, &reminder)",
               q == (-20 / 5) && r == (-20 % 5));

    call_divide(13, -15, &q, &r);
    check_cond("divide(13, -20, &quotient, &reminder)",
               q == (13 / -15) && r == (13 % -15));

    /* compare() tests */
    bool equal = false;
    bool is_less = false;

    call_compare(20, 30, &equal, &is_less);
    check_cond("compare(20, 30, &equal, &is_less)",
               equal == (20 == 30) && is_less == (20 < 30));

    call_compare(-120, -120, &equal, &is_less);
    check_cond("compare(-120, -120, &equal, &is_less)",
               equal == (-120 == -120) && is_less == (-120 < -120));

    call_compare(0, -1, &equal, &is_less);
    check_cond("compare(0, -1, &equal, &is_less)",
               equal == (0 == -1) && is_less == (0 < -1));

    /* uint_ptr_dref() tests */
    unsigned val;

    val = 85;
    check_uint("uint_ptr_dref(&val, true)",
               call_uint_ptr_dref(&val, true), val);
    val = 2000002;
    check_uint("uint_ptr_dref(&val, true)",
               call_uint_ptr_dref(&val, true), val);
    val = 32;
    check_uint("uint_ptr_dref(&val, false)",
               call_uint_ptr_dref(&val, false), val * 2 + val / 2);
    val = 100;
    check_uint("uint_ptr_dref(&val, false)",
               call_uint_ptr_dref(&val, false), val * 2 + val / 2);

    /* char_ptr_dref() tests */
    char c;
    c = 'x';
    check_bool("char_ptr_dref(&'x')", call_char_ptr_dref(&c), false);
    c = 'G';
    check_bool("char_ptr_dref(&'G')", call_char_ptr_dref(&c), true);
    c = '7';
    check_bool("char_ptr_dref(&'7')", call_char_ptr_dref(&c), false);

    check_exit();
}

