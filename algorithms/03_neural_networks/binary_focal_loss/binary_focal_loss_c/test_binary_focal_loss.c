#include "binary_focal_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double half[]={.5,.5},perfect[]={0,1},wrong[]={0,1};
    const double invalid[]={1.2,.5},nan[]={NAN,.5};
    const uint8_t labels[]={1,0},perfect_labels[]={0,1};
    const uint8_t wrong_labels[]={1,0},bad_labels[]={1,2};
    double value=-9;
    assert(binary_focal_loss(half,labels,2,&value));
    assert(fabs(value-log(2.0)/8.0)<1e-12);
    assert(binary_focal_loss(perfect,perfect_labels,2,&value));
    assert(value==0.0);
    assert(binary_focal_loss(wrong,wrong_labels,2,&value));
    assert(fabs(value-0.5*log(1e12))<1e-10);
    assert(binary_focal_loss(half,labels,1,&value));
    assert(fabs(value-log(2.0)/16.0)<1e-12);
    value=-9;
    assert(!binary_focal_loss(half,labels,0,&value));
    assert(!binary_focal_loss(invalid,labels,2,&value));
    assert(!binary_focal_loss(nan,labels,2,&value));
    assert(!binary_focal_loss(half,bad_labels,2,&value));
    assert(!binary_focal_loss(half,labels,2,NULL));
    assert(value==-9);
    puts("C binary focal loss tests passed");
    return 0;
}
