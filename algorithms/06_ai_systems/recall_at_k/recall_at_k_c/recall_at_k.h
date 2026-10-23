#ifndef EDUJBARRIOS_AI_RECALL_AT_K_H
#define EDUJBARRIOS_AI_RECALL_AT_K_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* Array contains n binary labels. All labels, even beyond k, are validated.
   On failure *out stays unchanged; caller guarantees n input elements. */
bool recall_at_k(const uint8_t *relevance, size_t n, size_t k,
                 size_t total_relevant, double *out);
#endif
