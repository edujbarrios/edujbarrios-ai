#include "clip_zero_shot_classification.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double image[]={1,0}, reversed[]={0,1};
    const double prompts[]={1,0,0,1};
    const double duplicates[]={1,0,1,0};
    const double scaled_image[]={2,0}, scaled_prompts[]={3,0,0,5};
    const double zeros[]={0,0,0,1};
    const double invalid[]={1,0,NAN,1};
    const double excessive[]={1,0,1001,1};
    double out[]={99,99};
    const double expected=exp(1)/(exp(1)+1);
    assert(clip_zero_shot_classification(image,prompts,2,2,1,out));
    assert(fabs(out[0]-expected)<1e-12 && fabs(out[1]-(1-expected))<1e-12);
    assert(clip_zero_shot_classification(reversed,prompts,2,2,1,out));
    assert(fabs(out[1]-expected)<1e-12);
    assert(clip_zero_shot_classification(image,duplicates,2,2,1,out));
    assert(out[0]==0.5 && out[1]==0.5);
    assert(clip_zero_shot_classification(scaled_image,scaled_prompts,2,2,1,out));
    assert(fabs(out[0]-expected)<1e-12);
    assert(clip_zero_shot_classification(image,prompts,2,2,100,out));
    assert(out[0]>0.99999);
    out[0]=99;
    assert(!clip_zero_shot_classification(image,prompts,1,2,1,out));
    assert(!clip_zero_shot_classification(image,prompts,2,0,1,out));
    assert(!clip_zero_shot_classification(image,prompts,65,2,1,out));
    assert(!clip_zero_shot_classification(image,prompts,2,129,1,out));
    assert(!clip_zero_shot_classification(image,prompts,2,2,0,out));
    assert(!clip_zero_shot_classification(image,prompts,2,2,NAN,out));
    assert(!clip_zero_shot_classification(image,zeros,2,2,1,out));
    assert(!clip_zero_shot_classification(image,invalid,2,2,1,out));
    assert(!clip_zero_shot_classification(image,excessive,2,2,1,out));
    assert(!clip_zero_shot_classification(NULL,prompts,2,2,1,out));
    assert(!clip_zero_shot_classification(image,prompts,2,2,1,NULL));
    assert(out[0]==99);
    puts("C zero-shot classification tests passed");
    return 0;
}
