#include <stdlib.h>
#include "check_utils.h"

int *
call_def_int_ptr();

char *
call_def_char_ptr();

void *
call_malloced_ptr();

void
call_compare(int arg1, int arg2, bool *arg3, bool *arg4);

void
call_divide(int arg1, int arg2, int *arg3, int *arg4);

unsigned
call_uint_ptr_dref(unsigned *arg1, int arg2);

bool
call_char_ptr_dref(char *arg);

int
call_alias_ptr_arg(int *arg);

int *
ipointer_int_addition(int *arg1, int arg2, bool arg3);

char *
cpointer_int_addition(char *ptr, uint i);

bool *
bpointer_char_addition(char c, bool *ptr);

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

    /* alias_ptr_arg() tests */
    int arg;

    arg = 10;
    check_int("alias_ptr_arg(&arg)", call_alias_ptr_arg(&arg), 9);
    check_cond("arg = 11", arg == 11);

    arg = 0;
    check_int("alias_ptr_arg(&arg)", call_alias_ptr_arg(&arg), -1);
    check_cond("arg = 1", arg == 1);

    /* ipointer_int_addition() tests */
    check_pointer("ipointer_int_addition(0x100, 3, true)",
                  ipointer_int_addition((int*)0x100, 3, true),
                                        (int*)(0x100 + 3 * 4));
    check_pointer("ipointer_int_addition(0x200, 1, false)",
                  ipointer_int_addition((int*)0x200, 1, false),
                                        (int*)(0x200 + 1 * 4));
    check_pointer("ipointer_int_addition(0x108, -2, true)",
                  ipointer_int_addition((int*)0x108, -2, true),
                                        (int*)(0x108 + (-2) * 4));
    check_pointer("ipointer_int_addition(0x20c, -1, true)",
                  ipointer_int_addition((int*)0x20c, -1, true),
                                        (int*)(0x20c + (-1) * 4));

    /* cpointer_int_addition() tests */
    check_pointer("cpointer_int_addition(0x1000, 2)",
                  cpointer_int_addition((char*)0x1000, 2),
                  (char*)(0x1000 + 2));
    check_pointer("cpointer_int_addition(0x1008, -2)",
                  cpointer_int_addition((char*)0x1008, -2),
                  (char*)(0x1008 + (-2)));
    check_pointer("cpointer_int_addition(0x0, 1)",
                  cpointer_int_addition((char*)0x0, 1),
                  (char*)(0x0 + 1));

    /* bpointer_char_addition() tests */
    check_pointer("bpointer_char_addition('x', 0x0)",
                  bpointer_char_addition('x', 0x0),
                  (bool *)('x' + 0x0));

    check_pointer("bpointer_char_addition('F', 0xcafe)",
                  bpointer_char_addition('F', (bool*)0xcafe),
                  (bool *)('F' + 0xcafe));


    check_exit();
}

