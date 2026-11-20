#ifndef EDUJBARRIOS_AI_MINHASH_SIGNATURE_H
#define EDUJBARRIOS_AI_MINHASH_SIGNATURE_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MINHASH_PRIME UINT32_C(65521)

/* Token IDs in [0,65520], a in [1,65520], b in [0,65520].
 * n may be zero; m must be >=1. All pointers must be nonnull, even for n=0.
 * Signature must hold m uint32_t entries and must not overlap inputs.
 * Failure leaves caller-owned signature unchanged. */
bool minhash_signature(const uint32_t *tokens, size_t n,
                       const uint32_t *a, const uint32_t *b,
                       size_t m, uint32_t *signature);
#endif
