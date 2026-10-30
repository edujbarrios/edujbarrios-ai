#include "soft_thresholding.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[] = {-3,-0.5,0,0.5,3};
    const double expected[] = {-2,0,0,0,2};
    const double bad[] = {1,NAN};
    double out[] = {7,7,7,7,7};
    assert(soft_thresholding(x,5,1,out));
    for (size_t i=0;i<5;++i) assert(out[i] == expected[i]);
    assert(soft_thresholding(x,5,0,out));
    for (size_t i=0;i<5;++i) assert(out[i] == x[i]);
    double inplace[] = {-3,0.5,3};
    assert(soft_thresholding(inplace,3,1,inplace));
    assert(inplace[0]==-2 && inplace[1]==0 && inplace[2]==2);
    const double huge[] = {1e308,-1e308};
    assert(soft_thresholding(huge,2,1,out));
    assert(out[0]==1e308 && out[1]==-1e308);
    out[0]=7;
    assert(!soft_thresholding(bad,2,1,out) && out[0]==7);
    assert(!soft_thresholding(x,0,1,out) && out[0]==7);
    assert(!soft_thresholding(x,5,-1,out) && out[0]==7);
    assert(!soft_thresholding(x,5,INFINITY,out) && out[0]==7);
    assert(!soft_thresholding(NULL,5,1,out) && out[0]==7);
    assert(!soft_thresholding(x,5,1,NULL));
    puts("C soft thresholding tests passed");
    return 0;
}
