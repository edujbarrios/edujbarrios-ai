#include "otsu_threshold.h"

bool otsu_threshold(const uint8_t *pixels, size_t length, uint8_t *threshold) {
    if (!pixels || !threshold || length < 1 || length > 4096)
        return false;
    size_t hist[256] = {0};
    size_t sum_all = 0;
    for (size_t i = 0; i < length; ++i) {
        ++hist[pixels[i]];
        sum_all += pixels[i];
    }
    size_t count_low = 0, sum_low = 0;
    double best = -1.0;
    uint8_t choice = 0;
    for (size_t t = 0; t < 255; ++t) {
        count_low += hist[t];
        sum_low += t * hist[t];
        if (count_low == 0 || count_low == length) continue;
        double difference = (double)sum_all * (double)count_low -
                            (double)length * (double)sum_low;
        double score = difference * difference /
            ((double)count_low * (double)(length - count_low));
        if (score > best) {
            best = score;
            choice = (uint8_t)t;
        }
    }
    *threshold = choice;
    return true;
}
