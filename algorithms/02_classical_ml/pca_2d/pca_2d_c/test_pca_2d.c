#include "pca_2d.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {0,0,2,0,4,0};
    const double y[] = {0,0,0,2};
    const double diagonal[] = {0,0,2,2,4,4};
    const double isotropic[] = {1,0,-1,0,0,1,0,-1};
    const double constant[] = {1,1,1,1};
    const double bad[] = {0,NAN,2,3};
    const double huge[] = {-1e308,0,1e308,0};
    double mean[2]={-9,-9},v[2]={-9,-9},out[4]={-9,-9,-9,-9};
    assert(pca_2d(x,3,mean,v,out));
    assert(mean[0]==2 && mean[1]==0);
    assert(v[0]==1 && v[1]==0);
    assert(out[0]==-2 && out[1]==0 && out[2]==2);
    assert(pca_2d(y,2,mean,v,out));
    assert(mean[0]==0 && mean[1]==1);
    assert(v[0]==0 && v[1]==1);
    assert(out[0]==-1 && out[1]==1);
    assert(pca_2d(diagonal,3,mean,v,out));
    assert(fabs(v[0]-sqrt(0.5))<1e-12 && fabs(v[1]-sqrt(0.5))<1e-12);
    assert(pca_2d(isotropic,4,mean,v,out));
    assert(v[0]==1 && v[1]==0);
    mean[0]=-9;
    assert(!pca_2d(constant,2,mean,v,out));
    assert(!pca_2d(bad,2,mean,v,out));
    assert(!pca_2d(huge,2,mean,v,out));
    assert(!pca_2d(x,1,mean,v,out));
    assert(mean[0]==-9);
    puts("C PCA 2D tests passed");
    return 0;
}
