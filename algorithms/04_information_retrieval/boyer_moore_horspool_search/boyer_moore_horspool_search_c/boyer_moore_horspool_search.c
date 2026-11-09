#include "boyer_moore_horspool_search.h"

static bool checked_length(const char *s, size_t *out) {
    if (!s) return false;
    for (size_t i = 0; i <= 512; ++i) {
        unsigned char ch = (unsigned char)s[i];
        if (ch == 0) { *out = i; return true; }
        if (ch < 32 || ch > 126) return false;
    }
    return false;
}

bool boyer_moore_horspool_search(
    const char *text, const char *pattern, size_t positions[513],
    size_t *count_out) {
    if (!positions || !count_out) return false;
    size_t n, m;
    if (!checked_length(text, &n) || !checked_length(pattern, &m)) return false;
    if (m == 0) {
        for (size_t i = 0; i <= n; ++i) positions[i] = i;
        *count_out = n + 1;
        return true;
    }
    if (m > n) { *count_out = 0; return true; }

    size_t shift[128];
    for (size_t i = 0; i < 128; ++i) shift[i] = m;
    for (size_t j = 0; j + 1 < m; ++j)
        shift[(unsigned char)pattern[j]] = m - 1 - j;
    size_t count = 0;
    size_t start = 0;
    while (start <= n - m) {
        size_t j = m;
        while (j > 0 && text[start + j - 1] == pattern[j - 1]) --j;
        if (j == 0) positions[count++] = start;
        start += shift[(unsigned char)text[start + m - 1]];
    }
    *count_out = count;
    return true;
}
