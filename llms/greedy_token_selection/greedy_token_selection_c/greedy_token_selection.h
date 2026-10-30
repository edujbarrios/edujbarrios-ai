#ifndef GREEDY_TOKEN_SELECTION_H
#define GREEDY_TOKEN_SELECTION_H
#include <stdbool.h>
#include <stddef.h>
/* Return index of largest finite logit; on error *index is unchanged. */
bool greedy_token_selection(const double *logits, size_t n, size_t *index);
#endif
