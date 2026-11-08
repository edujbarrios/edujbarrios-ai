#include "rabin_karp_substring_search.h"
#include <stdint.h>
#include <string.h>

#define BASE UINT64_C(257)
#define MODULUS UINT64_C(1000000007)

static bool checked_length(const char *s, size_t *out) {
    if (!s) return false;
    for (size_t i = 0; i <= 512; ++i) {
        unsigned char ch = (unsigned char)s[i];
        if (ch == 0) { *out = i; return true; }
        if (ch < 32 || ch > 126) return false;
    }
    return false;
}

bool rabin_karp_substring_search(const char *text, const char *pattern,
                                 size_t positions[513], size_t *count_out) {
    if (!positions || !count_out) return false;
    size_t n, m;
    if (!checked_length(text, &n) || !checked_length(pattern, &m)) return false;
    if (m == 0) {
        for (size_t i = 0; i <= n; ++i) positions[i] = i;
        *count_out = n + 1;
        return true;
    }
    if (m > n) { *count_out = 0; return true; }

    uint64_t high_power = 1, ph = 0, th = 0;
    for (size_t i = 1; i < m; ++i) high_power = (high_power * BASE) % MODULUS;
    for (size_t i = 0; i < m; ++i) {
        ph = (ph * BASE + (unsigned char)pattern[i]) % MODULUS;
        th = (th * BASE + (unsigned char)text[i]) % MODULUS;
    }
    size_t count = 0;
    for (size_t start = 0; start <= n - m; ++start) {
        if (ph == th && memcmp(text + start, pattern, m) == 0)
            positions[count++] = start;
        if (start < n - m) {
            uint64_t outgoing = ((unsigned char)text[start] * high_power) % MODULUS;
            th = (((th + MODULUS - outgoing) % MODULUS) * BASE
                  + (unsigned char)text[start + m]) % MODULUS;
        }
    }
    *count_out = count;
    return true;
}
