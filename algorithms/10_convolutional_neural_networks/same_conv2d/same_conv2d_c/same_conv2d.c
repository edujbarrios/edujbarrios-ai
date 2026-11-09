#include "same_conv2d.h"
#include <math.h>

bool same_conv2d(const double *image,size_t height,size_t width,
                 const double *kernel,size_t kh,size_t kw,double *output) {
    if (!image || !kernel || !output || image==output || kernel==output ||
        height<1 || height>32 || width<1 || width>32 ||
        kh<1 || kh>5 || (kh%2)==0 || kw<1 || kw>5 || (kw%2)==0)
        return false;
    for(size_t i=0;i<height*width;++i)
        if(!isfinite(image[i]) || fabs(image[i])>1000.0)return false;
    for(size_t i=0;i<kh*kw;++i)
        if(!isfinite(kernel[i]) || fabs(kernel[i])>1000.0)return false;
    for(size_t row=0;row<height;++row) {
        for(size_t col=0;col<width;++col) {
            double sum=0.0;
            for(size_t ky=0;ky<kh;++ky) {
                const int rr=(int)row+(int)ky-(int)(kh/2);
                if(rr<0 || rr>=(int)height)continue;
                for(size_t kx=0;kx<kw;++kx) {
                    const int cc=(int)col+(int)kx-(int)(kw/2);
                    if(cc>=0 && cc<(int)width)
                        sum+=image[(size_t)rr*width+(size_t)cc]*kernel[ky*kw+kx];
                }
            }
            output[row*width+col]=sum;
        }
    }
    return true;
}
