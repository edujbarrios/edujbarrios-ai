#ifndef EDUJBARRIOS_AI_MAXIMAL_MARGINAL_RELEVANCE_H
#define EDUJBARRIOS_AI_MAXIMAL_MARGINAL_RELEVANCE_H
#include <stdbool.h>
#include <stddef.h>
/* relevance[n], similarities[n*n] row-major, 0<=lambda<=1 and 1<=k<=n.
   Caller owns distinct, correctly sized input/output arrays; selected[k]
   remains unmodified on failure. */
bool maximal_marginal_relevance(const double *relevance,
    const double *similarities,size_t n,double lambda,size_t k,
    size_t *selected);
#endif
