#include "top_p_nucleus_filtering.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
struct probability_index {double probability;size_t index;};
static int compare_probability(const void *pa,const void *pb) {
    const struct probability_index *a=pa,*b=pb;
    if (a->probability>b->probability) return -1;
    if (a->probability<b->probability) return 1;
    if (a->index<b->index) return -1;
    if (a->index>b->index) return 1;
    return 0;
}
bool top_p_nucleus_filtering(const double *probabilities,size_t n,
                              double threshold,size_t *indices,size_t *count) {
    if (!probabilities || !indices || !count || n==0 || n>4096 ||
        !isfinite(threshold) || threshold<=0 || threshold>1)
        return false;
    double total=0.0;
    for (size_t i=0;i<n;++i) {
        if (!isfinite(probabilities[i]) || probabilities[i]<0 ||
            probabilities[i]>1) return false;
        total+=probabilities[i];
        if (!isfinite(total)) return false;
    }
    if (fabs(total-1.0)>1e-9) return false;
    struct probability_index *sorted=malloc(n*sizeof(*sorted));
    if (!sorted) return false;
    for (size_t i=0;i<n;++i) {
        sorted[i].probability=probabilities[i];
        sorted[i].index=i;
    }
    qsort(sorted,n,sizeof(*sorted),compare_probability);
    double cumulative=0.0;
    size_t selected=0;
    for (size_t i=0;i<n;++i) {
        cumulative+=sorted[i].probability;
        ++selected;
        if (cumulative>=threshold) break;
    }
    for (size_t i=0;i<selected;++i) indices[i]=sorted[i].index;
    *count=selected;
    free(sorted);
    return true;
}
