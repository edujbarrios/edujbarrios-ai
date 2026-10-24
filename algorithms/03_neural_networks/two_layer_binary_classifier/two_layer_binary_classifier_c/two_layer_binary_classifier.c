#include "two_layer_binary_classifier.h"
#include <math.h>

static double stable_sigmoid(double z) {
    if (z >= 0.0) {
        const double e = exp(-z);
        return 1.0 / (1.0 + e);
    }
    const double e = exp(z);
    return e / (1.0 + e);
}
bool two_layer_predict(const double params[4], double x, double *probability) {
    if (!params || !probability || !isfinite(x)) return false;
    for (size_t j = 0; j < 4; ++j) {
        if (!isfinite(params[j])) return false;
    }
    const double pre = params[0]*x + params[1];
    if (!isfinite(pre)) return false;
    const double hidden = fmax(0.0, pre);
    const double logit = params[2]*hidden + params[3];
    if (!isfinite(logit)) return false;
    const double predicted = stable_sigmoid(logit);
    if (!isfinite(predicted)) return false;
    *probability = predicted;
    return true;
}
bool two_layer_binary_classifier(const double *x, const double *labels,
    size_t n, double learning_rate, size_t epochs, double parameters_out[4]) {
    if (!x || !labels || !parameters_out || n == 0 || epochs == 0 ||
        !isfinite(learning_rate) || learning_rate <= 0.0) return false;
    for (size_t i = 0; i < n; ++i) {
        if (!isfinite(x[i]) || !isfinite(labels[i]) ||
            (labels[i] != 0.0 && labels[i] != 1.0)) return false;
    }
    double params[4] = {1.0, 0.5, 1.0, 0.0};
    for (size_t epoch = 0; epoch < epochs; ++epoch) {
        double gradient[4] = {0.0, 0.0, 0.0, 0.0};
        for (size_t i = 0; i < n; ++i) {
            const double pre = params[0]*x[i] + params[1];
            if (!isfinite(pre)) return false;
            const double hidden = fmax(0.0, pre);
            const double logit = params[2]*hidden + params[3];
            if (!isfinite(logit)) return false;
            const double error = stable_sigmoid(logit) - labels[i];
            const double hidden_delta = pre > 0.0 ? error*params[2] : 0.0;
            const double terms[4] = {
                hidden_delta*x[i], hidden_delta, error*hidden, error
            };
            for (size_t j = 0; j < 4; ++j) {
                if (!isfinite(terms[j])) return false;
                gradient[j] += terms[j];
                if (!isfinite(gradient[j])) return false;
            }
        }
        const double factor = learning_rate/(double)n;
        if (!isfinite(factor)) return false;
        for (size_t j = 0; j < 4; ++j) {
            const double update = factor*gradient[j];
            if (!isfinite(update)) return false;
            params[j] -= update;
            if (!isfinite(params[j])) return false;
        }
    }
    for (size_t j = 0; j < 4; ++j) parameters_out[j] = params[j];
    return true;
}
