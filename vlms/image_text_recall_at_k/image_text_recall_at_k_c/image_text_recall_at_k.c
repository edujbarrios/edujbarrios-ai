#include "image_text_recall_at_k.h"
#include <math.h>

bool image_text_recall_at_k(const double *images, const double *texts,
                            size_t n, size_t dim, size_t k,
                            double result[2]) {
    if (!images || !texts || !result || n < 2 || n > 64 ||
        dim < 1 || dim > 128 || k < 1 || k > n) return false;
    double visual[64*128], language[64*128];
    for (size_t mode = 0; mode < 2; ++mode) {
        const double *src = mode == 0 ? images : texts;
        double *dst = mode == 0 ? visual : language;
        for (size_t i = 0; i < n; ++i) {
            double norm2 = 0.0;
            for (size_t d = 0; d < dim; ++d) {
                double v = src[i*dim+d];
                if (!isfinite(v) || fabs(v) > 1000.0) return false;
                norm2 += v*v;
            }
            if (!(norm2 > 0.0)) return false;
            double norm = sqrt(norm2);
            for (size_t d = 0; d < dim; ++d)
                dst[i*dim+d] = src[i*dim+d] / norm;
        }
    }
    double sim[64*64];
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) {
            double dot = 0.0;
            for (size_t d = 0; d < dim; ++d)
                dot += visual[i*dim+d] * language[j*dim+d];
            sim[i*n+j] = dot;
        }
    size_t hit_image = 0, hit_text = 0;
    for (size_t i = 0; i < n; ++i) {
        size_t row_rank = 0, col_rank = 0;
        const double correct = sim[i*n+i];
        for (size_t j = 0; j < n; ++j) {
            double row = sim[i*n+j];
            double col = sim[j*n+i];
            if (row > correct || (row == correct && j < i)) ++row_rank;
            if (col > correct || (col == correct && j < i)) ++col_rank;
        }
        if (row_rank < k) ++hit_image;
        if (col_rank < k) ++hit_text;
    }
    result[0] = (double)hit_image / (double)n;
    result[1] = (double)hit_text / (double)n;
    return true;
}
