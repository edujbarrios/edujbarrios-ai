#include "batch_normalization.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void){
    const double x[]={1,10,3,10};
    const double gamma[]={1,2},beta[]={0,-1};
    const double single[]={7,4};
    const double neg_gamma[]={-1,0};
    const double wrong[]={1,NAN,3,10};
    const double too_high[]={1,1000001,3,10};
    double output[]={-9,-9,-9,-9};
    assert(batch_normalization(x,2,2,gamma,beta,output));
    const double s=1.0/sqrt(1.0+1e-5);
    assert(fabs(output[0]+s)<1e-12);
    assert(output[1]==-1 && fabs(output[2]-s)<1e-12 && output[3]==-1);
    assert(batch_normalization(single,1,2,gamma,beta,output));
    assert(output[0]==0 && output[1]==-1);
    assert(batch_normalization(x,2,2,neg_gamma,beta,output));
    assert(fabs(output[0]-s)<1e-12 && output[1]==-1);
    output[0]=-9;
    assert(!batch_normalization(wrong,2,2,gamma,beta,output));
    assert(!batch_normalization(too_high,2,2,gamma,beta,output));
    assert(!batch_normalization(x,2,0,gamma,beta,output));
    assert(!batch_normalization(x,0,2,gamma,beta,output));
    assert(!batch_normalization(x,2,2,gamma,beta,NULL));
    assert(output[0]==-9);
    puts("C batch normalization tests passed");
    return 0;
}
