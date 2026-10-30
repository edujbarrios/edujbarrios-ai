#include "token_presence_penalty.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double logits[] = {1,2,3};
    const double negatives[] = {-1,0,1};
    const size_t history[] = {0,2,2};
    const size_t invalid[] = {3};
    double out[] = {-1,-1,-1};
    assert(token_presence_penalty(logits,3,history,3,0.5,out));
    assert(out[0]==0.5 && out[1]==2 && out[2]==2.5);
    assert(token_presence_penalty(negatives,3,history,3,1,out));
    assert(out[0]==-2 && out[1]==0 && out[2]==0);
    assert(token_presence_penalty(logits,3,NULL,0,2,out));
    assert(out[0]==1 && out[1]==2 && out[2]==3);
    assert(token_presence_penalty(logits,3,history,3,0,out));
    assert(out[0]==1 && out[1]==2 && out[2]==3);
    double inplace[] = {1,2,3};
    assert(token_presence_penalty(inplace,3,history,3,0.5,inplace));
    assert(inplace[0]==0.5 && inplace[2]==2.5);
    out[0]=-1;
    assert(!token_presence_penalty(logits,3,invalid,1,0.5,out) && out[0]==-1);
    assert(!token_presence_penalty(logits,3,NULL,1,0.5,out) && out[0]==-1);
    assert(!token_presence_penalty(logits,0,NULL,0,0.5,out));
    assert(!token_presence_penalty(logits,3,NULL,0,NAN,out));
    assert(!token_presence_penalty(logits,3,NULL,0,-1,out));
    assert(!token_presence_penalty(logits,3,NULL,0,0.5,NULL));
    puts("C presence penalty tests passed");
    return 0;
}
