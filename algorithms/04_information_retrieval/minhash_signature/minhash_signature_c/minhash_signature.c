#include "minhash_signature.h"

bool minhash_signature(const uint32_t *tokens, size_t n,
                       const uint32_t *a, const uint32_t *b,
                       size_t m, uint32_t *signature) {
    if (!tokens || !a || !b || !signature || m == 0) return false;
    for (size_t i = 0; i < n; ++i) {
        if (tokens[i] >= MINHASH_PRIME) return false;
    }
    for (size_t j = 0; j < m; ++j) {
        if (a[j] == 0 || a[j] >= MINHASH_PRIME || b[j] >= MINHASH_PRIME)
            return false;
    }

    /* All input validation is complete before writing any output. */
    for (size_t j = 0; j < m; ++j) {
        uint32_t minimum = MINHASH_PRIME;
        for (size_t i = 0; i < n; ++i) {
            const uint64_t numerator = (uint64_t)a[j] * tokens[i] + b[j];
            const uint32_t hashed = (uint32_t)(numerator % MINHASH_PRIME);
            if (hashed < minimum) minimum = hashed;
        }
        signature[j] = minimum;
    }
    return true;
}
