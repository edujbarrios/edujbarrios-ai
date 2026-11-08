#include "kmp_substring_search.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    size_t matches[513] = {999};
    size_t count = 999;
    assert(kmp_substring_search("aaaa", "aa", matches, &count));
    assert(count == 3 && matches[0] == 0 && matches[1] == 1 && matches[2] == 2);
    assert(kmp_substring_search("ABABDABACDABABCABAB", "ABABCABAB", matches, &count));
    assert(count == 1 && matches[0] == 10);
    assert(kmp_substring_search("abc", "z", matches, &count) && count == 0);
    assert(kmp_substring_search("abc", "ABC", matches, &count) && count == 0);
    assert(kmp_substring_search("abc", "", matches, &count));
    assert(count == 4 && matches[0] == 0 && matches[3] == 3);
    assert(kmp_substring_search("", "", matches, &count));
    assert(count == 1 && matches[0] == 0);
    assert(kmp_substring_search("", "a", matches, &count) && count == 0);

    char long_text[514];
    memset(long_text, 'a', 512);
    long_text[512] = '\0';
    assert(kmp_substring_search(long_text, "aa", matches, &count));
    assert(count == 511 && matches[510] == 510);
    assert(kmp_substring_search(long_text, "", matches, &count));
    assert(count == 513 && matches[512] == 512);

    long_text[512] = 'a';
    long_text[513] = '\0';
    matches[0] = 777;
    count = 777;
    assert(!kmp_substring_search(long_text, "a", matches, &count));
    assert(!kmp_substring_search("a\tb", "a", matches, &count));
    assert(!kmp_substring_search("abc", "é", matches, &count));
    assert(!kmp_substring_search(NULL, "a", matches, &count));
    assert(!kmp_substring_search("a", "a", NULL, &count));
    assert(!kmp_substring_search("a", "a", matches, NULL));
    assert(count == 777 && matches[0] == 777);
    puts("C KMP tests passed");
    return 0;
}
