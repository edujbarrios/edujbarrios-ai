#include "exact_cosine_top_k.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

struct scored_index {
    double score;
    size_t index;
};
static int compare_desc(const void *pa, const void *pb) {
    const struct scored_index *a = pa;
    const struct scored_index *b = pb;
    if (a->score > b->score) return -1;
    if (a->score < b->score) return 1;
    if (a->index < b->index) return -1;
    if (a->index > b->index) return 1;
    return 0;
}
static bool norm(const double *v, size_t d, double *result) {
    double magnitude = 0.0;
    for (size_t i = 0; i < d; ++i) {
        if (!isfinite(v[i])) return false;
        magnitude = hypot(magnitude, v[i]);
        if (!isfinite(magnitude)) return false;
    }
    if (magnitude <= 0.0) return false;
    *result = magnitude;
    return true;
}
bool exact_cosine_top_k(const double *query, size_t d,
                        const double *candidates, size_t n,
                        size_t k, size_t *indices) {
    if (!query || !candidates || !indices || d == 0 || n == 0 ||
        k == 0 || k > n || n > SIZE_MAX/d ||
        n > SIZE_MAX/sizeof(struct scored_index)) return false;
    double query_norm;
    if (!norm(query,d,&query_norm)) return false;
    struct scored_index *pairs = malloc(n*sizeof(*pairs));
    if (!pairs) return false;
    for (size_t row = 0; row < n; ++row) {
        const double *candidate = candidates + row*d;
        double candidate_norm;
        if (!norm(candidate,d,&candidate_norm)) {
            free(pairs);
            return false;
        }
        double score = 0.0;
        for (size_t i = 0; i < d; ++i) {
            const double term = (query[i]/query_norm) *
                                (candidate[i]/candidate_norm);
            score += term;
            if (!isfinite(term) || !isfinite(score)) {
                free(pairs);
                return false;
            }
        }
        pairs[row].score=fmax(-1.0, fmin(1.0, score));
        pairs[row].index=row;
    }
    qsort(pairs,n,sizeof(*pairs),compare_desc);
    for (size_t i=0;i<k;++i) indices[i]=pairs[i].index;
    free(pairs);
    return true;
}
