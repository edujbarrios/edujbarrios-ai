#ifndef EDUJBARRIOS_AI_CHARACTER_BIGRAM_JACCARD_H
#define EDUJBARRIOS_AI_CHARACTER_BIGRAM_JACCARD_H
#include <stdbool.h>
/* Case-sensitive printable ASCII NUL-terminated strings of 0..512 chars.
   On invalid input similarity_out stays unchanged. */
bool character_bigram_jaccard(const char *a, const char *b,
                              double *similarity_out);
#endif
