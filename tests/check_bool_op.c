#include "check_utils.h"

#include <assert.h>

/*---------------------------------------------------------------------------*
 *          wrappers to call test function with D calling convention         *
 *---------------------------------------------------------------------------*/

bool
call_return_true()
{
   bool res;

   asm ("    call return_true\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_return_false()
{
   bool res;

   asm ("    call return_false\n"
        : "=a"(res)
        : );

   return res;
}

bool
call_invert(bool in)
{
   bool res;

   asm ("    call invert\n"
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
    /* return_true() test */
    check_bool("return_true()", call_return_true(), true);

    /* return_true() test */
    check_bool("return_false()", call_return_false(), false);

    /* invert() tests */
    check_bool("invert(true)", call_invert(true), false);
    check_bool("invert(false)", call_invert(false), true);

/*
 * todo add test on:
 * is_equal()
 * not_equal() 
 * is_less_or_equal()
 * is_greater_or_equal()
 * is_greater()
 * greater_tst1()
 * greater_tst2(int x)
 * greater_tst3(int x)
 * is_less(int a, int b)
 * less_tst1()
 * less_tst2(int x)
 * less_tst3(int x)
 * tripple_and_op(bool left, bool right, bool last)
 * andor_ops(bool a, bool b, bool c)
 * and_oper(bool left, bool right)
 * or_oper(bool l, bool r)
 */
   assert(false);
   check_exit();
}
