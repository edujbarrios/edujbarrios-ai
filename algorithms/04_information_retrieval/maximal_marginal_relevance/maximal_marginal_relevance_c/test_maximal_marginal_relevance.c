#include "maximal_marginal_relevance.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double relevance[]={.9,.8,.7};
    const double similarity[]={1,.9,-.5, .9,1,0, -.5,0,1};
    const double ties[]={1,1,1};
    const double identity[]={1,0,0, 0,1,0, 0,0,1};
    const double bad[]={1,INFINITY,-.5, .9,1,0, -.5,0,1};
    const double out_of_bounds[]={1,2,0, 0,1,0, 0,0,1};
    double invalid_relevance[]={.9,.8,-.1};
    size_t out[]={99,99,99};
    assert(maximal_marginal_relevance(relevance,similarity,3,.5,3,out));
    assert(out[0]==0 && out[1]==2 && out[2]==1);
    assert(maximal_marginal_relevance(relevance,similarity,3,1,3,out));
    assert(out[0]==0 && out[1]==1 && out[2]==2);
    assert(maximal_marginal_relevance(relevance,similarity,3,0,3,out));
    assert(out[0]==0 && out[1]==2 && out[2]==1);
    assert(maximal_marginal_relevance(ties,identity,3,.5,3,out));
    assert(out[0]==0 && out[1]==1 && out[2]==2);
    out[0]=99;
    assert(!maximal_marginal_relevance(relevance,similarity,3,.5,0,out));
    assert(!maximal_marginal_relevance(relevance,similarity,3,.5,4,out));
    assert(!maximal_marginal_relevance(relevance,bad,3,.5,1,out));
    assert(!maximal_marginal_relevance(relevance,out_of_bounds,3,.5,1,out));
    assert(!maximal_marginal_relevance(invalid_relevance,similarity,3,.5,1,out));
    assert(!maximal_marginal_relevance(relevance,similarity,3,NAN,1,out));
    assert(!maximal_marginal_relevance(relevance,similarity,3,.5,1,NULL));
    assert(out[0]==99);
    puts("C MMR tests passed");
    return 0;
}
