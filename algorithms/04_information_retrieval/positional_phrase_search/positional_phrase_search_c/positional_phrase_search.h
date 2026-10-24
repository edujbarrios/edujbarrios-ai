#ifndef EDUJBARRIOS_AI_POSITIONAL_PHRASE_SEARCH_H
#define EDUJBARRIOS_AI_POSITIONAL_PHRASE_SEARCH_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Sorted (doc_id, position) lists of length at most 4096 each.
   For n>0, corresponding doc and position arrays must be non-NULL.
   The output buffer supports min(left_n,right_n) document IDs.
   On failure results and count stay unmodified; no aliasing. */
bool positional_phrase_search(
    const uint32_t *left_docs,const uint32_t *left_positions,size_t left_n,
    const uint32_t *right_docs,const uint32_t *right_positions,size_t right_n,
    uint32_t *result_docs,size_t *result_count);
#endif
