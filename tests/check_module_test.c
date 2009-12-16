#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

void
call_depl(unsigned in)
{
   int res;

   asm ("    call _D7trinity4deplFkZv\n"
        :
        : "a"(in));
}

bool
call_boom(int arg1, int arg2)
{
   bool res;

   asm ("    pushl %[arg1]\n"
        "    call _D7trinity4boomFiiZb\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          "a"(arg2));

   return res;
}


/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* depl() test */
    call_depl(23);
    check("depl(23)");

    /* boom() tests */
    check_bool("boom(2, 3)", call_boom(2, 3), (2 - 3) == 0);
    check_bool("boom(-12, -12)", call_boom(-12, -12), true);


    check_exit();
}
