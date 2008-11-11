#include <check.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

#include "../java_trgt_int.h"

/*---------------------------------------------------------------------------*
 *                                test cases                                 *
 *---------------------------------------------------------------------------*/

#include <stdio.h>
START_TEST(test_java_trgt_get_next_label)
{

    java_trgt_comp_params_t param;
    char l[MAX_JAVA_LABEL];
    char *str;
    int i;
    int *iptr;
    GHashTable* htable;

    /* set up test environment */
    htable = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    param.out = stdout;
    param.class_name = "";
    strcpy(param.next_label, "A");

    for (i = 1; i < 3200; i++)
    {
        java_trgt_get_next_label(&param, l);

        /* check that this label have not been returned before */
        iptr = g_hash_table_lookup(htable, l);
        fail_if(iptr != NULL, 
                "duplicate label returned. '%s' returned on both"
                " %i and %i call.", l, iptr ? *iptr : -1 , i);
        /* store this label and the iteration when it was returned */
        str = malloc(sizeof(l));
        iptr = malloc(sizeof(i));
        strcpy(str, l);
        *iptr = i;
        g_hash_table_insert(htable, str, iptr);
    }
    /* cleanup */
    g_hash_table_destroy(htable);
}
END_TEST


/*---------------------------------------------------------------------------*
 *                           test suite constructor                          *
 *---------------------------------------------------------------------------*/

void
java_trgt_add_test_cases(Suite *s)
{
    /* Core test case */
    TCase *tc_core = tcase_create ("java backend");

    tcase_add_test (tc_core, test_java_trgt_get_next_label);

    suite_add_tcase (s, tc_core);     
}


