#include "softmax.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double equal[] = {1,1,1};
    const double large[] = {1000,1001};
    const double nonfinite[] = {1,NAN};
    const double infinity[] = {INFINITY};
    double out[] = {-1,-1,-1};

    assert(softmax(equal,3,out));
    for (size_t i=0;i<3;++i) assert(fabs(out[i]-1.0/3.0)<1e-12);
    assert(softmax(large,2,out));
    assert(out[0]<out[1] && fabs(out[0]+out[1]-1.0)<1e-12);
    out[0]=-1; out[1]=-1;
    assert(!softmax(nonfinite,2,out) && out[0]==-1 && out[1]==-1);
    assert(!softmax(infinity,1,out) && out[0]==-1);
    assert(!softmax(equal,0,out));
    assert(!softmax(NULL,3,out));
    double inplace[] = {0,0};
    assert(softmax(inplace,2,inplace));
    assert(inplace[0]==0.5 && inplace[1]==0.5);
    puts("C softmax tests passed");
    return 0;
}
