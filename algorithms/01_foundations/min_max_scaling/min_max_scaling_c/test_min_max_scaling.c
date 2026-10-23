#include "min_max_scaling.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double data[] = {-5.0,0.0,5.0};
    const double constant[] = {7.0,7.0,7.0};
    const double invalid[] = {1.0,NAN};
    const double extreme[] = {-1e308,1e308};
    const double one[] = {1.0};
    double out[] = {-9.0,-9.0,-9.0};
    assert(min_max_scaling(data,3,out));
    assert(out[0]==0.0 && out[1]==0.5 && out[2]==1.0);
    double inplace[] = {10.0,20.0,30.0};
    assert(min_max_scaling(inplace,3,inplace));
    assert(inplace[0]==0.0 && inplace[1]==0.5 && inplace[2]==1.0);

    out[0]=-9.0;
    assert(!min_max_scaling(constant,3,out));
    assert(!min_max_scaling(invalid,2,out));
    assert(!min_max_scaling(extreme,2,out));
    assert(!min_max_scaling(one,1,out));
    assert(!min_max_scaling(data,0,out));
    assert(!min_max_scaling(data,3,NULL));
    assert(out[0]==-9.0);
    puts("C min-max scaling tests passed");
    return 0;
}
