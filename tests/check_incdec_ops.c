#include "check_utils.h"

enum ops
{
  pre_inc,
  post_inc,
  pre_dec,
  post_dec
};

int
i_ops(enum ops op, int var, int *var_res);

unsigned *
uint_ptr_ops(enum ops op, unsigned *ptr, bool return_ptr);

char *
char_ptr_ops(enum ops op, char *ptr, bool return_var);

int
invoke_arry_sum(int array_num);

/*---------------------------------------------------------------------------*
 *                              run tests                                    *
 *---------------------------------------------------------------------------*/
int
main()
{
    /* i_ops() tests */
    int res, var, var_res;

    var = 20;
    res = i_ops(pre_inc, var, &var_res);
    check_cond("i_ops(pre_inc, 20, &var_res)",
               res == (var + 1) && var_res == (var + 1));

    var = -2;
    res = i_ops(pre_dec, var, &var_res);
    check_cond("i_ops(pre_dec, -2, &var_res)",
               res == (var - 1) && var_res == (var - 1));

    var = 100;
    res = i_ops(post_inc, var, &var_res);
    check_cond("i_ops(post_inc, 100, &var_res)",
               res == var && var_res == var + 1);

    var = 0;
    res = i_ops(post_dec, var, &var_res);
    check_cond("i_ops(post_dec, 0, &var_res)",
               res == var && var_res == var - 1);

    /* uint_ptr_ops() tests */

    check_pointer("uint_ptr_ops(post_dec, 0xa0, true)",
                  uint_ptr_ops(post_dec, (unsigned *)0xa0, true),
                  (unsigned *)(0xa0 - 4));
    check_pointer("uint_ptr_ops(post_dec, 0xa0, false)",
                  uint_ptr_ops(post_dec, (unsigned *)0xa0, false),
                  (unsigned *)0xa0);

    check_pointer("uint_ptr_ops(pre_dec, 0x14, true)",
                  uint_ptr_ops(pre_dec, (unsigned *)0x14, true),
                  (unsigned *)(0x14 - 4));
    check_pointer("uint_ptr_ops(pre_dec, 0x14, false)",
                  uint_ptr_ops(pre_dec, (unsigned *)0x14, false),
                  (unsigned *)(0x14 - 4));

    check_pointer("uint_ptr_ops(post_inc, 0x14, true)",
                  uint_ptr_ops(post_inc, (unsigned *)0x14, true),
                  (unsigned *)(0x14 + 4));
    check_pointer("uint_ptr_ops(post_inc, 0x14, false)",
                  uint_ptr_ops(post_inc, (unsigned *)0x14, false),
                  (unsigned *)0x14);

    check_pointer("uint_ptr_ops(pre_inc, 0x14, true)",
                  uint_ptr_ops(pre_inc, (unsigned *)0x14, true),
                  (unsigned *)(0x14 + 4));
    check_pointer("uint_ptr_ops(pre_inc, 0x14, false)",
                  uint_ptr_ops(pre_inc, (unsigned *)0x14, false),
                  (unsigned *)(0x14 + 4));

    /* char_ptr_ops() tests */

    check_pointer("char_ptr_ops(post_dec, 0xa0, true)",
                  char_ptr_ops(post_dec, (char *)0xa0, true),
                  (char *)(0xa0 - 1));
    check_pointer("char_ptr_ops(post_dec, 0xa0, false)",
                  char_ptr_ops(post_dec, (char *)0xa0, false),
                  (char *)0xa0);

    check_pointer("char_ptr_ops(pre_dec, 0x14, true)",
                  char_ptr_ops(pre_dec, (char *)0x14, true),
                  (char *)(0x14 - 1));
    check_pointer("char_ptr_ops(pre_dec, 0x14, false)",
                  char_ptr_ops(pre_dec, (char *)0x14, false),
                  (char *)(0x14 - 1));

    check_pointer("char_ptr_ops(post_inc, 0x14, true)",
                  char_ptr_ops(post_inc, (char *)0x14, true),
                  (char *)(0x14 + 1));
    check_pointer("char_ptr_ops(post_inc, 0x14, false)",
                  char_ptr_ops(post_inc, (char *)0x14, false),
                  (char *)0x14);

    check_pointer("char_ptr_ops(pre_inc, 0x14, true)",
                  char_ptr_ops(pre_inc, (char *)0x14, true),
                  (char *)(0x14 + 1));
    check_pointer("char_ptr_ops(pre_inc, 0x14, false)",
                  char_ptr_ops(pre_inc, (char *)0x14, false),
                  (char *)(0x14 + 1));

    /* arry_sum() tests */
    check_int("invoke_arry_sum(-1)", invoke_arry_sum(-1), 0);
    check_int("invoke_arry_sum(0)", invoke_arry_sum(0), 5 + 8 + 2);
    check_int("invoke_arry_sum(1)", invoke_arry_sum(1), 3 + 1 + 4 + 1 + 5);
    check_int("invoke_arry_sum(2)", invoke_arry_sum(2), 10 + (-2));


    check_exit();
}

