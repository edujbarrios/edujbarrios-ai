#include "siglip_pairwise_loss.h"
#include <math.h>

static double softplus(double x) {
    return fmax(x, 0.0) + log1p(exp(-fabs(x)));
}

bool siglip_pairwise_loss(const double *images, const double *texts,
                          size_t n, size_t dim, double logit_scale,
                          double bias, double *loss) {
    if (!images || !texts || !loss || n < 2 || n > 64 ||
        dim < 1 || dim > 128 || !isfinite(logit_scale) ||
        logit_scale <= 0.0 || logit_scale > 100.0 ||
        !isfinite(bias) || fabs(bias) > 20.0) return false;
    double visual[64*128], language[64*128];
    for (size_t which = 0; which < 2; ++which) {
        const double *src = which ? texts : images;
        double *dst = which ? language : visual;
        for (size_t i = 0; i < n; ++i) {
            double norm2 = 0.0;
            for (size_t d = 0; d < dim; ++d) {
                const double x = src[i*dim+d];
                if (!isfinite(x) || fabs(x) > 1000.0) return false;
                norm2 += x*x;
            }
            if (norm2 <= 0.0) return false;
            const double norm = sqrt(norm2);
            for (size_t d = 0; d < dim; ++d)
                dst[i*dim+d] = src[i*dim+d]/norm;
        }
    }
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double similarity = 0.0;
            for (size_t d = 0; d < dim; ++d)
                similarity += visual[i*dim+d]*language[j*dim+d];
            const double label = i == j ? 1.0 : -1.0;
            const double x = -label*(logit_scale*similarity+bias);
            total += softplus(x);
        }
    }
    *loss = total/((double)n*(double)n);
    return true;
}
