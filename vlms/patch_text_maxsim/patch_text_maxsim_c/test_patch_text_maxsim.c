#include "patch_text_maxsim.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double patches[]={1,0,0,1}, tokens[]={1,0,0.6,0.8};
    const double same[]={1,0,1,0}, opposite[]={0,-1};
    const double bad[]={NAN,0,0,1}, huge[]={1001,0,0,1};
    const double zero[]={0,0,0,1};
    const double scaled[]={2,0,0,3};
    size_t matches[2]={99,99};
    double score=99;
    assert(patch_text_maxsim(patches,2,tokens,2,2,matches,&score));
    assert(matches[0]==0 && matches[1]==1);
    assert(fabs(score-0.9)<1e-12);
    assert(patch_text_maxsim(same,2,tokens,1,2,matches,&score));
    assert(matches[0]==0 && fabs(score-1)<1e-12);
    assert(patch_text_maxsim(patches+2,1,opposite,1,2,matches,&score));
    assert(matches[0]==0 && fabs(score+1)<1e-12);
    assert(patch_text_maxsim(scaled,2,tokens,2,2,matches,&score));
    assert(matches[0]==0 && matches[1]==1);
    assert(fabs(score-0.9)<1e-12);
    matches[0]=99;score=99;
    assert(!patch_text_maxsim(patches,0,tokens,2,2,matches,&score));
    assert(!patch_text_maxsim(patches,33,tokens,2,2,matches,&score));
    assert(!patch_text_maxsim(patches,2,tokens,0,2,matches,&score));
    assert(!patch_text_maxsim(patches,2,tokens,2,65,matches,&score));
    assert(!patch_text_maxsim(zero,2,tokens,2,2,matches,&score));
    assert(!patch_text_maxsim(bad,2,tokens,2,2,matches,&score));
    assert(!patch_text_maxsim(huge,2,tokens,2,2,matches,&score));
    assert(!patch_text_maxsim(NULL,2,tokens,2,2,matches,&score));
    assert(!patch_text_maxsim(patches,2,tokens,2,2,NULL,&score));
    assert(!patch_text_maxsim(patches,2,tokens,2,2,matches,NULL));
    assert(matches[0]==99 && score==99);
    puts("C patch-text MaxSim tests passed");
    return 0;
}
