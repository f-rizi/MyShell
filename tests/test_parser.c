#define _POSIX_C_SOURCE 200809L
#include <check.h>
#include <stdlib.h>
#include <string.h>

#include "../include/parser.h"

START_TEST(test_parse_simple_command)
{
    char *tokens[] = {"apple", "banana", NULL};
    cmd_t *head = parse(tokens);
    ck_assert_ptr_nonnull(head);
    
    ck_assert_str_eq(head->argv[0], "apple");
    ck_assert_str_eq(head->argv[1], "banana");
    ck_assert_ptr_null(head->argv[2]);

    ck_assert_ptr_null(head->infile);
    ck_assert_ptr_null(head->outfile);

    ck_assert(head->append == false);
    ck_assert(head->background == false);
    ck_assert_ptr_null(head->next);

    free_cmds(head);
}
END_TEST

Suite *parser_suite(void)
{
    Suite *s = suite_create("Parser");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_parse_simple_command);

    suite_add_tcase(s, tc);
    return s;
}
