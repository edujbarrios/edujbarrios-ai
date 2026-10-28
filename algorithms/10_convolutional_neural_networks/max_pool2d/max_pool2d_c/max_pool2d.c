#include "max_pool2d.h"
#include <math.h>
bool max_pool2d(const double *input, size_t height, size_t width,
                double *output) {
    if (!input || !output || height<2 || height>32 ||
        width<2 || width>32) return false;
    for (size_t i=0; i<height*width; ++i)
        if (!isfinite(input[i]) || fabs(input[i])>1000) return false;
    size_t ow=width/2;
    for (size_t r=0; r<height/2; ++r) for (size_t c=0; c<ow; ++c) {
        size_t at=(r*2)*width+c*2;
        double top=fmax(input[at],input[at+1]);
        double bot=fmax(input[at+width],input[at+width+1]);
        output[r*ow+c]=fmax(top,bot);
    }
    return true;
}
