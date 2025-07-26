#include <check.h>
#include "../include/executor.h"

START_TEST(test_execute_null_input)
{
    int val = exec_cmds(NULL);
    ck_assert_int_eq(val, 0);
}
END_TEST

START_TEST(test_execute_single_cmd)
{
    char *t1[] = {"true", NULL};
    ck_assert_int_eq(exec_cmds(parse(t1)), 0);

    char *t2[] = {"false", NULL};
    ck_assert_int_eq(exec_cmds(parse(t2)), 1);
}
END_TEST

START_TEST(test_execute_outfile_append_cmd)
{
    char *tokens[] = { "sh", "-c", "echo hello", ">>", "/tmp/test.txt", NULL };
    ck_assert_int_eq(exec_cmds(parse(tokens)), 0);
}
END_TEST

START_TEST(test_execute_outfile_cmd)
{
    char *tokens[] = { "sh", "-c", "echo hello", ">", "/tmp/test.txt", NULL };
    ck_assert_int_eq(exec_cmds(parse(tokens)), 0);
}
END_TEST

Suite *executer_suite(void)
{
    Suite *s = suite_create("Executer");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_execute_null_input);
    tcase_add_test(tc, test_execute_single_cmd);
    tcase_add_test(tc, test_execute_outfile_append_cmd);
    tcase_add_test(tc, test_execute_outfile_cmd);

    suite_add_tcase(s, tc);
    return s;
}
