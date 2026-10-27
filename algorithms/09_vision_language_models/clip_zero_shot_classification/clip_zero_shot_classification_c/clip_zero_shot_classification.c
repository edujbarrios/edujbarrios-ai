#include "clip_zero_shot_classification.h"
#include <math.h>

bool clip_zero_shot_classification(const double *image, const double *texts,
                                   size_t classes, size_t dim,
                                   double logit_scale, double *probabilities) {
    if (!image || !texts || !probabilities || classes < 2 || classes > 64 ||
        dim < 1 || dim > 128 || !isfinite(logit_scale) ||
        logit_scale <= 0.0 || logit_scale > 100.0)
        return false;
    double image_squared = 0.0;
    for (size_t d = 0; d < dim; ++d) {
        if (!isfinite(image[d]) || fabs(image[d]) > 1000.0) return false;
        image_squared += image[d] * image[d];
    }
    if (image_squared <= 0.0) return false;
    const double image_norm = sqrt(image_squared);
    double logits[64], maximum = -INFINITY;
    for (size_t k = 0; k < classes; ++k) {
        double text_squared = 0.0, dot = 0.0;
        for (size_t d = 0; d < dim; ++d) {
            const double value = texts[k * dim + d];
            if (!isfinite(value) || fabs(value) > 1000.0) return false;
            text_squared += value * value;
            dot += image[d] * value;
        }
        if (text_squared <= 0.0) return false;
        logits[k] = logit_scale * dot / (image_norm * sqrt(text_squared));
        if (logits[k] > maximum) maximum = logits[k];
    }
    double total = 0.0, unnormalized[64];
    for (size_t k = 0; k < classes; ++k) {
        unnormalized[k] = exp(logits[k] - maximum);
        total += unnormalized[k];
    }
    if (!isfinite(total) || total <= 0.0) return false;
    for (size_t k = 0; k < classes; ++k)
        probabilities[k] = unnormalized[k] / total;
    return true;
}
