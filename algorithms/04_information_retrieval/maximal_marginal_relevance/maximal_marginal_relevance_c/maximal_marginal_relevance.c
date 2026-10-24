#include "maximal_marginal_relevance.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool maximal_marginal_relevance(const double *relevance,
    const double *similarities,size_t n,double lambda,size_t k,
    size_t *selected) {
    if (!relevance || !similarities || !selected || n==0 ||
        k==0 || k>n || !isfinite(lambda) || lambda<0 || lambda>1 ||
        n>SIZE_MAX/n || n>SIZE_MAX/sizeof(double) ||
        k>SIZE_MAX/sizeof(size_t)) return false;
    for (size_t i=0;i<n;++i)
        if (!isfinite(relevance[i]) || relevance[i]<0 ||
            relevance[i]>1) return false;
    for (size_t i=0;i<n*n;++i)
        if (!isfinite(similarities[i]) || similarities[i]<-1 ||
            similarities[i]>1) return false;

    double *max_similarity=malloc(n*sizeof(*max_similarity));
    bool *used=calloc(n,sizeof(*used));
    size_t *scratch=malloc(k*sizeof(*scratch));
    if (!max_similarity || !used || !scratch) {
        free(max_similarity);free(used);free(scratch);return false;
    }
    for (size_t i=0;i<n;++i) max_similarity[i]=-INFINITY;
    for (size_t step=0;step<k;++step) {
        bool found=false;
        size_t best_index=0;
        double best_score=-INFINITY;
        for (size_t i=0;i<n;++i) {
            if (used[i]) continue;
            const double query_term=lambda*relevance[i];
            const double novelty_term=step==0?0.0:(1-lambda)*max_similarity[i];
            const double score=query_term-novelty_term;
            if (!isfinite(score)) goto fail;
            if (!found || score>best_score) {
                found=true;best_index=i;best_score=score;
            }
        }
        if (!found) goto fail;
        scratch[step]=best_index;
        used[best_index]=true;
        for (size_t i=0;i<n;++i) {
            if (used[i]) continue;
            const double s=similarities[i*n+best_index];
            if (s>max_similarity[i]) max_similarity[i]=s;
        }
    }
    memcpy(selected,scratch,k*sizeof(*scratch));
    free(max_similarity);free(used);free(scratch);
    return true;
fail:
    free(max_similarity);free(used);free(scratch);
    return false;
}
