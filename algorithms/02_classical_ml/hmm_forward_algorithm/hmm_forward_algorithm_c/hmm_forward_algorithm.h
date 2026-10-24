#ifndef EDUJBARRIOS_AI_HMM_FORWARD_ALGORITHM_H
#define EDUJBARRIOS_AI_HMM_FORWARD_ALGORITHM_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
/* obs[T] binary, 1<=T<=4096, initial[2], A[4], B[4] strictly
   positive row-normalized finite probabilities. A/B are row-major.
   Caller-owned output log_likelihood is unchanged on failure and
   disjoint from all inputs. */
bool hmm_forward_algorithm(const uint8_t *observations,size_t length,
    const double *initial,const double *transition,const double *emission,
    double *log_likelihood);
#endif
