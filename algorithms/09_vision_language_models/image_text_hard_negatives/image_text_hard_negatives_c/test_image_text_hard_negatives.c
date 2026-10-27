#include "image_text_hard_negatives.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double identity[]={1,0,0,1}, same[]={1,0,1,0,1,0};
    const double triple[]={1,0,0,1,0.6,0.8};
    const double scaled[]={2,0,0,3};
    const double zero[]={1,0,0,0}, nan[]={1,0,NAN,1};
    const double excessive[]={1,0,1001,1};
    size_t out[6]={99,99,99,99,99,99};
    assert(image_text_hard_negatives(identity,identity,2,2,out));
    assert(out[0]==1 && out[1]==0 && out[2]==1 && out[3]==0);
    assert(image_text_hard_negatives(triple,triple,3,2,out));
    assert(out[0]==2 && out[1]==2 && out[2]==1);
    assert(out[3]==2 && out[4]==2 && out[5]==1);
    assert(image_text_hard_negatives(same,same,3,2,out));
    assert(out[0]==1 && out[1]==0 && out[2]==0);
    assert(out[3]==1 && out[4]==0 && out[5]==0);
    assert(image_text_hard_negatives(scaled,identity,2,2,out));
    assert(out[0]==1 && out[1]==0 && out[2]==1 && out[3]==0);
    out[0]=99;
    assert(!image_text_hard_negatives(identity,identity,1,2,out));
    assert(!image_text_hard_negatives(identity,identity,65,2,out));
    assert(!image_text_hard_negatives(identity,identity,2,0,out));
    assert(!image_text_hard_negatives(identity,identity,2,129,out));
    assert(!image_text_hard_negatives(identity,zero,2,2,out));
    assert(!image_text_hard_negatives(identity,nan,2,2,out));
    assert(!image_text_hard_negatives(identity,excessive,2,2,out));
    assert(!image_text_hard_negatives(NULL,identity,2,2,out));
    assert(!image_text_hard_negatives(identity,identity,2,2,NULL));
    assert(out[0]==99);
    puts("C image-text hard negative tests passed");
    return 0;
}
