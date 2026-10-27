#include "siglip_pairwise_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double aligned[]={1,0,0,1}, swapped[]={0,1,1,0};
    const double scaled[]={3,0,0,2}, zero[]={0,0,0,1};
    const double nan[]={1,0,NAN,1}, big[]={1,0,1001,1};
    double result=99;
    double expected=(log1p(exp(-1.0))+log(2.0))/2.0;
    assert(siglip_pairwise_loss(aligned,aligned,2,2,1,0,&result));
    assert(fabs(result-expected)<1e-12);
    assert(siglip_pairwise_loss(scaled,aligned,2,2,1,0,&result));
    assert(fabs(result-expected)<1e-12);
    assert(siglip_pairwise_loss(aligned,swapped,2,2,1,0,&result));
    assert(result>expected);
    assert(siglip_pairwise_loss(aligned,aligned,2,2,100,-10,&result));
    assert(isfinite(result) && result>=0);
    result=99;
    assert(!siglip_pairwise_loss(aligned,aligned,1,2,1,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,65,2,1,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,0,1,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,129,1,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,2,0,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,2,101,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,2,NAN,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,2,1,21,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,2,1,NAN,&result));
    assert(!siglip_pairwise_loss(aligned,zero,2,2,1,0,&result));
    assert(!siglip_pairwise_loss(aligned,nan,2,2,1,0,&result));
    assert(!siglip_pairwise_loss(aligned,big,2,2,1,0,&result));
    assert(!siglip_pairwise_loss(NULL,aligned,2,2,1,0,&result));
    assert(!siglip_pairwise_loss(aligned,aligned,2,2,1,0,NULL));
    assert(result==99);
    puts("C SigLIP pairwise loss tests passed");
    return 0;
}
