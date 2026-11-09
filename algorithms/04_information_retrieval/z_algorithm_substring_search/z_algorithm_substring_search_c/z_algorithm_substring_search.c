#include "z_algorithm_substring_search.h"

static bool checked_length(const char *str, size_t *len) {
    if (!str) return false;
    for (size_t i = 0; i <= 512; ++i) {
        const unsigned char value = (unsigned char)str[i];
        if (value == 0) { *len = i; return true; }
        if (value < 32 || value > 126) return false;
    }
    return false;
}

bool z_algorithm_substring_search(
    const char *text, const char *pattern, size_t positions[513],
    size_t *count_out) {
    if (!positions || !count_out || positions == count_out) return false;
    size_t n, m;
    if (!checked_length(text, &n) || !checked_length(pattern, &m)) return false;
    if (m == 0) {
        for (size_t i = 0; i <= n; ++i) positions[i] = i;
        *count_out = n + 1;
        return true;
    }
    if (m > n) { *count_out = 0; return true; }
    unsigned char seq[1025];
    size_t z[1025] = {0};
    for (size_t i = 0; i < m; ++i) seq[i] = (unsigned char)pattern[i];
    seq[m] = 0;
    for (size_t i = 0; i < n; ++i) seq[m+1+i] = (unsigned char)text[i];

    const size_t total = m + 1 + n;
    size_t left = 0, right = 0, count = 0;
    for (size_t i = 1; i < total; ++i) {
        if (i < right) {
            const size_t cap = right - i;
            const size_t known = z[i - left];
            z[i] = known < cap ? known : cap;
        }
        while (i + z[i] < total && seq[z[i]] == seq[i + z[i]]) ++z[i];
        if (i + z[i] > right) {
            left = i;
            right = i + z[i];
        }
        if (i >= m + 1 && z[i] >= m) positions[count++] = i - m - 1;
    }
    *count_out = count;
    return true;
}
