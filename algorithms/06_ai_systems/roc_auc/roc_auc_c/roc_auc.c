#include "roc_auc.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct score_label {double score;uint8_t label;};
static int compare_score(const void *pa,const void *pb) {
    const struct score_label *a=pa,*b=pb;
    if (a->score<b->score) return -1;
    if (a->score>b->score) return 1;
    return 0;
}
bool roc_auc(const double *scores,const uint8_t *labels,size_t n,double *auc) {
    if (!scores || !labels || !auc || n<2 ||
        n>SIZE_MAX/sizeof(struct score_label)) return false;
    size_t positives=0;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(scores[i]) || labels[i]>1) return false;
        positives+=(size_t)labels[i];
    }
    if (positives==0 || positives==n) return false;
    struct score_label *pairs=malloc(n*sizeof(*pairs));
    if (!pairs) return false;
    for (size_t i=0;i<n;++i) {
        pairs[i].score=scores[i];
        pairs[i].label=labels[i];
    }
    qsort(pairs,n,sizeof(*pairs),compare_score);
    double positive_ranks=0.0;
    size_t start=0;
    while (start<n) {
        size_t end=start+1,group_positives=pairs[start].label;
        while (end<n && pairs[end].score==pairs[start].score) {
            group_positives+=pairs[end].label;
            ++end;
        }
        const double average_rank=((double)start+1.0+(double)end)/2.0;
        positive_ranks+=(double)group_positives*average_rank;
        start=end;
    }
    const double p=(double)positives,neg=(double)(n-positives);
    const double statistic=positive_ranks-p*(p+1.0)/2.0;
    double result=statistic/(p*neg);
    free(pairs);
    if (!isfinite(result) || result< -1e-12 || result>1.0+1e-12)
        return false;
    if (result<0.0) result=0.0;
    if (result>1.0) result=1.0;
    *auc=result;
    return true;
}
