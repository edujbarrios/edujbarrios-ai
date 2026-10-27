#include "binary_erosion.h"

bool binary_erosion(const uint8_t *pixels, size_t height,
                    size_t width, uint8_t *output) {
    if (!pixels || !output || height<1 || height>64 ||
        width<1 || width>64) return false;
    for (size_t i=0; i<height*width; ++i)
        if (pixels[i]>1) return false;
    for (size_t r=0; r<height; ++r) {
        for (size_t c=0; c<width; ++c) {
            uint8_t hit=1;
            for (int dr=-1; dr<=1; ++dr) {
                for (int dc=-1; dc<=1; ++dc) {
                    int y=(int)r+dr, x=(int)c+dc;
                    if (y<0 || y>=(int)height || x<0 || x>=(int)width ||
                        !pixels[(size_t)y*width+(size_t)x]) hit=0;
                }
            }
            output[r*width+c]=hit;
        }
    }
    return true;
}
