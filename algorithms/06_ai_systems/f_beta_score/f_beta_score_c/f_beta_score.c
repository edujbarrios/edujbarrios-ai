#include "f_beta_score.h"
#include <math.h>

bool f_beta_score(int32_t tp, int32_t fp, int32_t fn, double beta, double *result) {
    if (!result || tp < 0 || fp < 0 || fn < 0 ||
        !isfinite(beta) || beta <= 0.0) return false;
    if (tp == 0) {
        *result = 0.0;
        return true;
    }
    const double precision = (double)tp / ((double)tp + (double)fp);
    const double recall = (double)tp / ((double)tp + (double)fn);
    double weight;
    if (beta <= 1.0) {
        const double b2 = beta * beta;
        weight = b2 / (1.0 + b2);
    } else {
        const double inverse = 1.0 / beta;
        weight = 1.0 / (1.0 + inverse * inverse);
    }
    *result = precision * recall /
        (weight * precision + (1.0 - weight) * recall);
    return true;
}
