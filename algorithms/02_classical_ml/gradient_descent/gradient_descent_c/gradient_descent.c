#include "gradient_descent.h"
#include <math.h>

bool gradient_descent(const double *x, const double *y, size_t n,
                      double learning_rate, size_t epochs,
                      double *weight_out, double *bias_out) {
    if (!x || !y || !weight_out || !bias_out || weight_out == bias_out ||
        n == 0 || epochs == 0 || !isfinite(learning_rate) ||
        learning_rate <= 0.0) {
        return false;
    }
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(x[i]) || !isfinite(y[i])) return false;
    }
    double weight = 0.0;
    double bias = 0.0;
    for (size_t step = 0; step < epochs; ++step) {
        double grad_weight = 0.0, grad_bias = 0.0;
        for (size_t i = 0; i < n; ++i) {
            const double prediction = weight * x[i] + bias;
            const double error = prediction - y[i];
            const double component = error * x[i];
            if (!isfinite(prediction) || !isfinite(error) ||
                !isfinite(component)) return false;
            grad_weight += component;
            grad_bias += error;
            if (!isfinite(grad_weight) || !isfinite(grad_bias)) return false;
        }
        const double factor = (2.0 * learning_rate) / (double)n;
        if (!isfinite(factor)) return false;
        weight -= factor * grad_weight;
        bias -= factor * grad_bias;
        if (!isfinite(weight) || !isfinite(bias)) return false;
    }
    *weight_out = weight;
    *bias_out = bias;
    return true;
}
