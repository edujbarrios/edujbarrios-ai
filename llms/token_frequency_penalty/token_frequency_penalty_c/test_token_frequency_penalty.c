#include "token_frequency_penalty.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double scores[]={1,2,3}, negative[]={-1,0,1};
    const size_t ids[]={0,2,2}, repeated[]={0,0}, wrong[]={3};
    const double bad[]={1,NAN,3}, big[]={1,1001,3};
    double out[3]={99,99,99};
    assert(token_frequency_penalty(scores,3,ids,3,0.5,out));
    assert(out[0]==0.5 && out[1]==2.0 && out[2]==2.0);
    assert(token_frequency_penalty(negative,3,repeated,2,1,out));
    assert(out[0]==-3 && out[1]==0 && out[2]==1);
    assert(token_frequency_penalty(scores,3,NULL,0,1,out));
    for(size_t i=0;i<3;++i) assert(out[i]==scores[i]);
    assert(token_frequency_penalty(scores,3,ids,3,0,out));
    for(size_t i=0;i<3;++i) assert(out[i]==scores[i]);
    out[0]=99;
    assert(!token_frequency_penalty(scores,0,ids,3,1,out));
    assert(!token_frequency_penalty(scores,4097,ids,3,1,out));
    assert(!token_frequency_penalty(scores,3,ids,4097,1,out));
    assert(!token_frequency_penalty(scores,3,NULL,1,1,out));
    assert(!token_frequency_penalty(scores,3,wrong,1,1,out));
    assert(!token_frequency_penalty(scores,3,ids,3,-1,out));
    assert(!token_frequency_penalty(scores,3,ids,3,6,out));
    assert(!token_frequency_penalty(scores,3,ids,3,NAN,out));
    assert(!token_frequency_penalty(bad,3,ids,3,1,out));
    assert(!token_frequency_penalty(big,3,ids,3,1,out));
    assert(!token_frequency_penalty(NULL,3,ids,3,1,out));
    assert(!token_frequency_penalty(scores,3,ids,3,1,NULL));
    assert(out[0]==99);
    puts("C frequency penalty tests passed");
    return 0;
}
