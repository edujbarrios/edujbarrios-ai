#include "perplexity.h"
#include <math.h>

bool perplexity(const double *log_probabilities,size_t n,double *result) {
    if (!log_probabilities || !result || n==0 || n>4096) return false;
    double mean_nll=0.0;
    for (size_t i=0;i<n;++i) {
        const double value=log_probabilities[i];
        if (!isfinite(value) || value>0.0) return false;
        const double contribution=(-value)/(double)n;
        mean_nll+=contribution;
        if (!isfinite(contribution) || !isfinite(mean_nll)) return false;
    }
    const double output=exp(mean_nll);
    if (!isfinite(output) || output<1.0) return false;
    *result=output;
    return true;
}
