#include "longest_common_substring.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void check(const char *a, const char *b,
                  size_t expected_len, size_t expected_a, size_t expected_b) {
    size_t length = 99, start_a = 99, start_b = 99;
    assert(longest_common_substring(a, b, &length, &start_a, &start_b));
    assert(length == expected_len && start_a == expected_a && start_b == expected_b);
}

int main(void) {
    check("banana", "ananas", 5, 1, 0);
    check("abcdef", "zabcf", 3, 0, 1);
    check("ababa", "babab", 4, 0, 1);
    check("abcabc", "abc", 3, 0, 0);
    check("aa", "baaa", 2, 0, 1);
    check("ABC", "abc", 0, 0, 0);
    check("", "", 0, 0, 0);
    check("", "abc", 0, 0, 0);
    check("abc", "", 0, 0, 0);
    check("A", "A", 1, 0, 0);
    char max[514];
    memset(max, 'a', 512);
    max[512] = '\0';
    check(max, max, 512, 0, 0);
    size_t length = 99, sa = 99, sb = 99;
    max[512] = 'a';
    max[513] = '\0';
    assert(!longest_common_substring(max, "a", &length, &sa, &sb));
    assert(!longest_common_substring("é", "e", &length, &sa, &sb));
    assert(!longest_common_substring("a\tb", "a", &length, &sa, &sb));
    assert(!longest_common_substring(NULL, "a", &length, &sa, &sb));
    assert(!longest_common_substring("a", "a", NULL, &sa, &sb));
    assert(!longest_common_substring("a", "a", &length, &length, &sb));
    assert(length == 99 && sa == 99 && sb == 99);
    puts("C longest common substring tests passed");
    return 0;
}
