#include <check.h>
#include "../include/executor.h"

START_TEST(test_execute_null_input)
{
    int val = exec_cmds(NULL);
    ck_assert_int_eq(val, 0);
}
END_TEST

Suite *executer_suite(void)
{
    Suite *s = suite_create("Executer");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_execute_null_input);

    suite_add_tcase(s, tc);
    return s;
}
