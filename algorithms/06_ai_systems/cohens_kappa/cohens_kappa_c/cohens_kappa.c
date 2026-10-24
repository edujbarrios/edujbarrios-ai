#include "cohens_kappa.h"
#include <math.h>

bool cohens_kappa(const uint8_t *first,const uint8_t *second,
                  size_t n,double *result) {
    if (!first || !second || !result || n==0) return false;
    double zero_a=0.0,zero_b=0.0,matches=0.0;
    for (size_t i=0;i<n;++i) {
        if (first[i]>1 || second[i]>1) return false;
        zero_a+=(first[i]==0);
        zero_b+=(second[i]==0);
        matches+=(first[i]==second[i]);
    }
    const double na=(double)n;
    const double pa=zero_a/na,pb=zero_b/na;
    const double observed=matches/na;
    const double expected=pa*pb+(1.0-pa)*(1.0-pb);
    const double denominator=1.0-expected;
    if (!isfinite(expected) || !isfinite(observed) || !(denominator>0.0))
        return false;
    double score=(observed-expected)/denominator;
    if (!isfinite(score) || score< -1.0-1e-12 || score>1.0+1e-12)
        return false;
    if (score< -1.0)score=-1.0;
    if (score>1.0)score=1.0;
    *result=score;
    return true;
}
