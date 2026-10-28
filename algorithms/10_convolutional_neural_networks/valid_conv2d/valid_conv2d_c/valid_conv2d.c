#include "valid_conv2d.h"
#include <math.h>
bool valid_conv2d(const double *image, size_t height, size_t width,
                  const double *kernel, size_t kh, size_t kw, double *out) {
    if (!image || !kernel || !out || height<1 || height>32 ||
        width<1 || width>32 || kh<1 || kh>8 || kw<1 || kw>8 ||
        kh>height || kw>width) return false;
    for (size_t i=0; i<height*width; ++i)
        if (!isfinite(image[i]) || fabs(image[i])>1000) return false;
    for (size_t i=0; i<kh*kw; ++i)
        if (!isfinite(kernel[i]) || fabs(kernel[i])>1000) return false;
    const size_t oh=height-kh+1, ow=width-kw+1;
    for (size_t r=0; r<oh; ++r) for (size_t c=0; c<ow; ++c) {
        double value=0.0;
        for (size_t y=0; y<kh; ++y) for (size_t x=0; x<kw; ++x)
            value += image[(r+y)*width+c+x]*kernel[y*kw+x];
        out[r*ow+c]=value;
    }
    return true;
}
