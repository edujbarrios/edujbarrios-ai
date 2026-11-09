#include "binary_perimeter.h"

bool binary_perimeter(
    const uint8_t *pixels, size_t height, size_t width, uint32_t *perimeter_out) {
    if (!pixels || !perimeter_out || height < 1 || height > 64 ||
        width < 1 || width > 64) return false;
    const size_t n = height * width;
    for (size_t i=0;i<n;++i) if (pixels[i] > 1) return false;

    uint32_t total = 0;
    for (size_t i=0;i<n;++i) {
        if (pixels[i] == 0) continue;
        const size_t row=i/width, col=i%width;
        if (row==0 || pixels[i-width]==0) ++total;
        if (row+1==height || pixels[i+width]==0) ++total;
        if (col==0 || pixels[i-1]==0) ++total;
        if (col+1==width || pixels[i+1]==0) ++total;
    }
    *perimeter_out=total;
    return true;
}
