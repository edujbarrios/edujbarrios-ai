#include "postings_union.h"
#include <stdint.h>

static bool valid(const uint32_t *values,size_t n) {
    if (n!=0 && !values) return false;
    for (size_t i=0;i<n;++i) {
        if (values[i]>(uint32_t)INT32_MAX ||
            (i>0 && values[i]<=values[i-1])) return false;
    }
    return true;
}
bool postings_union(const uint32_t *left, size_t n_left,
                    const uint32_t *right, size_t n_right,
                    uint32_t *output, size_t *out_count) {
    if (!out_count || n_right>SIZE_MAX-n_left ||
        ((n_left+n_right)>0 && !output) ||
        !valid(left,n_left) || !valid(right,n_right)) return false;
    size_t i=0,j=0,count=0;
    while (i<n_left && j<n_right) {
        if (left[i]<right[j]) output[count++]=left[i++];
        else if (left[i]>right[j]) output[count++]=right[j++];
        else {
            output[count++]=left[i];
            ++i;
            ++j;
        }
    }
    while (i<n_left) output[count++]=left[i++];
    while (j<n_right) output[count++]=right[j++];
    *out_count=count;
    return true;
}
