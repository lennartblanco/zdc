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

int
addr_of_int(int a, int b, bool ret_divident);

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

unsigned *
pointer_substraction(unsigned *ptr, unsigned i, bool use_arg);

int
pointer_pointer_substraction(int *l, int *r);

int
ptr_offset_access(int *ptr, unsigned offset);

bool
ptr_comp(char *left, char *right, int op_type);

bool
void_ptr_comp(void *left, int *right, int op_type);

bool
null_ptr_comp(unsigned* ptr, int op_type);

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

    /* addr_of_int() tests */
    check_int("addr_of_int(31, 10, true)", addr_of_int(31, 10, true), 31 / 10);
    check_int("addr_of_int(31, 10, false)", addr_of_int(31, 10, false), 31 % 10);
    check_int("addr_of_int(27, 3, true)", addr_of_int(27, 3, true), 27 / 3);
    check_int("addr_of_int(27, 3, false)", addr_of_int(27, 3, false), 27 % 3);

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

    /* pointer_substraction() tests */
    check_pointer("pointer_substraction(0x108, 2, true)",
                  pointer_substraction((unsigned *)0x108, 2, true),
                  (unsigned *)(0x108 - 2 * 4));
    check_pointer("pointer_substraction(0x108, 12, false)",
                  pointer_substraction((unsigned *)0x108, 12, false),
                  (unsigned *)(0x108 - 3 * 4));

    /* pointer_pointer_substraction() tests */
    check_int("pointer_pointer_substraction(0xff, 0xf7)",
              pointer_pointer_substraction((int*)0xff, (int*)0xf7),
              ((0xff - 0xf7) / 4));
    check_int("pointer_pointer_substraction(0xbeef, 0xbeef)",
              pointer_pointer_substraction((int*)0xbeef, (int*)0xbeef), 0);
    check_int("pointer_pointer_substraction(4, 8)",
              pointer_pointer_substraction((int*)4, (int*)8), (4-8)/4);

    /* ptr_offset_access() tests */
    int arr[4] = {1, 11, 111, 1111};

    check_int("ptr_offset_access(arr, 3)", ptr_offset_access(arr, 3), arr[3]);
    check_int("ptr_offset_access(arr, 2)", ptr_offset_access(arr, 2), arr[2]);
    check_int("ptr_offset_access(arr, 1)", ptr_offset_access(arr, 1), arr[1]);
    check_int("ptr_offset_access(arr, 0)", ptr_offset_access(arr, 0), arr[0]);

    /* ptr_comp() tests */
    check_bool("ptr_comp(NULL, NULL, -1)",
               ptr_comp(NULL, NULL, -1), false);

    check_bool("ptr_comp(NULL, NULL, 0)",
               ptr_comp(NULL, NULL, 0), true);
    check_bool("ptr_comp(NULL, 0x1234, 0)",
               ptr_comp(NULL, (char*)0x1234, 0), false);

    check_bool("ptr_comp(NULL, NULL, 1)",
               ptr_comp(NULL, NULL, 1), false);
    check_bool("ptr_comp(NULL, 0x1234, 1)",
               ptr_comp(NULL, (char*)0x1234, 1), true);

    check_bool("ptr_comp(NULL, 0x1234, 2)",
               ptr_comp(NULL, (char*)0x1234, 2), true);
    check_bool("ptr_comp(0x10, 0x10, 2)",
               ptr_comp((char*)0x10, (char*)0x10, 2), false);
    check_bool("ptr_comp(0x35, 0x34, 2)",
               ptr_comp((char*)0x35, (char*)0x34, 2), false);

    check_bool("ptr_comp(0x5, 0x8, 3)",
               ptr_comp((char *)0x5, (char*)0x8, 3), true);
    check_bool("ptr_comp(0x8, 0x8, 3)",
               ptr_comp((char *)0x8, (char*)0x8, 3), true);
    check_bool("ptr_comp(0x12, 0x8, 3)",
               ptr_comp((char *)0x12, (char*)0x8, 3), false);

    check_bool("ptr_comp(0xab, 0xab, 4)",
               ptr_comp((char*)0xab, (char*)0xab, 4), false);
    check_bool("ptr_comp(0xa0, 0xab, 4)",
               ptr_comp((char*)0xa0, (char*)0xab, 4), false);
    check_bool("ptr_comp(0xb0, 0xab, 4)",
               ptr_comp((char*)0xb0, (char*)0xab, 4), true);

    check_bool("ptr_comp(0xab, 0xab, 5)",
               ptr_comp((char*)0xab, (char*)0xab, 5), true);
    check_bool("ptr_comp(0xa0, 0xab, 5)",
               ptr_comp((char*)0xa0, (char*)0xab, 5), false);
    check_bool("ptr_comp(0xb0, 0xab, 5)",
               ptr_comp((char*)0xb0, (char*)0xab, 5), true);

    check_bool("ptr_comp(NULL, NULL, 6)",
               ptr_comp(NULL, NULL, 6), false);

    /* void_ptr_comp() tests */
    check_bool("void_ptr_comp(NULL, NULL, -1)",
               void_ptr_comp(NULL, NULL, -1), false);

    check_bool("void_ptr_comp(NULL, NULL, 0)",
               void_ptr_comp(NULL, NULL, 0), true);
    check_bool("void_ptr_comp(NULL, 0x1234, 0)",
               void_ptr_comp(NULL, (int*)0x1234, 0), false);

    check_bool("void_ptr_comp(NULL, NULL, 1)",
               void_ptr_comp(NULL, NULL, 1), false);
    check_bool("void_ptr_comp(NULL, 0x1234, 1)",
               void_ptr_comp(NULL, (int*)0x1234, 1), true);

    check_bool("void_ptr_comp(NULL, 0x1234, 2)",
               void_ptr_comp(NULL, (int*)0x1234, 2), true);
    check_bool("void_ptr_comp(0x10, 0x10, 2)",
               void_ptr_comp((void*)0x10, (int*)0x10, 2), false);
    check_bool("void_ptr_comp(0x35, 0x34, 2)",
               void_ptr_comp((void*)0x35, (int*)0x34, 2), false);

    check_bool("void_ptr_comp(0x5, 0x8, 3)",
               void_ptr_comp((void *)0x5, (int*)0x8, 3), true);
    check_bool("void_ptr_comp(0x8, 0x8, 3)",
               void_ptr_comp((void *)0x8, (int*)0x8, 3), true);
    check_bool("void_ptr_comp(0x12, 0x8, 3)",
               void_ptr_comp((void *)0x12, (int*)0x8, 3), false);

    check_bool("void_ptr_comp(0xab, 0xab, 4)",
               void_ptr_comp((void*)0xab, (int*)0xab, 4), false);
    check_bool("void_ptr_comp(0xa0, 0xab, 4)",
               void_ptr_comp((void*)0xa0, (int*)0xab, 4), false);
    check_bool("void_ptr_comp(0xb0, 0xab, 4)",
               void_ptr_comp((void*)0xb0, (int*)0xab, 4), true);

    check_bool("void_ptr_comp(0xab, 0xab, 5)",
               void_ptr_comp((void*)0xab, (int*)0xab, 5), true);
    check_bool("void_ptr_comp(0xa0, 0xab, 5)",
               void_ptr_comp((void*)0xa0, (int*)0xab, 5), false);
    check_bool("void_ptr_comp(0xb0, 0xab, 5)",
               void_ptr_comp((void*)0xb0, (int*)0xab, 5), true);

    check_bool("void_ptr_comp(NULL, NULL, 6)",
               void_ptr_comp(NULL, NULL, 6), false);

    /* null_ptr_comp() tests */
    check_bool("null_ptr_comp(NULL, 0)", null_ptr_comp(NULL, 0), true);
    check_bool("null_ptr_comp(0xff, 0)",
               null_ptr_comp((unsigned *)0xff, 0), false);
    check_bool("null_ptr_comp(NULL, 1)", null_ptr_comp(NULL, 1), false);
    check_bool("null_ptr_comp(0xff, 1)",
               null_ptr_comp((unsigned *)0xff, 1), true);
    check_bool("null_ptr_comp(NULL, 2)", null_ptr_comp(NULL, 2), false);
    check_bool("null_ptr_comp(0xff, 2)",
               null_ptr_comp((unsigned *)0xff, 2), true);
    check_bool("null_ptr_comp(NULL, 3)", null_ptr_comp(NULL, 3), true);
    check_bool("null_ptr_comp(0xff, 3)",
               null_ptr_comp((unsigned *)0xff, 3), false);
    check_bool("null_ptr_comp(NULL, 4)", null_ptr_comp(NULL, 4), false);
    check_bool("null_ptr_comp(0xff, 4)",
               null_ptr_comp((unsigned *)0xff, 4), true);
    check_bool("null_ptr_comp(NULL, 5)", null_ptr_comp(NULL, 5), true);
    check_bool("null_ptr_comp(0xff, 5)",
               null_ptr_comp((unsigned *)0xff, 5), true);

    check_exit();
}

