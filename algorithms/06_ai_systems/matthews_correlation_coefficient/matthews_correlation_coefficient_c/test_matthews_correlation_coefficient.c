#include "matthews_correlation_coefficient.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const uint8_t y[]={1,1,0,0},correct[]={1,1,0,0};
    const uint8_t wrong[]={0,0,1,1},mixed[]={1,0,1,0};
    const uint8_t all_zero[]={0,0,0,0};
    const uint8_t bad[]={0,0,2,0};
    const uint8_t imbalanced_y[]={1,1,1,0,0};
    const uint8_t imbalanced_p[]={1,1,0,1,0};
    double result=-9;
    assert(matthews_correlation_coefficient(y,correct,4,&result)&&result==1);
    assert(matthews_correlation_coefficient(y,wrong,4,&result)&&result==-1);
    assert(matthews_correlation_coefficient(y,mixed,4,&result)&&result==0);
    assert(matthews_correlation_coefficient(y,all_zero,4,&result)&&result==0);
    assert(matthews_correlation_coefficient(imbalanced_y,imbalanced_p,5,&result));
    assert(fabs(result-(2.0/15.0)/sqrt(36.0/625.0))<1e-12);
    result=-9;
    assert(!matthews_correlation_coefficient(y,correct,0,&result));
    assert(!matthews_correlation_coefficient(y,bad,4,&result));
    assert(!matthews_correlation_coefficient(y,correct,4,NULL));
    assert(result==-9);
    puts("C MCC tests passed");
    return 0;
}
