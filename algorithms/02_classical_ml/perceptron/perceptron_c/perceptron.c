#include "perceptron.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool perceptron_predict(const double *weights, const double *features,
                        size_t d, double bias, uint8_t *prediction_out) {
    if (!weights || !features || !prediction_out || d == 0 ||
        !isfinite(bias)) return false;
    double score = 0.0;
    for (size_t j = 0; j < d; ++j) {
        if (!isfinite(weights[j]) || !isfinite(features[j])) return false;
        const double term = weights[j]*features[j];
        if (!isfinite(term)) return false;
        score += term;
        if (!isfinite(score)) return false;
    }
    score += bias;
    if (!isfinite(score)) return false;
    *prediction_out = (uint8_t)(score > 0.0);
    return true;
}

bool perceptron(const double *features, const uint8_t *labels,
                size_t n, size_t d, double learning_rate, size_t epochs,
                double *weights_out, double *bias_out) {
    if (!features || !labels || !weights_out || !bias_out ||
        n == 0 || d == 0 || epochs == 0 ||
        n > SIZE_MAX/d || d > SIZE_MAX/sizeof(double) ||
        !isfinite(learning_rate) || learning_rate <= 0.0) return false;
    for (size_t i = 0; i < n; ++i) {
        if (labels[i] > 1) return false;
        for (size_t j = 0; j < d; ++j)
            if (!isfinite(features[i*d+j])) return false;
    }
    double *weights = calloc(d,sizeof(*weights));
    if (!weights) return false;
    double bias = 0.0;
    for (size_t epoch = 0; epoch < epochs; ++epoch) {
        for (size_t i = 0; i < n; ++i) {
            uint8_t pred;
            if (!perceptron_predict(weights,features+i*d,d,bias,&pred)) {
                free(weights); return false;
            }
            if (pred == labels[i]) continue;
            const double sign = labels[i] == 1 ? 1.0 : -1.0;
            const double delta = learning_rate*sign;
            for (size_t j = 0; j < d; ++j) {
                const double update = delta*features[i*d+j];
                if (!isfinite(update)) {free(weights);return false;}
                weights[j] += update;
                if (!isfinite(weights[j])) {free(weights);return false;}
            }
            bias += delta;
            if (!isfinite(bias)) {free(weights);return false;}
        }
    }
    memcpy(weights_out,weights,d*sizeof(*weights));
    *bias_out = bias;
    free(weights);
    return true;
}
