#include <check.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

#include "../label_gen.h"

/*---------------------------------------------------------------------------*
 *                                test cases                                 *
 *---------------------------------------------------------------------------*/

START_TEST(test_label_gen_next)
{
    label_gen_t label_gen;
    char l[LABEL_MAX_LEN];
    char *str;
    int i;
    int *iptr;
    GHashTable* htable;

    /* set up test environment */
    htable = g_hash_table_new_full(g_str_hash, g_str_equal, free, free);
    label_gen_init(&label_gen);

    for (i = 1; i < 3200; i++)
    {
        label_gen_next(&label_gen, l);

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
label_gen_add_test_cases(Suite *s)
{
    TCase *tc = tcase_create("label generator");

    tcase_add_test(tc, test_label_gen_next);

    suite_add_tcase(s, tc);     
}


