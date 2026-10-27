#include "bilinear_resize.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double image[]={0,10,20,30};
    const double expected[]={0,5,10,10,15,20,20,25,30};
    const double point[]={7}, row[]={0,10}, bad[]={0,NAN,20,30};
    const double excessive[]={0,1000001,20,30};
    double output[9]={99,99,99,99,99,99,99,99,99};
    assert(bilinear_resize(image,2,2,3,3,output));
    for (size_t i=0;i<9;++i) assert(fabs(output[i]-expected[i])<1e-12);
    assert(bilinear_resize(image,2,2,2,2,output));
    for (size_t i=0;i<4;++i) assert(output[i]==image[i]);
    assert(bilinear_resize(point,1,1,3,2,output));
    for (size_t i=0;i<6;++i) assert(output[i]==7);
    assert(bilinear_resize(row,1,2,1,3,output));
    assert(output[0]==0 && output[1]==5 && output[2]==10);
    output[0]=99;
    assert(!bilinear_resize(image,0,2,3,3,output));
    assert(!bilinear_resize(image,2,2,0,3,output));
    assert(!bilinear_resize(image,2,2,65,3,output));
    assert(!bilinear_resize(image,65,2,3,3,output));
    assert(!bilinear_resize(bad,2,2,3,3,output));
    assert(!bilinear_resize(excessive,2,2,3,3,output));
    assert(!bilinear_resize(NULL,2,2,3,3,output));
    assert(!bilinear_resize(image,2,2,3,3,NULL));
    assert(output[0]==99);
    puts("C bilinear resize tests passed");
    return 0;
}
