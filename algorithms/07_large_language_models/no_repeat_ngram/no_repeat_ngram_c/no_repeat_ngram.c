#include "no_repeat_ngram.h"
#include <string.h>

bool no_repeat_ngram(const size_t *history, size_t length, size_t vocab,
                     size_t n, bool *blocked) {
    if (!blocked || (length && !history) || length > 16384 ||
        vocab < 1 || vocab > 4096 || n < 1 || n > 32) return false;
    for (size_t i = 0; i < length; ++i)
        if (history[i] >= vocab) return false;
    memset(blocked, 0, vocab * sizeof(*blocked));
    if (length < n) return true;
    for (size_t i = 0; i + n <= length; ++i) {
        bool same = true;
        for (size_t j = 0; j + 1 < n; ++j) {
            if (history[i + j] != history[length - n + 1 + j]) {
                same = false;
                break;
            }
        }
        if (same) blocked[history[i + n - 1]] = true;
    }
    return true;
}
