// tests/test_parser.c
#include <check.h>
#include <stdbool.h>
#include "../include/parser.h"

/* Helper to free parse trees in tests */
extern void free_cmds(cmd_t *head);

/* 1) Simple command with no args */
START_TEST(test_parse_simple)
{
    char *tokens[] = {"ls", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_ptr_nonnull(c);
    ck_assert_str_eq(c->argv[0], "ls");
    ck_assert_ptr_null(c->argv[1]);
    ck_assert_ptr_null(c->infile);
    ck_assert_ptr_null(c->outfile);
    ck_assert(!c->background);
    ck_assert_ptr_null(c->next);
    free_cmds(c);
}
END_TEST

/* 2) One argument */
START_TEST(test_parse_one_arg)
{
    char *tokens[] = {"echo", "hello", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "echo");
    ck_assert_str_eq(c->argv[1], "hello");
    ck_assert_ptr_null(c->argv[2]);
    free_cmds(c);
}
END_TEST

/* 3) Multiple arguments */
START_TEST(test_parse_multiple_args)
{
    char *tokens[] = {"gcc", "-Wall", "-o", "app", "main.c", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "gcc");
    ck_assert_str_eq(c->argv[1], "-Wall");
    ck_assert_str_eq(c->argv[2], "-o");
    ck_assert_str_eq(c->argv[3], "app");
    ck_assert_str_eq(c->argv[4], "main.c");
    ck_assert_ptr_null(c->argv[5]);
    free_cmds(c);
}
END_TEST

/* 4) Quoted argument passed through lexer */
START_TEST(test_parse_quoted_arg)
{
    char *tokens[] = {"echo", "hello world", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[1], "hello world");
    free_cmds(c);
}
END_TEST

/* 5) Input redirection */
START_TEST(test_parse_input_redir)
{
    char *tokens[] = {"grep", "foo", "<", "in.txt", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "grep");
    ck_assert_ptr_null(c->outfile);
    ck_assert_str_eq(c->infile, "in.txt");
    free_cmds(c);
}
END_TEST

/* 6) Output redirection overwrite */
START_TEST(test_parse_output_redir)
{
    char *tokens[] = {"ls", ">", "out.txt", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "ls");
    ck_assert_str_eq(c->outfile, "out.txt");
    ck_assert(!c->append);
    free_cmds(c);
}
END_TEST

/* 7) Output redirection append */
START_TEST(test_parse_append_redir)
{
    char *tokens[] = {"echo", "hi", ">>", "log.txt", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "echo");
    ck_assert_str_eq(c->outfile, "log.txt");
    ck_assert(c->append);
    free_cmds(c);
}
END_TEST

/* 8) Background-only command */
START_TEST(test_parse_background)
{
    char *tokens[] = {"sleep", "10", "&", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "sleep");
    ck_assert(c->background);
    free_cmds(c);
}
END_TEST

/* 9) Simple two-stage pipeline */
START_TEST(test_parse_pipe_two)
{
    char *tokens[] = {"ls", "|", "wc", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "ls");
    ck_assert_ptr_nonnull(c->next);
    ck_assert_str_eq(c->next->argv[0], "wc");
    free_cmds(c);
}
END_TEST

/* 10) Pipeline with arguments */
START_TEST(test_parse_pipe_with_args)
{
    char *tokens[] = {"ps", "aux", "|", "grep", "ssh", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[1], "aux");
    ck_assert_str_eq(c->next->argv[0], "grep");
    ck_assert_str_eq(c->next->argv[1], "ssh");
    free_cmds(c);
}
END_TEST

/* 11) Pipeline with background on last stage */
START_TEST(test_parse_pipe_background)
{
    char *tokens[] = {"cmd1", "|", "cmd2", "&", NULL};
    cmd_t *c = parse(tokens);
    ck_assert(!c->background);
    ck_assert(c->next->background);
    free_cmds(c);
}
END_TEST

/* 12) Pipeline with redirection on last stage */
START_TEST(test_parse_pipe_with_redir)
{
    char *tokens[] = {"cat", "file", "|", "grep", "foo", ">", "out", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_ptr_null(c->outfile);
    ck_assert_str_eq(c->next->outfile, "out");
    free_cmds(c);
}
END_TEST

/* 13) Three-stage pipeline */
START_TEST(test_parse_three_pipelines)
{
    char *tokens[] = {"a", "|", "b", "|", "c", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "a");
    ck_assert_str_eq(c->next->argv[0], "b");
    ck_assert_str_eq(c->next->next->argv[0], "c");
    free_cmds(c);
}
END_TEST

/* 14) Complex pipeline with mixed redirections */
START_TEST(test_parse_complex_pipeline)
{
    char *tokens[] = {
        "a", "arg", "|",
        "b", "<", "in", "|",
        "c", ">>", "out", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->next->argv[0], "b");
    ck_assert_str_eq(c->next->infile, "in");
    ck_assert_str_eq(c->next->next->outfile, "out");
    free_cmds(c);
}
END_TEST

/* 15) Same command with input and output redirections */
START_TEST(test_parse_in_and_out)
{
    char *tokens[] = {"sort", "<", "in.txt", ">", "out.txt", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "sort");
    ck_assert_str_eq(c->infile, "in.txt");
    ck_assert_str_eq(c->outfile, "out.txt");
    free_cmds(c);
}
END_TEST

/* 16) Redirection plus background */
START_TEST(test_parse_redir_and_background)
{
    char *tokens[] = {"cmd", "<", "in", ">>", "out", "&", NULL};
    cmd_t *c = parse(tokens);
    ck_assert(c->background);
    ck_assert_str_eq(c->infile, "in");
    ck_assert_str_eq(c->outfile, "out");
    free_cmds(c);
}
END_TEST

/* 17) Missing outfile name (parse stops gracefully) */
START_TEST(test_parse_missing_outfile)
{
    char *tokens[] = {"ls", ">", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "ls");
    ck_assert_ptr_null(c->outfile);
    free_cmds(c);
}
END_TEST

/* 18) Trailing pipe (no second stage) */
START_TEST(test_parse_trailing_pipe)
{
    char *tokens[] = {"ls", "|", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[0], "ls");
    ck_assert_ptr_null(c->next);
    free_cmds(c);
}
END_TEST

/* 19) Command with a space-containing filename */
START_TEST(test_parse_spaced_filename)
{
    char *tokens[] = {"cp", "my file.txt", "dest.txt", NULL};
    cmd_t *c = parse(tokens);
    ck_assert_str_eq(c->argv[1], "my file.txt");
    free_cmds(c);
}
END_TEST

/* 20) Multiple background markers */
START_TEST(test_parse_multiple_backgrounds)
{
    char *tokens[] = {"cmd1", "&", "cmd2", "&", NULL};
    cmd_t *c = parse(tokens);
    ck_assert(c->background);
    ck_assert_ptr_nonnull(c->next);
    ck_assert(c->next->background);
    free_cmds(c);
}
END_TEST

Suite *parser_suite(void)
{
    Suite *s = suite_create("Parser");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_parse_simple);
    tcase_add_test(tc, test_parse_one_arg);
    tcase_add_test(tc, test_parse_multiple_args);
    tcase_add_test(tc, test_parse_quoted_arg);
    tcase_add_test(tc, test_parse_input_redir);
    tcase_add_test(tc, test_parse_output_redir);
    tcase_add_test(tc, test_parse_append_redir);
    tcase_add_test(tc, test_parse_background);
    tcase_add_test(tc, test_parse_pipe_two);
    tcase_add_test(tc, test_parse_pipe_with_args);
    tcase_add_test(tc, test_parse_pipe_background);
    tcase_add_test(tc, test_parse_pipe_with_redir);
    tcase_add_test(tc, test_parse_three_pipelines);
    tcase_add_test(tc, test_parse_complex_pipeline);
    tcase_add_test(tc, test_parse_in_and_out);
    tcase_add_test(tc, test_parse_redir_and_background);
    tcase_add_test(tc, test_parse_missing_outfile);
    tcase_add_test(tc, test_parse_trailing_pipe);
    tcase_add_test(tc, test_parse_spaced_filename);
    tcase_add_test(tc, test_parse_multiple_backgrounds);

    suite_add_tcase(s, tc);
    return s;
}
