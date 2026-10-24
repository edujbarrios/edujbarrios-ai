#include "positional_encoding.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    double values[8];
    assert(positional_encoding(2,4,values));
    assert(values[0]==0 && values[1]==1 &&
           values[2]==0 && values[3]==1);
    assert(fabs(values[4]-sin(1.0))<1e-12);
    assert(fabs(values[5]-cos(1.0))<1e-12);
    assert(fabs(values[6]-sin(0.01))<1e-12);
    assert(fabs(values[7]-cos(0.01))<1e-12);
    double odd[6];
    assert(positional_encoding(2,3,odd));
    assert(odd[0]==0 && odd[1]==1 && odd[2]==0);
    assert(fabs(odd[5]-sin(1.0/pow(10000.0,2.0/3.0)))<1e-12);
    double single[1]={-9};
    assert(positional_encoding(1,1,single) && single[0]==0);
    single[0]=-9;
    assert(!positional_encoding(0,1,single));
    assert(!positional_encoding(1,0,single));
    assert(!positional_encoding(4097,1,single));
    assert(!positional_encoding(1,1025,single));
    assert(!positional_encoding(1,1,NULL));
    assert(single[0]==-9);
    puts("C positional encoding tests passed");
    return 0;
}
