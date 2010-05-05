#include "check_utils.h"

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

int
call_immutable_int(unsigned arg1, unsigned arg2)
{
   int res;

   asm ("    pushl %[arg1]\n"
        "    movl %[arg2],%%eax\n"
        "    call _D9immutable13immutable_intFkkZi\n"
        : "=a"(res)
        : [arg1]"m"(arg1),
          [arg2]"m"(arg2));

   return res;
}

bool
call_immutable_bool(int in)
{
   int res;

   asm ("    call _D9immutable14immutable_boolFiZb\n"
        : "=a"(res)
        : "a"(in));

   return res;
}

bool
call_invoke_char_present(char in)
{
   int res;

   asm ("    call _D9immutable19invoke_char_presentFaZb\n"
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

    /* immutable_int() tests */
    check_int("immutable_int(0, 0)", call_immutable_int(0, 0), -1);
    check_int("immutable_int(1, 0)", call_immutable_int(1, 0), 20);
    check_int("immutable_int(2, 0)", call_immutable_int(2, 0), 0);
    check_int("immutable_int(3, 0)", call_immutable_int(3, 0), -10);
    check_int("immutable_int(3, 1)", call_immutable_int(3, 1), -20);
    check_int("immutable_int(3, 2)", call_immutable_int(3, 2), -30);
    check_int("immutable_int(4, 0)", call_immutable_int(4, 0), 1);
    check_int("immutable_int(4, 1)", call_immutable_int(4, 1), 2);
    check_int("immutable_int(4, 2)", call_immutable_int(4, 2), 3);
    check_int("immutable_int(5, 0)", call_immutable_int(5, 0), -1);

    /* immutable_bool() tests */
    check_bool("immutable_bool(-30)", call_immutable_bool(-30), false);
    check_bool("immutable_bool(0)", call_immutable_bool(0), false);
    check_bool("immutable_bool(1)", call_immutable_bool(1), true);

    /* char_present() and invoke_char_present() tests */
    check_bool("invoke_char_present('a')",
               call_invoke_char_present('a'),
               false);
    check_bool("invoke_char_present('X')",
               call_invoke_char_present('X'),
               false);
    check_bool("invoke_char_present('p')",
               call_invoke_char_present('p'),
               true);
    check_bool("invoke_char_present('q')",
               call_invoke_char_present('q'),
               true);

    check_exit();
}
