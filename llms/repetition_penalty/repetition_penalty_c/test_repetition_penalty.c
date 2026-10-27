#include "repetition_penalty.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double logits[]={2,-2,0,1};
    const size_t ids[]={0,1,1,2}, invalid[]={4};
    const double nan[]={2,NAN,0,1}, big[]={2,-2,1001,1};
    double out[]={99,99,99,99};
    assert(repetition_penalty(logits,4,ids,4,2,out));
    assert(out[0]==1 && out[1]==-4 && out[2]==0 && out[3]==1);
    assert(repetition_penalty(logits,4,ids,4,1,out));
    assert(out[0]==2 && out[1]==-2 && out[2]==0 && out[3]==1);
    assert(repetition_penalty(logits,4,NULL,0,10,out));
    assert(out[0]==2 && out[1]==-2 && out[2]==0 && out[3]==1);
    out[0]=99;
    assert(!repetition_penalty(logits,4,invalid,1,2,out));
    assert(!repetition_penalty(logits,4,NULL,1,2,out));
    assert(!repetition_penalty(logits,0,ids,4,2,out));
    assert(!repetition_penalty(logits,4097,ids,4,2,out));
    assert(!repetition_penalty(logits,4,ids,16385,2,out));
    assert(!repetition_penalty(logits,4,ids,4,0.5,out));
    assert(!repetition_penalty(logits,4,ids,4,11,out));
    assert(!repetition_penalty(logits,4,ids,4,NAN,out));
    assert(!repetition_penalty(nan,4,ids,4,2,out));
    assert(!repetition_penalty(big,4,ids,4,2,out));
    assert(!repetition_penalty(NULL,4,ids,4,2,out));
    assert(!repetition_penalty(logits,4,ids,4,2,NULL));
    assert(out[0]==99);
    puts("C repetition penalty tests passed");
    return 0;
}
