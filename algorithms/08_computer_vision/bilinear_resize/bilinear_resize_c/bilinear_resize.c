#include "bilinear_resize.h"
#include <math.h>

static double clamp_coord(double x, size_t size) {
    if (x < 0.0) return 0.0;
    const double last = (double)(size - 1);
    return x > last ? last : x;
}

bool bilinear_resize(const double *pixels, size_t h, size_t w,
                     size_t out_h, size_t out_w, double *out) {
    if (!pixels || !out || h < 1 || h > 64 || w < 1 || w > 64 ||
        out_h < 1 || out_h > 64 || out_w < 1 || out_w > 64)
        return false;
    for (size_t i = 0; i < h * w; ++i)
        if (!isfinite(pixels[i]) || fabs(pixels[i]) > 1000000.0)
            return false;
    for (size_t r = 0; r < out_h; ++r) {
        const double y = clamp_coord(((double)r + 0.5) *
                                    (double)h / (double)out_h - 0.5, h);
        const size_t y0 = (size_t)floor(y);
        const size_t y1 = y0 + 1 < h ? y0 + 1 : y0;
        const double wy = y - (double)y0;
        for (size_t c = 0; c < out_w; ++c) {
            const double x = clamp_coord(((double)c + 0.5) *
                                        (double)w / (double)out_w - 0.5, w);
            const size_t x0 = (size_t)floor(x);
            const size_t x1 = x0 + 1 < w ? x0 + 1 : x0;
            const double wx = x - (double)x0;
            const double a = (1.0 - wx) * pixels[y0 * w + x0] +
                             wx * pixels[y0 * w + x1];
            const double b = (1.0 - wx) * pixels[y1 * w + x0] +
                             wx * pixels[y1 * w + x1];
            out[r * out_w + c] = (1.0 - wy) * a + wy * b;
        }
    }
    return true;
}
