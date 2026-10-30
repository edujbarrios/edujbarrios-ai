#include "gradient_clip_by_value.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double gradients[] = {-3,-0.5,0,0.5,3};
    const double expected[] = {-1,-0.5,0,0.5,1};
    double out[] = {9,9,9,9,9};
    assert(gradient_clip_by_value(gradients,5,1,out));
    for (size_t i=0;i<5;++i) assert(out[i]==expected[i]);
    double inplace[] = {-3,0,3};
    assert(gradient_clip_by_value(inplace,3,1,inplace));
    assert(inplace[0]==-1 && inplace[1]==0 && inplace[2]==1);
    const double huge[] = {-1e308,1e308};
    assert(gradient_clip_by_value(huge,2,1,out) && out[0]==-1 && out[1]==1);
    const double nan_values[] = {1,NAN};
    out[0]=9;
    assert(!gradient_clip_by_value(nan_values,2,1,out) && out[0]==9);
    assert(!gradient_clip_by_value(gradients,0,1,out) && out[0]==9);
    assert(!gradient_clip_by_value(gradients,5,0,out) && out[0]==9);
    assert(!gradient_clip_by_value(gradients,5,INFINITY,out) && out[0]==9);
    assert(!gradient_clip_by_value(NULL,5,1,out) && out[0]==9);
    assert(!gradient_clip_by_value(gradients,5,1,NULL));
    puts("C gradient value clipping tests passed");
    return 0;
}
