#include "markov_beam_search.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double p[]={.6,.4},a[]={.9,.1,.2,.8};
    const double equal[]={.5,.5},uniform[]={.5,.5,.5,.5};
    const double singleton[]={1},one[]={1};
    const double bad[]={0,1},malformed[]={.8,.8};
    const double invalid[]={.9,NAN,.2,.8};
    uint8_t result[]={9,9,9};
    double score=-9;
    assert(markov_beam_search(p,a,2,3,2,result,&score));
    assert(result[0]==0 && result[1]==0 && result[2]==0);
    assert(fabs(score-log(.486))<1e-12);
    assert(markov_beam_search(equal,uniform,2,3,2,result,&score));
    assert(result[0]==0 && result[1]==0 && result[2]==0);
    assert(markov_beam_search(p,a,2,3,1,result,&score));
    assert(result[0]==0 && result[1]==0 && result[2]==0);
    assert(markov_beam_search(singleton,one,1,3,8,result,&score));
    assert(result[0]==0 && result[1]==0 && result[2]==0 && score==0);
    result[0]=9;score=-9;
    assert(!markov_beam_search(bad,a,2,3,2,result,&score));
    assert(!markov_beam_search(p,malformed,2,3,2,result,&score));
    assert(!markov_beam_search(p,invalid,2,3,2,result,&score));
    assert(!markov_beam_search(p,a,2,0,2,result,&score));
    assert(!markov_beam_search(p,a,2,3,0,result,&score));
    assert(!markov_beam_search(p,a,2,3,2,NULL,&score));
    assert(result[0]==9 && score==-9);
    puts("C Markov beam search tests passed");
    return 0;
}
