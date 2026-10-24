#ifndef EDUJBARRIOS_AI_TOP_P_NUCLEUS_FILTERING_H
#define EDUJBARRIOS_AI_TOP_P_NUCLEUS_FILTERING_H
#include <stdbool.h>
#include <stddef.h>
/* probabilities[n] finite normalized to 1 +/- 1e-9, 1<=n<=4096,
   threshold in (0,1]. Caller owns indices[n] and nonnull count;
   outputs are unchanged on error and may not overlap inputs. */
bool top_p_nucleus_filtering(const double *probabilities,size_t n,
                              double threshold,size_t *indices,size_t *count);
#endif
