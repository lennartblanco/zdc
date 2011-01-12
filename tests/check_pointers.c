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

    /* call_divide() tests */
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

    check_exit();
}

