#include "maximum_inner_product_top_k.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct item {double score;size_t index;};
static int compare_items(const void *left,const void *right) {
    const struct item *a=left,*b=right;
    if (a->score>b->score) return -1;
    if (a->score<b->score) return 1;
    if (a->index<b->index) return -1;
    if (a->index>b->index) return 1;
    return 0;
}
bool maximum_inner_product_top_k(const double *query,size_t d,
    const double *candidates,size_t n,size_t k,size_t *indices) {
    if (!query || !candidates || !indices || d==0 || n==0 ||
        k==0 || k>n || n>SIZE_MAX/d || n>SIZE_MAX/sizeof(struct item))
        return false;
    for (size_t j=0;j<d;++j) if (!isfinite(query[j])) return false;
    for (size_t i=0;i<n*d;++i)
        if (!isfinite(candidates[i])) return false;
    struct item *items=malloc(n*sizeof(*items));
    if (!items) return false;
    for (size_t i=0;i<n;++i) {
        double score=0.0;
        for (size_t j=0;j<d;++j) {
            const double product=query[j]*candidates[i*d+j];
            if (!isfinite(product)) {free(items);return false;}
            score+=product;
            if (!isfinite(score)) {free(items);return false;}
        }
        items[i].score=score;items[i].index=i;
    }
    qsort(items,n,sizeof(*items),compare_items);
    for (size_t i=0;i<k;++i) indices[i]=items[i].index;
    free(items);
    return true;
}
