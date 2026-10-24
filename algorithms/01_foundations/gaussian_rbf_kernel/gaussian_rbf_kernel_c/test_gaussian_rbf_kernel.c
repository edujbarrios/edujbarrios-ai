#include "gaussian_rbf_kernel.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[]={0,1,2};
    const double duplicates[]={2,2,2};
    const double bad[]={0,NAN,2};
    const double outside[]={0,1000001,2};
    double out[9]={-9,-9,-9,-9,-9,-9,-9,-9,-9};
    const double near=exp(-.5),far=exp(-2.0);
    assert(gaussian_rbf_kernel(x,3,1,1,out));
    assert(out[0]==1 && out[4]==1 && out[8]==1);
    assert(fabs(out[1]-near)<1e-12 && fabs(out[3]-near)<1e-12);
    assert(fabs(out[2]-far)<1e-12 && fabs(out[6]-far)<1e-12);
    assert(fabs(out[5]-near)<1e-12 && fabs(out[7]-near)<1e-12);
    assert(gaussian_rbf_kernel(duplicates,3,1,1,out));
    for (size_t i=0;i<9;++i) assert(out[i]==1);
    assert(gaussian_rbf_kernel(x,1,1,1,out) && out[0]==1);
    assert(gaussian_rbf_kernel(x,3,1,2,out));
    assert(fabs(out[1]-exp(-.125))<1e-12);
    out[0]=-9;
    assert(!gaussian_rbf_kernel(x,3,1,0,out));
    assert(!gaussian_rbf_kernel(x,3,1,1e-7,out));
    assert(!gaussian_rbf_kernel(x,3,1,1e7,out));
    assert(!gaussian_rbf_kernel(bad,3,1,1,out));
    assert(!gaussian_rbf_kernel(outside,3,1,1,out));
    assert(!gaussian_rbf_kernel(x,0,1,1,out));
    assert(!gaussian_rbf_kernel(x,3,0,1,out));
    assert(!gaussian_rbf_kernel(x,3,1,1,NULL));
    assert(out[0]==-9);
    puts("C Gaussian RBF kernel tests passed");
    return 0;
}
