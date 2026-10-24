#include "average_precision_at_k.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint8_t rel[]={1,0,1,0,1};
    const uint8_t zero[]={0,0};
    const uint8_t perfect[]={1,1,1};
    const uint8_t outside[]={0,0,1};
    const uint8_t invalid[]={0,0,2};
    double result=-9;
    assert(average_precision_at_k(rel,5,2,3,&result));
    assert(fabs(result-.5)<1e-12);
    assert(average_precision_at_k(rel,5,4,3,&result));
    assert(fabs(result-5.0/9.0)<1e-12);
    assert(average_precision_at_k(rel,5,5,3,&result));
    assert(fabs(result-34.0/45.0)<1e-12);
    assert(average_precision_at_k(zero,2,2,0,&result) && result==0);
    assert(average_precision_at_k(perfect,3,3,3,&result) && result==1);
    assert(average_precision_at_k(outside,3,2,1,&result) && result==0);
    result=-9;
    assert(!average_precision_at_k(rel,5,2,2,&result));
    assert(!average_precision_at_k(invalid,3,1,1,&result));
    assert(!average_precision_at_k(rel,5,0,3,&result));
    assert(!average_precision_at_k(rel,5,6,3,&result));
    assert(!average_precision_at_k(rel,0,1,3,&result));
    assert(!average_precision_at_k(NULL,5,2,3,&result));
    assert(!average_precision_at_k(rel,5,2,3,NULL));
    assert(result==-9);
    puts("C AP@K tests passed");
    return 0;
}
