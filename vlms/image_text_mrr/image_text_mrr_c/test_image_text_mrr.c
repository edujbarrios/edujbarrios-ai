#include "image_text_mrr.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double id[]={1,0,0,1}, swapped[]={0,1,1,0};
    const double tied[]={1,0,1,0}, scaled[]={2,0,0,3};
    const double zero[]={0,0,0,1}, bad[]={1,NAN,0,1};
    const double big[]={1,1001,0,1};
    double out[2]={99,99};
    assert(image_text_mrr(id,id,2,2,out));
    assert(fabs(out[0]-1)<1e-12 && fabs(out[1]-1)<1e-12);
    assert(image_text_mrr(id,swapped,2,2,out));
    assert(fabs(out[0]-.5)<1e-12 && fabs(out[1]-.5)<1e-12);
    assert(image_text_mrr(tied,tied,2,2,out));
    assert(fabs(out[0]-.75)<1e-12 && fabs(out[1]-.75)<1e-12);
    assert(image_text_mrr(scaled,id,2,2,out));
    assert(fabs(out[0]-1)<1e-12 && fabs(out[1]-1)<1e-12);
    out[0]=99; out[1]=99;
    assert(!image_text_mrr(id,id,1,2,out));
    assert(!image_text_mrr(id,id,65,2,out));
    assert(!image_text_mrr(id,id,2,0,out));
    assert(!image_text_mrr(id,id,2,129,out));
    assert(!image_text_mrr(zero,id,2,2,out));
    assert(!image_text_mrr(bad,id,2,2,out));
    assert(!image_text_mrr(big,id,2,2,out));
    assert(!image_text_mrr(NULL,id,2,2,out));
    assert(!image_text_mrr(id,id,2,2,NULL));
    assert(out[0]==99 && out[1]==99);
    puts("C image-text MRR tests passed");
    return 0;
}
