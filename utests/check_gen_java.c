#include <check.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

#include "../gen_java_int.h"

/*---------------------------------------------------------------------------*
 *                                test cases                                 *
 *---------------------------------------------------------------------------*/

#include <stdio.h>
START_TEST(test_gen_java_get_next_label)
{

    gen_java_comp_params_t param;
    char l[MAX_JAVA_LABEL];
    char *str;
    int i;
    GHashTable* htable;
    gpointer p;


    htable = g_hash_table_new(g_str_hash, g_str_equal);


    param.out = stdout;
    param.class_name = "";
    strcpy(param.next_label, "A");

    for (i = 1; i < 3200; i++)
    {
        gen_java_get_next_label(&param, l);

        p = g_hash_table_lookup(htable, l);
        fail_if(p != NULL, 
                "duplicate label returned. '%s' returned on both"
                " %i and %i call.", l, (int)p, i);
        str = malloc(sizeof(l));
        strcpy(str, l);
        g_hash_table_insert(htable, str, (gpointer)i);
    }
    g_hash_table_destroy(htable);
}
END_TEST


/*---------------------------------------------------------------------------*
 *                           test suite constructor                          *
 *---------------------------------------------------------------------------*/

void
gen_java_add_test_cases(Suite *s)
{
    /* Core test case */
    TCase *tc_core = tcase_create ("java backend");

    tcase_add_test (tc_core, test_gen_java_get_next_label);

    suite_add_tcase (s, tc_core);     
}


