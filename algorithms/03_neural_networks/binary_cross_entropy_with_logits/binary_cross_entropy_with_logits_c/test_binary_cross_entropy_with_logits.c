#include "binary_cross_entropy_with_logits.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double logits[]={0,2,-2}, targets[]={0,1,1};
    const double zero[]={0}, one[]={1}, half[]={0.5};
    const double plus[]={1000000}, minus[]={-1000000};
    const double bad_target[]={-0.1}, high_target[]={1.1};
    const double bad_logit[]={NAN}, inf_logit[]={INFINITY};
    const double too_high[]={1000001};
    double out=99;
    double expected=(log(2.0)+2*log1p(exp(-2.0))+2.0)/3.0;
    assert(binary_cross_entropy_with_logits(logits,targets,3,&out));
    assert(fabs(out-expected)<1e-12);
    assert(binary_cross_entropy_with_logits(zero,half,1,&out));
    assert(fabs(out-log(2.0))<1e-12);
    assert(binary_cross_entropy_with_logits(plus,one,1,&out));
    assert(out<1e-12);
    assert(binary_cross_entropy_with_logits(minus,one,1,&out));
    assert(fabs(out-1000000.0)<1e-9);
    out=99;
    assert(!binary_cross_entropy_with_logits(zero,bad_target,1,&out));
    assert(!binary_cross_entropy_with_logits(zero,high_target,1,&out));
    assert(!binary_cross_entropy_with_logits(bad_logit,one,1,&out));
    assert(!binary_cross_entropy_with_logits(inf_logit,one,1,&out));
    assert(!binary_cross_entropy_with_logits(too_high,one,1,&out));
    assert(!binary_cross_entropy_with_logits(zero,one,0,&out));
    assert(!binary_cross_entropy_with_logits(zero,one,10001,&out));
    assert(!binary_cross_entropy_with_logits(NULL,one,1,&out));
    assert(!binary_cross_entropy_with_logits(zero,one,1,NULL));
    assert(out==99);
    puts("C BCE with logits tests passed");
    return 0;
}
