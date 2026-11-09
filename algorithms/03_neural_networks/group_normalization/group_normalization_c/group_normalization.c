#include "group_normalization.h"
#include <math.h>
#include <string.h>

bool group_normalization(const double *input,size_t channels,size_t spatial_size,
                         size_t groups,const double *gamma,const double *beta,
                         double epsilon,double *output) {
    if(!input || !gamma || !beta || !output ||
       input==output || gamma==output || beta==output ||
       channels<1 || channels>32 || spatial_size<1 || spatial_size>64 ||
       groups<1 || groups>channels || channels%groups!=0 ||
       !isfinite(epsilon) || epsilon<=0.0 || epsilon>1.0) return false;
    const size_t size=channels*spatial_size;
    for(size_t i=0;i<size;++i)
        if(!isfinite(input[i]) || fabs(input[i])>1000.0)return false;
    for(size_t c=0;c<channels;++c)
        if(!isfinite(gamma[c]) || fabs(gamma[c])>1000.0 ||
           !isfinite(beta[c]) || fabs(beta[c])>1000.0) return false;
    const size_t group_size=(channels/groups)*spatial_size;
    double scratch[2048];
    for(size_t g=0;g<groups;++g) {
        const size_t start=g*group_size,stop=start+group_size;
        double mean=0.0;
        for(size_t i=start;i<stop;++i)mean+=input[i]/(double)group_size;
        double variance=0.0;
        for(size_t i=start;i<stop;++i) {
            const double diff=input[i]-mean;
            variance+=(diff*diff)/(double)group_size;
        }
        const double denom=sqrt(variance+epsilon);
        if(!isfinite(denom) || denom<=0.0)return false;
        for(size_t i=start;i<stop;++i) {
            const size_t c=i/spatial_size;
            const double value=gamma[c]*((input[i]-mean)/denom)+beta[c];
            if(!isfinite(value))return false;
            scratch[i]=value;
        }
    }
    memcpy(output,scratch,size*sizeof(double));
    return true;
}
