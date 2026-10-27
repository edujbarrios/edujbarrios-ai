#include "image_text_hard_negatives.h"
#include <math.h>

bool image_text_hard_negatives(const double *images, const double *texts,
                               size_t n, size_t dim, size_t *indices) {
    if (!images || !texts || !indices || n < 2 || n > 64 ||
        dim < 1 || dim > 128) return false;
    double visual[64*128], language[64*128], similarities[64*64];
    for (size_t which = 0; which < 2; ++which) {
        const double *src = which == 0 ? images : texts;
        double *dst = which == 0 ? visual : language;
        for (size_t i = 0; i < n; ++i) {
            double norm2 = 0.0;
            for (size_t j = 0; j < dim; ++j) {
                double x = src[i*dim+j];
                if (!isfinite(x) || fabs(x) > 1000.0) return false;
                norm2 += x*x;
            }
            if (!(norm2 > 0.0)) return false;
            double norm = sqrt(norm2);
            for (size_t j = 0; j < dim; ++j)
                dst[i*dim+j] = src[i*dim+j]/norm;
        }
    }
    for (size_t i = 0; i < n; ++i)
        for (size_t j = 0; j < n; ++j) {
            double dot = 0.0;
            for (size_t d = 0; d < dim; ++d)
                dot += visual[i*dim+d] * language[j*dim+d];
            similarities[i*n+j] = dot;
        }
    size_t selected[128];
    for (size_t i = 0; i < n; ++i) {
        size_t image_best = n, text_best = n;
        double image_score = -INFINITY, text_score = -INFINITY;
        for (size_t j = 0; j < n; ++j) {
            if (j == i) continue;
            double from_image = similarities[i*n+j];
            double from_text = similarities[j*n+i];
            if (image_best == n || from_image > image_score) {
                image_best = j;
                image_score = from_image;
            }
            if (text_best == n || from_text > text_score) {
                text_best = j;
                text_score = from_text;
            }
        }
        selected[i] = image_best;
        selected[n+i] = text_best;
    }
    for (size_t i = 0; i < 2*n; ++i) indices[i] = selected[i];
    return true;
}
