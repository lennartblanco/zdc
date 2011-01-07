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

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* def_int_ptr() test */
    check_pointer("call_def_int_ptr()", call_def_int_ptr(), NULL);

    /* def_char_ptr test */
    check_pointer("call_def_char_ptr()", call_def_char_ptr(), NULL);

    /* malloced_ptr test */
    check_non_null_ptr("call_malloced_ptr()", call_malloced_ptr());

    check_exit();
}

