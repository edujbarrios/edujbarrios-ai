#include "lsh_bucket_index.h"
#include <stdint.h>
#include <stdlib.h>

#define MAX_SIGNATURE ((UINT32_C(1) << 30) - 1)
struct entry {uint32_t signature;size_t index;};
struct lsh_bucket_index {struct entry *entries;size_t n;};

static int compare(const void *lhs, const void *rhs) {
    const struct entry *a=lhs,*b=rhs;
    if (a->signature<b->signature) return -1;
    if (a->signature>b->signature) return 1;
    if (a->index<b->index) return -1;
    if (a->index>b->index) return 1;
    return 0;
}
bool lsh_bucket_index_build(const uint32_t *signatures, size_t n,
                            lsh_bucket_index **out_index) {
    if (!signatures || !out_index || n==0 ||
        n>SIZE_MAX/sizeof(struct entry)) return false;
    for (size_t i=0;i<n;++i)
        if (signatures[i]>MAX_SIGNATURE) return false;
    lsh_bucket_index *index=malloc(sizeof(*index));
    if (!index) return false;
    index->entries=malloc(n*sizeof(*index->entries));
    if (!index->entries) {free(index);return false;}
    index->n=n;
    for (size_t i=0;i<n;++i) {
        index->entries[i].signature=signatures[i];
        index->entries[i].index=i;
    }
    qsort(index->entries,n,sizeof(*index->entries),compare);
    *out_index=index;
    return true;
}
bool lsh_bucket_index_query(const lsh_bucket_index *index, uint32_t signature,
                            size_t *output, size_t *out_count) {
    if (!index || !output || !out_count || signature>MAX_SIGNATURE)
        return false;
    size_t lo=0,hi=index->n;
    while (lo<hi) {
        size_t mid=lo+(hi-lo)/2;
        if (index->entries[mid].signature<signature) lo=mid+1;
        else hi=mid;
    }
    size_t n=0;
    for (size_t j=lo;j<index->n &&
         index->entries[j].signature==signature;++j) {
        output[n++]=index->entries[j].index;
    }
    *out_count=n;
    return true;
}
void lsh_bucket_index_free(lsh_bucket_index *index) {
    if (!index) return;
    free(index->entries);
    free(index);
}
