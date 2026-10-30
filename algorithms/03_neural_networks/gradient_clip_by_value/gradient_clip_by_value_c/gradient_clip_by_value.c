#include "gradient_clip_by_value.h"
#include <math.h>
bool gradient_clip_by_value(const double *gradients, size_t n, double clip_value, double *output) {
    if (!gradients || !output || n == 0 || !isfinite(clip_value) || clip_value <= 0.0) return false;
    for (size_t i=0;i<n;++i) if (!isfinite(gradients[i])) return false;
    for (size_t i=0;i<n;++i) {
        double x = gradients[i];
        output[i] = x > clip_value ? clip_value : x < -clip_value ? -clip_value : x;
    }
    return true;
}
