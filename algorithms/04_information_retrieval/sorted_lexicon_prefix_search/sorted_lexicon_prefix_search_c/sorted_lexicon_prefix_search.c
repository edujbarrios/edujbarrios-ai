#include "sorted_lexicon_prefix_search.h"
#include <string.h>

static bool valid_text(const char *s, bool allow_empty) {
    if (!s) return false;
    size_t length = 0;
    while (length < 512 && s[length] != '\0') {
        if (s[length] < 'a' || s[length] > 'z') return false;
        ++length;
    }
    return s[length] == '\0' && (allow_empty || length != 0);
}

bool sorted_lexicon_prefix_search(const char *const *terms, size_t n,
                                  const char *prefix, size_t *indices,
                                  size_t *count) {
    if (!terms || !indices || !count || !valid_text(prefix, true)) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!valid_text(terms[i], false) ||
            (i != 0 && strcmp(terms[i-1], terms[i]) >= 0)) return false;
    }
    size_t low = 0, high = n;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        if (strcmp(terms[mid], prefix) < 0) low = mid + 1;
        else high = mid;
    }
    const size_t prefix_length = strlen(prefix);
    size_t written = 0;
    for (size_t i = low; i < n && strncmp(terms[i], prefix, prefix_length) == 0; ++i)
        indices[written++] = i;
    *count = written;
    return true;
}
