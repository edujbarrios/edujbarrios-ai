#include "layer_normalization.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[]={1,2,3};
    const double gamma[]={1,1,1}, beta[]={0,0,0};
    const double scaled[]={2,1,0.5},shift[]={1,2,3};
    const double constant[]={7,7};
    const double ones[]={1,1}, offsets[]={4,5};
    const double single[]={10};
    const double invalid[]={1,NAN};
    const double huge[]={1.7e308,-1.7e308};
    double out[]={-9,-9,-9};
    const double inv=1.0/sqrt(2.0/3.0+1e-5);
    assert(layer_normalization(x,gamma,beta,3,1e-5,out));
    assert(fabs(out[0]+inv)<1e-12 && fabs(out[1])<1e-12 &&
           fabs(out[2]-inv)<1e-12);
    assert(layer_normalization(x,scaled,shift,3,1e-5,out));
    assert(fabs(out[0]-(1-2*inv))<1e-12);
    assert(fabs(out[1]-2)<1e-12);
    assert(fabs(out[2]-(3+0.5*inv))<1e-12);
    assert(layer_normalization(constant,ones,offsets,2,1e-5,out));
    assert(out[0]==4 && out[1]==5);
    assert(layer_normalization(single,ones,offsets,1,1e-5,out));
    assert(out[0]==4);
    out[0]=-9;
    assert(!layer_normalization(x,gamma,beta,3,0.0,out));
    assert(!layer_normalization(x,gamma,beta,0,1e-5,out));
    assert(!layer_normalization(invalid,ones,offsets,2,1e-5,out));
    assert(!layer_normalization(huge,ones,offsets,2,1e-5,out));
    assert(!layer_normalization(x,gamma,beta,3,INFINITY,out));
    assert(out[0]==-9);
    puts("C layer normalization tests passed");
    return 0;
}
