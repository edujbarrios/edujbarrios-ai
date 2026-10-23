#include "sigmoid.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double x[] = {-1,0,1};
    const double extreme[] = {-1e308,1e308};
    const double bad[] = {1,NAN};
    double out[] = {-9,-9,-9};
    assert(sigmoid(x,3,out));
    assert(fabs(out[0]-0.2689414213699951)<1e-12);
    assert(out[1]==0.5);
    assert(fabs(out[2]-0.7310585786300049)<1e-12);
    assert(sigmoid(extreme,2,out) && out[0]==0.0 && out[1]==1.0);
    out[0]=-9; out[1]=-9;
    assert(!sigmoid(bad,2,out) && out[0]==-9 && out[1]==-9);
    assert(sigmoid(NULL,0,out));
    assert(!sigmoid(NULL,1,out));
    assert(!sigmoid(x,3,NULL));
    double inplace[] = {-1,1};
    assert(sigmoid(inplace,2,inplace));
    assert(fabs(inplace[0]+inplace[1]-1)<1e-12);
    puts("C sigmoid tests passed");
    return 0;
}
