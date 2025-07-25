#include <check.h>
#include <stdlib.h>

Suite *lexer_suite(void);

int main(void)
{
    int failed = 0;
    SRunner *sr = srunner_create(lexer_suite());

    srunner_run_all(sr, CK_NORMAL);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
