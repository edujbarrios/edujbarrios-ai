#include "longest_common_substring.h"

static bool checked_length(const char *value, size_t *length) {
    if (!value) return false;
    for (size_t i = 0; i <= 512; ++i) {
        unsigned char ch = (unsigned char)value[i];
        if (ch == 0) { *length = i; return true; }
        if (ch < 32 || ch > 126) return false;
    }
    return false;
}

bool longest_common_substring(const char *a, const char *b,
                              size_t *length_out,
                              size_t *start_a_out, size_t *start_b_out) {
    if (!length_out || !start_a_out || !start_b_out ||
        length_out == start_a_out || length_out == start_b_out ||
        start_a_out == start_b_out) return false;
    size_t n, m;
    if (!checked_length(a, &n) || !checked_length(b, &m)) return false;

    size_t rows[2][513] = {{0}};
    size_t *prev = rows[0], *curr = rows[1];
    size_t best = 0, best_a = 0, best_b = 0;
    for (size_t i = 1; i <= n; ++i) {
        curr[0] = 0;
        for (size_t j = 1; j <= m; ++j) {
            curr[j] = a[i - 1] == b[j - 1] ? prev[j - 1] + 1 : 0;
            const size_t len = curr[j];
            const size_t sa = i - len, sb = j - len;
            if (len > best ||
                (len != 0 && len == best &&
                 (sa < best_a || (sa == best_a && sb < best_b)))) {
                best = len;
                best_a = sa;
                best_b = sb;
            }
        }
        size_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }
    *length_out = best;
    *start_a_out = best_a;
    *start_b_out = best_b;
    return true;
}
