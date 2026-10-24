#include "hamming_radius_search.h"
#include <stdint.h>
#include <stdlib.h>

#define MAX_SIGNATURE ((UINT32_C(1) << 30) - UINT32_C(1))
struct match { unsigned distance; size_t index; };
static unsigned bitcount(uint32_t bits) {
    unsigned count=0;
    while (bits) {
        count+=(unsigned)(bits & UINT32_C(1));
        bits >>= 1;
    }
    return count;
}
static int compare_matches(const void *a, const void *b) {
    const struct match *x=a,*y=b;
    if (x->distance<y->distance) return -1;
    if (x->distance>y->distance) return 1;
    if (x->index<y->index) return -1;
    if (x->index>y->index) return 1;
    return 0;
}
bool hamming_radius_search(uint32_t query, const uint32_t *signatures,
                           size_t n, unsigned radius, size_t *indices,
                           size_t *out_count) {
    if (!out_count || query>MAX_SIGNATURE || radius>30 ||
        (n>0 && (!signatures || !indices)) ||
        n>SIZE_MAX/sizeof(struct match)) return false;
    for (size_t i=0;i<n;++i) {
        if (signatures[i]>MAX_SIGNATURE) return false;
    }
    if (n==0) {
        *out_count=0;
        return true;
    }
    struct match *matches=malloc(n*sizeof(*matches));
    if (!matches) return false;
    size_t count=0;
    for (size_t i=0;i<n;++i) {
        const unsigned distance=bitcount(query ^ signatures[i]);
        if (distance<=radius) {
            matches[count].distance=distance;
            matches[count].index=i;
            ++count;
        }
    }
    qsort(matches,count,sizeof(*matches),compare_matches);
    for (size_t i=0;i<count;++i) indices[i]=matches[i].index;
    *out_count=count;
    free(matches);
    return true;
}
