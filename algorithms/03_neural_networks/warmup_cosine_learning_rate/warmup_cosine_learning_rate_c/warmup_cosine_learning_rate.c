#include "warmup_cosine_learning_rate.h"
#include <math.h>
bool warmup_cosine_learning_rate(double peak_rate, double minimum_rate,
                                 size_t warmup_steps, size_t total_steps,
                                 size_t step, double *output) {
    if (!output || !isfinite(peak_rate) || !isfinite(minimum_rate) ||
        minimum_rate < 0 || minimum_rate > peak_rate || peak_rate > 1000000 ||
        warmup_steps == 0 || total_steps > 100000 || warmup_steps >= total_steps ||
        step > total_steps) return false;
    double result;
    if (step <= warmup_steps) {
        result = peak_rate * ((double)step / (double)warmup_steps);
    } else if (step == total_steps) {
        result = minimum_rate;
    } else {
        const double pi = 3.14159265358979323846264338327950288;
        double progress = (double)(step - warmup_steps) / (double)(total_steps - warmup_steps);
        result = minimum_rate + (peak_rate - minimum_rate) * 0.5 * (1.0 + cos(pi * progress));
    }
    *output = result;
    return true;
}
