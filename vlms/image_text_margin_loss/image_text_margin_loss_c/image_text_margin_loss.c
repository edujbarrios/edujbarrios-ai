#include "image_text_margin_loss.h"
#include <math.h>

bool image_text_margin_loss(const double *images, const double *texts,
                            size_t n, size_t dim, double margin,
                            double *result) {
    if (!images || !texts || !result || n < 2 || n > 64 ||
        dim < 1 || dim > 128 || !isfinite(margin) ||
        margin < 0.0 || margin > 2.0) return false;
    double visual[64*128], language[64*128], sim[64*64];
    for (size_t which = 0; which < 2; ++which) {
        const double *src = which ? texts : images;
        double *dst = which ? language : visual;
        for (size_t i = 0; i < n; ++i) {
            double square = 0.0;
            for (size_t d = 0; d < dim; ++d) {
                const double v = src[i*dim+d];
                if (!isfinite(v) || fabs(v) > 1000.0) return false;
                square += v*v;
            }
            if (square <= 0.0) return false;
            const double norm = sqrt(square);
            for (size_t d = 0; d < dim; ++d)
                dst[i*dim+d] = src[i*dim+d]/norm;
        }
    }
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) {
            double dot = 0.0;
            for (size_t d = 0; d < dim; ++d)
                dot += visual[i*dim+d]*language[j*dim+d];
            sim[i*n+j] = dot;
        }
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double hardest_row = -INFINITY, hardest_col = -INFINITY;
        for (size_t j = 0; j < n; ++j) {
            if (j == i) continue;
            if (sim[i*n+j] > hardest_row) hardest_row = sim[i*n+j];
            if (sim[j*n+i] > hardest_col) hardest_col = sim[j*n+i];
        }
        const double positive = sim[i*n+i];
        total += fmax(0.0, margin-positive+hardest_row);
        total += fmax(0.0, margin-positive+hardest_col);
    }
    *result = total/(2.0*(double)n);
    return true;
}
