#include "clip_contrastive_loss.h"
#include <math.h>

bool clip_contrastive_loss(const double *images, const double *texts,
                           size_t n, size_t dim, double logit_scale,
                           double *loss_out) {
    if (!images || !texts || !loss_out || n < 2 || n > 64 ||
        dim < 1 || dim > 128 || !isfinite(logit_scale) ||
        logit_scale <= 0.0 || logit_scale > 100.0) return false;
    double image_norm[64 * 128], text_norm[64 * 128];
    for (size_t k = 0; k < 2; ++k) {
        const double *source = k == 0 ? images : texts;
        double *target = k == 0 ? image_norm : text_norm;
        for (size_t i = 0; i < n; ++i) {
            double norm2 = 0.0;
            for (size_t j = 0; j < dim; ++j) {
                double x = source[i * dim + j];
                if (!isfinite(x) || fabs(x) > 1000.0) return false;
                norm2 += x * x;
            }
            if (norm2 <= 0.0 || !isfinite(norm2)) return false;
            double norm = sqrt(norm2);
            for (size_t j = 0; j < dim; ++j)
                target[i * dim + j] = source[i * dim + j] / norm;
        }
    }
    double logits[64 * 64];
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            double dot = 0.0;
            for (size_t d = 0; d < dim; ++d)
                dot += image_norm[i * dim + d] * text_norm[j * dim + d];
            logits[i * n + j] = logit_scale * dot;
        }
    }
    double total = 0.0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t orientation = 0; orientation < 2; ++orientation) {
            double maximum = -INFINITY;
            for (size_t j = 0; j < n; ++j) {
                double score = orientation == 0 ? logits[i*n+j] : logits[j*n+i];
                if (score > maximum) maximum = score;
            }
            double sumexp = 0.0;
            for (size_t j = 0; j < n; ++j) {
                double score = orientation == 0 ? logits[i*n+j] : logits[j*n+i];
                sumexp += exp(score - maximum);
            }
            total += maximum + log(sumexp) - logits[i*n+i];
        }
    }
    double loss = total / (2.0 * (double)n);
    if (!isfinite(loss)) return false;
    *loss_out = fmax(0.0, loss);
    return true;
}
