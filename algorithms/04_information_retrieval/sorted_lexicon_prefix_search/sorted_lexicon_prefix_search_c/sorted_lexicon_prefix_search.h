#ifndef EDUJBARRIOS_AI_SORTED_LEXICON_PREFIX_SEARCH_H
#define EDUJBARRIOS_AI_SORTED_LEXICON_PREFIX_SEARCH_H
#include <stdbool.h>
#include <stddef.h>

/* All terms are strictly sorted unique lowercase ASCII words (length 1..512).
 * Prefix may be empty (length 0..512); n may be zero.
 * Terms and caller-owned indices[n] must not overlap.
 * On failure, indices and *count are unchanged. */
bool sorted_lexicon_prefix_search(const char *const *terms, size_t n,
                                  const char *prefix, size_t *indices,
                                  size_t *count);
#endif
