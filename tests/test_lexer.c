#include <check.h>
#include "../include/lexer.h"

START_TEST(test_lex_word_single_split)
{
    char **tokens = lex("hi there");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "hi");
    ck_assert_str_eq(tokens[1], "there");
    ck_assert_ptr_null(tokens[2]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_word_split_multiple_space)
{
    char **tokens = lex("hi      there");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "hi");
    ck_assert_str_eq(tokens[1], "there");
    ck_assert_ptr_null(tokens[2]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_word_split_single_tab)
{
    char **tokens = lex("hi\tthere");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "hi");
    ck_assert_str_eq(tokens[1], "there");
    ck_assert_ptr_null(tokens[2]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_word_mix_space_tab)
{
    char **tokens = lex("hi\tthere how\tare you");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "hi");
    ck_assert_str_eq(tokens[1], "there");
    ck_assert_str_eq(tokens[2], "how");
    ck_assert_str_eq(tokens[3], "are");
    ck_assert_str_eq(tokens[4], "you");
    ck_assert_ptr_null(tokens[5]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_word_skip_space)
{
    char **tokens = lex("hi 'my name' there");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "hi");
    ck_assert_str_eq(tokens[1], "my name");
    ck_assert_str_eq(tokens[2], "there");
    ck_assert_ptr_null(tokens[3]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST


START_TEST(test_lex_word_skip_quote)
{
    char **tokens = lex("hi there it\'s me");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "hi");
    ck_assert_str_eq(tokens[1], "there");
    ck_assert_str_eq(tokens[2], "it's");
    ck_assert_str_eq(tokens[3], "me");
    ck_assert_ptr_null(tokens[4]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_escaped_space)
{
    char **tokens = lex("touch my\\ file.txt  next");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "touch");
    ck_assert_str_eq(tokens[1], "my file.txt");
    ck_assert_str_eq(tokens[2], "next");
    ck_assert_ptr_null(tokens[3]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_double_quotes_with_escape)
{
    char **tokens = lex("echo \"She said \\\"hi\\\"\" bye");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "echo");
    ck_assert_str_eq(tokens[1], "She said \"hi\"");
    ck_assert_str_eq(tokens[2], "bye");
    ck_assert_ptr_null(tokens[3]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_mixed_single_and_double)
{
    char **tokens = lex("cmd 'a b'\"c d\" e");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "cmd");
    ck_assert_str_eq(tokens[1], "a bc d");
    ck_assert_str_eq(tokens[2], "e");
    ck_assert_ptr_null(tokens[3]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_trailing_backslash)
{
    char **tokens = lex("path/to/dir\\");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "path/to/dir\\");
    ck_assert_ptr_null(tokens[1]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

START_TEST(test_lex_unmatched_quote)
{
    char **tokens = lex("say \"hello world");
    ck_assert_ptr_nonnull(tokens);
    ck_assert_str_eq(tokens[0], "say");
    ck_assert_str_eq(tokens[1], "hello world");
    ck_assert_ptr_null(tokens[2]);
    for (int i = 0; tokens[i]; i++) free(tokens[i]);
    free(tokens);
}
END_TEST

Suite *lexer_suite(void)
{
    Suite *s = suite_create("Lexer");
    TCase *tc = tcase_create("Basic");

    tcase_add_test(tc, test_lex_word_single_split);
    tcase_add_test(tc, test_lex_word_split_multiple_space); 
    tcase_add_test(tc, test_lex_word_split_single_tab); 
    tcase_add_test(tc, test_lex_word_mix_space_tab); 
    tcase_add_test(tc, test_lex_word_skip_space); 
    tcase_add_test(tc, test_lex_word_skip_quote);
    tcase_add_test(tc, test_lex_escaped_space);
    tcase_add_test(tc, test_lex_double_quotes_with_escape);
    tcase_add_test(tc, test_lex_mixed_single_and_double);
    tcase_add_test(tc, test_lex_trailing_backslash);
    tcase_add_test(tc, test_lex_unmatched_quote);

    suite_add_tcase(s, tc);
    return s;
}
