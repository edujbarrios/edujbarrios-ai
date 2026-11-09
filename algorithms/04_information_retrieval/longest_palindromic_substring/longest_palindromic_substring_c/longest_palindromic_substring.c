#include "longest_palindromic_substring.h"

static bool checked_length(const char *text, size_t *length) {
    if (!text) return false;
    for (size_t i = 0; i <= 512; ++i) {
        unsigned char ch = (unsigned char)text[i];
        if (ch == 0) { *length = i; return true; }
        if (ch < 32 || ch > 126) return false;
    }
    return false;
}

static void expand(const char *text, int n, int left, int right,
                   size_t *best_length, size_t *best_start) {
    while (left >= 0 && right < n && text[left] == text[right]) {
        const size_t length = (size_t)(right - left + 1);
        const size_t start = (size_t)left;
        if (length > *best_length ||
            (length == *best_length && start < *best_start)) {
            *best_length = length;
            *best_start = start;
        }
        --left;
        ++right;
    }
}

bool longest_palindromic_substring(const char *text,
                                   size_t *length_out, size_t *start_out) {
    if (!length_out || !start_out || length_out == start_out) return false;
    size_t length;
    if (!checked_length(text, &length)) return false;
    size_t best_length = 0, best_start = 0;
    for (int center = 0; center < (int)length; ++center) {
        expand(text, (int)length, center, center, &best_length, &best_start);
        expand(text, (int)length, center, center + 1, &best_length, &best_start);
    }
    *length_out = best_length;
    *start_out = best_start;
    return true;
}
