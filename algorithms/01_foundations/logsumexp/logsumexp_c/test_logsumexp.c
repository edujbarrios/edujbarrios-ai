#include "logsumexp.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double equal[]={0,0};
    const double unequal[]={0.6931471805599453,1.0986122886681098};
    const double small[]={-1000,-1000};
    const double single[]={5};
    const double invalid[]={NAN,0};
    const double extremes[]={-1e308,1e308};
    double value=-9,grad[]={-9,-9};
    assert(logsumexp(equal,2,&value,grad));
    assert(fabs(value-log(2))<1e-12 && grad[0]==.5 && grad[1]==.5);
    assert(logsumexp(unequal,2,&value,grad));
    assert(fabs(value-log(5))<1e-12);
    assert(fabs(grad[0]-.4)<1e-12 && fabs(grad[1]-.6)<1e-12);
    assert(logsumexp(small,2,&value,grad));
    assert(fabs(value-(-1000+log(2)))<1e-12 && grad[0]==.5);
    assert(logsumexp(single,1,&value,grad));
    assert(value==5 && grad[0]==1);
    value=-9;grad[0]=-9;
    assert(!logsumexp(invalid,2,&value,grad));
    assert(!logsumexp(extremes,2,&value,grad));
    assert(!logsumexp(equal,0,&value,grad));
    assert(!logsumexp(equal,2,NULL,grad));
    assert(!logsumexp(equal,2,&value,NULL));
    assert(value==-9 && grad[0]==-9);
    puts("C log-sum-exp tests passed");
    return 0;
}
