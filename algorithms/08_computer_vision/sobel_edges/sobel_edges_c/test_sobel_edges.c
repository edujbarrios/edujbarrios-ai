#include "sobel_edges.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double ramp[]={0,1,2,0,1,2,0,1,2};
    const double vertical[]={0,0,0,1,1,1,2,2,2};
    const double constant[]={5,5,5,5};
    const double single[]={7}, bad[]={0,NAN,2,0,1,2,0,1,2};
    const double excessive[]={0,1001,2,0,1,2,0,1,2};
    double output[9]={99,99,99,99,99,99,99,99,99};
    assert(sobel_edges(ramp,3,3,output));
    for (size_t i=0;i<9;++i) {
        double expected = (i%3==1) ? 8.0 : 4.0;
        assert(fabs(output[i]-expected)<1e-12);
    }
    assert(sobel_edges(vertical,3,3,output));
    for (size_t i=0;i<9;++i) {
        double expected = (i/3==1) ? 8.0 : 4.0;
        assert(fabs(output[i]-expected)<1e-12);
    }
    assert(sobel_edges(constant,2,2,output));
    for (size_t i=0;i<4;++i) assert(output[i]==0.0);
    assert(sobel_edges(single,1,1,output) && output[0]==0.0);
    output[0]=99;
    assert(!sobel_edges(ramp,0,3,output));
    assert(!sobel_edges(ramp,3,0,output));
    assert(!sobel_edges(ramp,65,3,output));
    assert(!sobel_edges(ramp,3,65,output));
    assert(!sobel_edges(bad,3,3,output));
    assert(!sobel_edges(excessive,3,3,output));
    assert(!sobel_edges(NULL,3,3,output));
    assert(!sobel_edges(ramp,3,3,NULL));
    assert(output[0]==99);
    puts("C Sobel edge tests passed");
    return 0;
}
