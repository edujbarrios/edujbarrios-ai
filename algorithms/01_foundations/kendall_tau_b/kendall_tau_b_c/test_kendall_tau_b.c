#include "kendall_tau_b.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double x[]={1,2,3}, pos[]={2,3,4}, neg[]={4,3,2}, mix[]={1,3,2};
    const double tx[]={1,1,2,3}, ty[]={1,2,2,3};
    const double same[]={7,7,7}, nan[]={1,NAN,3}, huge[]={1,1000001,3};
    const double two[]={1,2};
    double result=99.0;
    assert(kendall_tau_b(x,pos,3,&result) && fabs(result-1)<1e-12);
    assert(kendall_tau_b(x,neg,3,&result) && fabs(result+1)<1e-12);
    assert(kendall_tau_b(x,mix,3,&result) && fabs(result-1.0/3)<1e-12);
    assert(kendall_tau_b(tx,ty,4,&result) && fabs(result-0.8)<1e-12);
    assert(kendall_tau_b(two,two,2,&result) && result==1);
    result=99.0;
    assert(!kendall_tau_b(x,same,3,&result));
    assert(!kendall_tau_b(same,pos,3,&result));
    assert(!kendall_tau_b(x,nan,3,&result));
    assert(!kendall_tau_b(x,huge,3,&result));
    assert(!kendall_tau_b(x,pos,1,&result));
    assert(!kendall_tau_b(x,pos,129,&result));
    assert(!kendall_tau_b(NULL,pos,3,&result));
    assert(!kendall_tau_b(x,pos,3,NULL));
    assert(result==99.0);
    puts("C Kendall tau-b tests passed");
    return 0;
}
