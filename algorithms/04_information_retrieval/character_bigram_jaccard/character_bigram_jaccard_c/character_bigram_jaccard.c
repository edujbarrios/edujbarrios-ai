#include "character_bigram_jaccard.h"
#include <stddef.h>

#define ALPHABET 95
#define PAIRS (ALPHABET * ALPHABET)

static bool checked_length(const char *s, size_t *length_out) {
    if (!s) return false;
    for (size_t i = 0; i <= 512; ++i) {
        const unsigned char ch = (unsigned char)s[i];
        if (ch == 0) { *length_out = i; return true; }
        if (ch < 32 || ch > 126) return false;
    }
    return false;
}

bool character_bigram_jaccard(const char *a, const char *b,
                              double *similarity_out) {
    if (!similarity_out) return false;
    size_t na, nb;
    if (!checked_length(a, &na) || !checked_length(b, &nb)) return false;
    bool left[PAIRS] = {false}, right[PAIRS] = {false};
    for (size_t i = 1; i < na; ++i) {
        const size_t key = ((unsigned char)a[i-1] - 32u) * ALPHABET
                           + ((unsigned char)a[i] - 32u);
        left[key] = true;
    }
    for (size_t i = 1; i < nb; ++i) {
        const size_t key = ((unsigned char)b[i-1] - 32u) * ALPHABET
                           + ((unsigned char)b[i] - 32u);
        right[key] = true;
    }
    size_t intersection = 0, union_count = 0;
    for (size_t i = 0; i < PAIRS; ++i) {
        if (left[i] && right[i]) ++intersection;
        if (left[i] || right[i]) ++union_count;
    }
    *similarity_out = union_count == 0 ? 1.0
                       : (double)intersection / (double)union_count;
    return true;
}
