#include "sobel_edges.h"
#include <math.h>

static size_t clamp_index(int x, size_t length) {
    if (x < 0) return 0;
    return (size_t)x >= length ? length - 1 : (size_t)x;
}

bool sobel_edges(const double *pixels, size_t height, size_t width,
                 double *magnitude) {
    if (!pixels || !magnitude || height < 1 || height > 64 ||
        width < 1 || width > 64) return false;
    for (size_t i = 0; i < height * width; ++i)
        if (!isfinite(pixels[i]) || fabs(pixels[i]) > 1000.0)
            return false;
    const int kx[3][3]={{-1,0,1},{-2,0,2},{-1,0,1}};
    const int ky[3][3]={{-1,-2,-1},{0,0,0},{1,2,1}};
    for (size_t row = 0; row < height; ++row) {
        for (size_t col = 0; col < width; ++col) {
            double gx = 0.0, gy = 0.0;
            for (int y = 0; y < 3; ++y)
                for (int x = 0; x < 3; ++x) {
                    size_t r = clamp_index((int)row + y - 1,height);
                    size_t c = clamp_index((int)col + x - 1,width);
                    double v = pixels[r * width + c];
                    gx += (double)kx[y][x] * v;
                    gy += (double)ky[y][x] * v;
                }
            magnitude[row * width + col] = sqrt(gx * gx + gy * gy);
        }
    }
    return true;
}
