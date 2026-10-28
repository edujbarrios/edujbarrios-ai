#include "cross_modal_attention.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double q[]={1,0}, zero[]={0,0};
    const double keys[]={1,0,0,1}, values[]={10,0,0,20};
    const double bad[]={NAN,0}, big[]={1001,0};
    double weights[2]={99,99}, context[2]={99,99};
    double expected=exp(1/sqrt(2.0))/(1+exp(1/sqrt(2.0)));
    assert(cross_modal_attention(q,keys,values,2,2,weights,context));
    assert(fabs(weights[0]-expected)<1e-12);
    assert(fabs(weights[1]-(1-expected))<1e-12);
    assert(fabs(context[0]-10*expected)<1e-12);
    assert(fabs(context[1]-20*(1-expected))<1e-12);
    assert(cross_modal_attention(zero,keys,values,2,2,weights,context));
    assert(weights[0]==0.5 && weights[1]==0.5);
    assert(context[0]==5 && context[1]==10);
    assert(cross_modal_attention(q,keys,values,1,2,weights,context));
    assert(weights[0]==1 && context[0]==10 && context[1]==0);
    weights[0]=99;context[0]=99;
    assert(!cross_modal_attention(q,keys,values,0,2,weights,context));
    assert(!cross_modal_attention(q,keys,values,65,2,weights,context));
    assert(!cross_modal_attention(q,keys,values,2,0,weights,context));
    assert(!cross_modal_attention(q,keys,values,2,65,weights,context));
    assert(!cross_modal_attention(bad,keys,values,2,2,weights,context));
    assert(!cross_modal_attention(q,big,values,1,2,weights,context));
    assert(!cross_modal_attention(q,keys,NULL,2,2,weights,context));
    assert(!cross_modal_attention(NULL,keys,values,2,2,weights,context));
    assert(!cross_modal_attention(q,keys,values,2,2,NULL,context));
    assert(weights[0]==99 && context[0]==99);
    puts("C cross-modal attention tests passed");
    return 0;
}
