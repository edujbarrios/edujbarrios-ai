#include "global_average_pool2d.h"
#include <math.h>
bool global_average_pool2d(const double *input, size_t channels,
                            size_t height, size_t width, double *output) {
    if (!input || !output || channels<1 || channels>16 ||
        height<1 || height>32 || width<1 || width>32) return false;
    const size_t spatial=height*width;
    for (size_t i=0; i<channels*spatial; ++i)
        if (!isfinite(input[i]) || fabs(input[i])>1000.0) return false;
    for (size_t c=0; c<channels; ++c) {
        double total=0.0;
        for (size_t i=0; i<spatial; ++i)
            total+=input[c*spatial+i];
        output[c]=total/(double)spatial;
    }
    return true;
}
