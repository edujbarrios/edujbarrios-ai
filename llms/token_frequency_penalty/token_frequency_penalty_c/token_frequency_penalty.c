#include "token_frequency_penalty.h"
#include <math.h>

bool token_frequency_penalty(const double *logits, size_t vocab,
                             const size_t *history, size_t history_len,
                             double penalty, double *output) {
    if (!logits || !output || vocab<1 || vocab>4096 ||
        history_len>4096 || (history_len && !history) ||
        !isfinite(penalty) || penalty<0.0 || penalty>5.0) return false;
    for (size_t i=0; i<vocab; ++i)
        if (!isfinite(logits[i]) || fabs(logits[i])>1000.0)
            return false;
    for (size_t i=0; i<history_len; ++i)
        if (history[i]>=vocab) return false;
    size_t counts[4096]={0};
    for (size_t i=0; i<history_len; ++i) ++counts[history[i]];
    for (size_t i=0; i<vocab; ++i)
        output[i]=logits[i]-penalty*(double)counts[i];
    return true;
}
