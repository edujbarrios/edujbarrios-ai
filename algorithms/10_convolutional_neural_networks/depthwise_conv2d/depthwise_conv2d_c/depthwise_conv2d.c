#include "depthwise_conv2d.h"
#include <math.h>
bool depthwise_conv2d(const double *input, size_t channels,
                      size_t height, size_t width, const double *kernels,
                      size_t kh, size_t kw, double *output) {
    if (!input || !kernels || !output || channels<1 || channels>8 ||
        height<1 || height>16 || width<1 || width>16 ||
        kh<1 || kh>5 || kw<1 || kw>5 || kh%2==0 || kw%2==0) return false;
    for(size_t i=0;i<channels*height*width;++i)
        if(!isfinite(input[i]) || fabs(input[i])>1000.0) return false;
    for(size_t i=0;i<channels*kh*kw;++i)
        if(!isfinite(kernels[i]) || fabs(kernels[i])>1000.0) return false;
    for(size_t c=0;c<channels;++c)
      for(size_t r=0;r<height;++r)
        for(size_t s=0;s<width;++s) {
            double value=0.0;
            for(size_t i=0;i<kh;++i) for(size_t j=0;j<kw;++j) {
                const int y=(int)r+(int)i-(int)(kh/2);
                const int x=(int)s+(int)j-(int)(kw/2);
                if(y>=0 && y<(int)height && x>=0 && x<(int)width)
                    value+=input[c*height*width+(size_t)y*width+(size_t)x]*
                           kernels[c*kh*kw+i*kw+j];
            }
            output[c*height*width+r*width+s]=value;
        }
    return true;
}
