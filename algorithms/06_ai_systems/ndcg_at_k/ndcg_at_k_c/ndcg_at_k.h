#ifndef EDUJBARRIOS_AI_NDCG_AT_K_H
#define EDUJBARRIOS_AI_NDCG_AT_K_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* grades has n items, each in [0,20]. Requires 1 <= k <= n.
   Caller guarantees n readable grades; *out is unchanged on error. */
bool ndcg_at_k(const uint8_t *grades, size_t n, size_t k, double *out);
#endif
