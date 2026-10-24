#include "rocchio_relevance_feedback.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

int main(void) {
    const double query[]={1,1};
    const double rel[]={2,0,4,2};
    const double irr[]={0,2,2,2};
    const double zero[]={0,0};
    const double one[]={1,1};
    const double bad[]={NAN,0};
    const double negative[]={-1,1};
    double output[]={-9,-9};
    assert(rocchio_relevance_feedback(query,2,rel,2,irr,2,output));
    assert(fabs(output[0]-3.1)<1e-12 && fabs(output[1]-1.45)<1e-12);
    assert(rocchio_relevance_feedback(query,2,rel,2,NULL,0,output));
    assert(fabs(output[0]-3.25)<1e-12 && fabs(output[1]-1.75)<1e-12);
    assert(rocchio_relevance_feedback(zero,2,NULL,0,one,1,output));
    assert(output[0]==0 && output[1]==0);
    output[0]=-9;
    assert(!rocchio_relevance_feedback(query,2,NULL,0,NULL,0,output));
    assert(!rocchio_relevance_feedback(query,2,rel,2,NULL,1,output));
    assert(!rocchio_relevance_feedback(query,2,bad,1,NULL,0,output));
    assert(!rocchio_relevance_feedback(query,2,negative,1,NULL,0,output));
    assert(!rocchio_relevance_feedback(query,0,rel,2,irr,2,output));
    assert(!rocchio_relevance_feedback(query,2,rel,2,irr,2,NULL));
    assert(output[0]==-9);
    puts("C Rocchio tests passed");
    return 0;
}
