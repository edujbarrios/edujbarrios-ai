#include "kmp_substring_search.h"

static bool checked_length(const char *value, size_t *length) {
    if (!value) return false;
    for (size_t i = 0; i <= 512; ++i) {
        unsigned char ch = (unsigned char)value[i];
        if (ch == 0) {
            *length = i;
            return true;
        }
        if (ch < 32 || ch > 126) return false;
    }
    return false;
}

bool kmp_substring_search(const char *text, const char *pattern,
                          size_t positions[513], size_t *count_out) {
    if (!positions || !count_out) return false;
    size_t n, m;
    if (!checked_length(text, &n) || !checked_length(pattern, &m)) return false;

    if (m == 0) {
        for (size_t i = 0; i <= n; ++i) positions[i] = i;
        *count_out = n + 1;
        return true;
    }

    size_t prefix[512] = {0};
    size_t j = 0;
    for (size_t i = 1; i < m; ++i) {
        while (j > 0 && pattern[i] != pattern[j]) j = prefix[j - 1];
        if (pattern[i] == pattern[j]) ++j;
        prefix[i] = j;
    }

    size_t count = 0;
    j = 0;
    for (size_t i = 0; i < n; ++i) {
        while (j > 0 && text[i] != pattern[j]) j = prefix[j - 1];
        if (text[i] == pattern[j]) ++j;
        if (j == m) {
            positions[count++] = i + 1 - m;
            j = prefix[j - 1];
        }
    }
    *count_out = count;
    return true;
}
