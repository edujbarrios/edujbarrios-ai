#include "jaro_winkler_similarity.h"
#include <stddef.h>

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

bool jaro_winkler_similarity(const char *a, const char *b, double *score_out) {
    if (!score_out) return false;
    size_t n, m;
    if (!checked_length(a, &n) || !checked_length(b, &m)) return false;
    if (n == 0 || m == 0) {
        *score_out = n == m ? 1.0 : 0.0;
        return true;
    }

    size_t window = (n > m ? n : m) / 2;
    if (window > 0) --window;
    bool matched_a[512] = {false};
    bool matched_b[512] = {false};
    size_t matches = 0;
    for (size_t i = 0; i < n; ++i) {
        size_t start = i > window ? i - window : 0;
        size_t end = i + window + 1;
        if (end > m) end = m;
        for (size_t j = start; j < end; ++j) {
            if (!matched_b[j] && a[i] == b[j]) {
                matched_a[i] = true;
                matched_b[j] = true;
                ++matches;
                break;
            }
        }
    }
    if (matches == 0) {
        *score_out = 0.0;
        return true;
    }

    size_t j = 0;
    size_t mismatches = 0;
    for (size_t i = 0; i < n; ++i) {
        if (!matched_a[i]) continue;
        while (!matched_b[j]) ++j;
        if (a[i] != b[j]) ++mismatches;
        ++j;
    }
    const double matched = (double)matches;
    const double jaro = (matched / (double)n + matched / (double)m
                        + (matched - (double)mismatches / 2.0) / matched) / 3.0;
    size_t prefix = 0;
    size_t limit = n < m ? n : m;
    if (limit > 4) limit = 4;
    while (prefix < limit && a[prefix] == b[prefix]) ++prefix;
    *score_out = jaro > 0.7 ? jaro + 0.1 * (double)prefix * (1.0 - jaro) : jaro;
    return true;
}
