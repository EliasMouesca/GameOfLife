// File: src/utils/utils_test.c

#include <assert.h>
#include <string.h>

#include "utils.h"

#define ASSERT_TRUE(x)  assert((x) && "ASSERT_TRUE failed")
#define ASSERT_STREQ(a,b) assert(strcmp((a),(b)) == 0)

static void test_trim_no_spaces_no_change() {
    char s[] = "hola";
    trim(s);
    ASSERT_STREQ(s, "hola");
}

static void test_trim_trailing_spaces_removed() {
    char s[] = "hola   ";
    trim(s);
    ASSERT_STREQ(s, "hola");
}

static void test_trim_leading_spaces_removed() {
    char s[] = "   hola";
    trim(s);
    ASSERT_STREQ(s, "hola");
}

static void test_trim_both_sides_removed() {
    char s[] = "   hola mundo   ";
    trim(s);
    ASSERT_STREQ(s, "hola mundo");
}

static void test_trim_tabs_newlines_and_spaces() {
    char s[] = "\t \n  hola \r\n\t  ";
    trim(s);
    ASSERT_STREQ(s, "hola");
}

static void test_trim_internal_spacing_preserved() {
    char s[] = "hola   mundo";
    trim(s);
    ASSERT_STREQ(s, "hola   mundo");
}

static void test_trim_all_whitespace_becomes_empty() {
    char s[] = " \t \n\r  ";
    trim(s);
    ASSERT_STREQ(s, "");
}

static void test_trim_empty_string_stays_empty() {
    char s[] = "";
    trim(s);
    ASSERT_STREQ(s, "");
}

static void test_trim_single_space_becomes_empty() {
    char s[] = " ";
    trim(s);
    ASSERT_STREQ(s, "");
}

static void test_trim_single_char() {
    char s[] = "a";
    trim(s);
    ASSERT_STREQ(s, "a");
}

static void test_trim_does_not_overrun_on_small_strings() {
    // casos frontera cortitos que suelen romper implementaciones
    char s1[] = "  a";
    char s2[] = "a  ";
    char s3[] = "  a  ";
    char s4[] = "\n";
    char s5[] = "\tX\t";

    trim(s1); ASSERT_STREQ(s1, "a");
    trim(s2); ASSERT_STREQ(s2, "a");
    trim(s3); ASSERT_STREQ(s3, "a");
    trim(s4); ASSERT_STREQ(s4, "");
    trim(s5); ASSERT_STREQ(s5, "X");
}

int main(void) {
    test_trim_no_spaces_no_change();
    test_trim_trailing_spaces_removed();
    test_trim_leading_spaces_removed();
    test_trim_both_sides_removed();
    test_trim_tabs_newlines_and_spaces();
    test_trim_internal_spacing_preserved();
    test_trim_all_whitespace_becomes_empty();
    test_trim_empty_string_stays_empty();
    test_trim_single_space_becomes_empty();
    test_trim_single_char();
    test_trim_does_not_overrun_on_small_strings();
    return 0;
}

