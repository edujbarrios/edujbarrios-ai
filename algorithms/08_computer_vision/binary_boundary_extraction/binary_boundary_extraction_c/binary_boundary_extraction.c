#include "binary_boundary_extraction.h"

bool binary_boundary_extraction(
    const uint8_t *pixels, size_t height, size_t width, uint8_t *output) {
    if (!pixels || !output || pixels==output ||
        height<1 || height>64 || width<1 || width>64) return false;
    const size_t n=height*width;
    for(size_t i=0;i<n;++i) if(pixels[i]>1) return false;
    for(size_t i=0;i<n;++i) {
        if(pixels[i]==0) {output[i]=0;continue;}
        const size_t row=i/width, col=i%width;
        output[i]=(uint8_t)(
            row==0 || row+1==height || col==0 || col+1==width ||
            pixels[i-width]==0 || pixels[i+width]==0 ||
            pixels[i-1]==0 || pixels[i+1]==0
        );
    }
    return true;
}
