#include "average_pool2d.h"
#include <math.h>
bool average_pool2d(const double *image,size_t height,size_t width,double *output) {
    if(!image || !output || image==output || height<2 || height>32 ||
       width<2 || width>32) return false;
    for(size_t i=0;i<height*width;++i)
        if(!isfinite(image[i]) || fabs(image[i])>1000.0)return false;
    size_t oh=height/2,ow=width/2;
    for(size_t r=0;r<oh;++r)
        for(size_t c=0;c<ow;++c) {
            size_t i=2*r*width+2*c;
            output[r*ow+c]=(image[i]+image[i+1]+image[i+width]+image[i+width+1])/4.0;
        }
    return true;
}
