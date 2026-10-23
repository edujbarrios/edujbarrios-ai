#ifndef EDUJBARRIOS_AI_POSTINGS_LIST_H
#define EDUJBARRIOS_AI_POSTINGS_LIST_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* tokens contains n_tokens int32 IDs. offsets contains n_documents+1
   positions, beginning at 0, nondecreasing, ending at n_tokens.
   doc_ids and frequencies each have n_documents slots (or may be null
   when n_documents==0). out_count must be non-null. Output arrays
   and *out_count remain unchanged when validation fails. */
bool postings_list(const int32_t *tokens, size_t n_tokens,
                   const size_t *offsets, size_t n_documents,
                   int32_t target, size_t *doc_ids,
                   size_t *frequencies, size_t *out_count);
#endif
