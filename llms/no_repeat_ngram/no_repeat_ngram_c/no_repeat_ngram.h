#ifndef EDUJBARRIOS_AI_NO_REPEAT_NGRAM_H
#define EDUJBARRIOS_AI_NO_REPEAT_NGRAM_H
#include <stdbool.h>
#include <stddef.h>
/* True means a token ID must be blocked. History and output do not overlap.
   Valid: length <=16384, 1<=vocab<=4096, 1<=n<=32. On error mask unchanged. */
bool no_repeat_ngram(const size_t *history, size_t length, size_t vocab,
                     size_t n, bool *blocked);
#endif
