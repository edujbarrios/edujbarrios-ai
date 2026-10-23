#include "scalar_quantization.h"
#include <math.h>

bool scalar_quantization(const double *input, size_t n,
                         double minimum, double maximum, uint8_t *output) {
    if (!output || (n > 0 && !input) || !isfinite(minimum) ||
        !isfinite(maximum) || !(minimum < maximum)) return false;
    const double span = maximum - minimum;
    if (!isfinite(span) || span <= 0.0) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(input[i])) return false;
    }
    for (size_t i = 0; i < n; ++i) {
        if (input[i] <= minimum) output[i] = 0;
        else if (input[i] >= maximum) output[i] = UINT8_MAX;
        else {
            const double scaled = 255.0 * ((input[i] - minimum) / span);
            const double rounded = floor(scaled + 0.5);
            output[i] = (uint8_t)fmin(255.0, rounded);
        }
    }
    return true;
}
