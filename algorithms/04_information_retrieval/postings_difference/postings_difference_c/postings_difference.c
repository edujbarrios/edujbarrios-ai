#include "postings_difference.h"
#include <stdint.h>

static bool valid(const uint32_t *values, size_t n) {
    if (n != 0 && !values) return false;
    for (size_t i = 0; i < n; ++i) {
        if (values[i] > (uint32_t)INT32_MAX ||
            (i > 0 && values[i] <= values[i-1])) return false;
    }
    return true;
}
bool postings_difference(const uint32_t *included, size_t n_included,
                         const uint32_t *excluded, size_t n_excluded,
                         uint32_t *output, size_t *out_count) {
    if (!out_count || (n_included != 0 && !output) ||
        !valid(included,n_included) || !valid(excluded,n_excluded)) return false;

    size_t i = 0,j = 0,count = 0;
    while (i < n_included && j < n_excluded) {
        if (included[i] < excluded[j]) output[count++] = included[i++];
        else if (included[i] > excluded[j]) ++j;
        else {++i;++j;}
    }
    while (i < n_included) output[count++] = included[i++];
    *out_count = count;
    return true;
}
