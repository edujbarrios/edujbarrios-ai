#include "rms_normalization.h"
#include <math.h>
bool rms_normalization(const double *x, const double *gain,
                       size_t dim, double epsilon, double *out) {
    if (!x || !gain || !out || dim<1 || dim>1024 ||
        !isfinite(epsilon) || epsilon<1e-8 || epsilon>1.0)
        return false;
    double squares=0.0;
    for (size_t i=0; i<dim; ++i) {
        if (!isfinite(x[i]) || !isfinite(gain[i]) ||
            fabs(x[i])>1000 || fabs(gain[i])>1000) return false;
        squares+=x[i]*x[i];
    }
    double inverse=1.0/sqrt(squares/(double)dim+epsilon);
    for (size_t i=0; i<dim; ++i)
        out[i]=x[i]*inverse*gain[i];
    return true;
}
