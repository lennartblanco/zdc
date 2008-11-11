#include <check.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*
 *                           test suite constructors                         *
 *---------------------------------------------------------------------------*/

void
java_trgt_add_test_cases(Suite *s);

/*---------------------------------------------------------------------------*
 *                               run test suites                             *
 *---------------------------------------------------------------------------*/

int main()
{
    int number_failed;

    /* set up test objects */
    Suite *s = suite_create ("xdc");
    SRunner *sr = srunner_create (s);
 
    /* add test cases to the suite */
    java_trgt_add_test_cases(s);

    /* run tests */
    srunner_run_all (sr, CK_NORMAL);
    number_failed = srunner_ntests_failed (sr);

    /* clean up */
    srunner_free (sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
