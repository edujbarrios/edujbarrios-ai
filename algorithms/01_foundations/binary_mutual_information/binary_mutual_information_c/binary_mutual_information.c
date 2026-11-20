#include "binary_mutual_information.h"
#include <math.h>

bool binary_mutual_information(const uint8_t *x, const uint8_t *y,
                               size_t n, double *result) {
    if (!x || !y || !result || n == 0) return false;
    size_t counts[2][2] = {{0, 0}, {0, 0}};
    for (size_t i = 0; i < n; ++i) {
        if (x[i] > 1 || y[i] > 1) return false;
        ++counts[x[i]][y[i]];
    }
    size_t rows[2] = {counts[0][0] + counts[0][1],
                      counts[1][0] + counts[1][1]};
    size_t cols[2] = {counts[0][0] + counts[1][0],
                      counts[0][1] + counts[1][1]};
    const double dn = (double)n;
    double information = 0.0;
    for (size_t a = 0; a < 2; ++a) {
        for (size_t b = 0; b < 2; ++b) {
            if (counts[a][b] != 0) {
                const double p = (double)counts[a][b] / dn;
                const double px = (double)rows[a] / dn;
                const double py = (double)cols[b] / dn;
                information += p * log2(p / (px * py));
            }
        }
    }
    if (!isfinite(information)) return false;
    *result = fmax(0.0, information);
    return true;
}
