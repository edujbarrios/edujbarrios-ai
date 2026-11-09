#include "binary_projection_profiles.h"

bool binary_projection_profiles(
    const uint8_t *pixels, size_t height, size_t width, uint16_t *output) {
    if (!pixels || !output || height<1 || height>64 || width<1 || width>64)
        return false;
    const size_t n=height*width;
    for(size_t i=0;i<n;++i) if(pixels[i]>1) return false;
    for(size_t i=0;i<height+width;++i) output[i]=0;
    for(size_t i=0;i<n;++i) {
        if(pixels[i]!=0) {
            ++output[i/width];
            ++output[height+i%width];
        }
    }
    return true;
}
