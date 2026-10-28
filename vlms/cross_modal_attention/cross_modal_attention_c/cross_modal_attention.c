#include "cross_modal_attention.h"
#include <math.h>

bool cross_modal_attention(const double *query, const double *keys,
                           const double *values, size_t patches, size_t dim,
                           double *weights, double *context) {
    if (!query || !keys || !values || !weights || !context ||
        patches<1 || patches>64 || dim<1 || dim>64) return false;
    for (size_t d=0; d<dim; ++d)
        if (!isfinite(query[d]) || fabs(query[d])>1000) return false;
    for (size_t i=0; i<patches*dim; ++i)
        if (!isfinite(keys[i]) || fabs(keys[i])>1000 ||
            !isfinite(values[i]) || fabs(values[i])>1000) return false;
    double scores[64], probs[64], ctx[64]={0};
    double maximum=-INFINITY;
    for (size_t p=0; p<patches; ++p) {
        double dot=0;
        for (size_t d=0; d<dim; ++d)
            dot+=query[d]*keys[p*dim+d];
        scores[p]=dot/sqrt((double)dim);
        if (scores[p]>maximum) maximum=scores[p];
    }
    double total=0;
    for (size_t p=0; p<patches; ++p) {
        probs[p]=exp(scores[p]-maximum);
        total+=probs[p];
    }
    for (size_t p=0; p<patches; ++p) {
        probs[p]/=total;
        for (size_t d=0; d<dim; ++d)
            ctx[d]+=probs[p]*values[p*dim+d];
    }
    for (size_t p=0; p<patches; ++p) weights[p]=probs[p];
    for (size_t d=0; d<dim; ++d) context[d]=ctx[d];
    return true;
}
