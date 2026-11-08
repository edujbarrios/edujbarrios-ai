#include "rabin_karp_substring_search.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(void) {
    size_t p[513] = {777}, count = 777;
    assert(rabin_karp_substring_search("aaaa", "aa", p, &count));
    assert(count == 3 && p[0] == 0 && p[1] == 1 && p[2] == 2);
    assert(rabin_karp_substring_search("abababa", "aba", p, &count));
    assert(count == 3 && p[0] == 0 && p[1] == 2 && p[2] == 4);
    assert(rabin_karp_substring_search("ABABDABACDABABCABAB", "ABABCABAB", p, &count));
    assert(count == 1 && p[0] == 10);
    assert(rabin_karp_substring_search("abc", "ABC", p, &count) && count == 0);
    assert(rabin_karp_substring_search("ab", "abc", p, &count) && count == 0);
    assert(rabin_karp_substring_search("abc", "", p, &count));
    assert(count == 4 && p[0] == 0 && p[3] == 3);
    assert(rabin_karp_substring_search("", "", p, &count));
    assert(count == 1 && p[0] == 0);
    assert(rabin_karp_substring_search("", "x", p, &count) && count == 0);

    char big[514];
    memset(big, 'a', 512);
    big[512] = '\0';
    assert(rabin_karp_substring_search(big, "aa", p, &count));
    assert(count == 511 && p[510] == 510);
    assert(rabin_karp_substring_search(big, "", p, &count));
    assert(count == 513 && p[512] == 512);
    big[512] = 'a';
    big[513] = '\0';
    p[0] = 777; count = 777;
    assert(!rabin_karp_substring_search(big, "a", p, &count));
    assert(!rabin_karp_substring_search("a\tb", "a", p, &count));
    assert(!rabin_karp_substring_search("é", "e", p, &count));
    assert(!rabin_karp_substring_search(NULL, "a", p, &count));
    assert(!rabin_karp_substring_search("a", "b", NULL, &count));
    assert(!rabin_karp_substring_search("a", "b", p, NULL));
    assert(count == 777 && p[0] == 777);
    puts("C Rabin-Karp tests passed");
    return 0;
}
