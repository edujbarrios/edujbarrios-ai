#include "image_text_margin_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double aligned[]={1,0,0,1}, swapped[]={0,1,1,0};
    const double duplicates[]={1,0,1,0}, scaled[]={2,0,0,3};
    const double zeros[]={0,0,0,1}, nonfinite[]={1,0,NAN,1};
    const double large[]={1,0,1001,1};
    double out=99;
    assert(image_text_margin_loss(aligned,aligned,2,2,0.25,&out));
    assert(fabs(out-0.0)<1e-12);
    assert(image_text_margin_loss(aligned,swapped,2,2,0.25,&out));
    assert(fabs(out-1.25)<1e-12);
    assert(image_text_margin_loss(duplicates,duplicates,2,2,0.25,&out));
    assert(fabs(out-0.25)<1e-12);
    assert(image_text_margin_loss(scaled,aligned,2,2,0.25,&out));
    assert(fabs(out)<1e-12);
    assert(image_text_margin_loss(aligned,aligned,2,2,2,&out));
    assert(fabs(out-1.0)<1e-12);
    out=99;
    assert(!image_text_margin_loss(aligned,aligned,1,2,0.25,&out));
    assert(!image_text_margin_loss(aligned,aligned,2,0,0.25,&out));
    assert(!image_text_margin_loss(aligned,aligned,2,129,0.25,&out));
    assert(!image_text_margin_loss(aligned,aligned,2,2,-1,&out));
    assert(!image_text_margin_loss(aligned,aligned,2,2,2.1,&out));
    assert(!image_text_margin_loss(aligned,aligned,2,2,NAN,&out));
    assert(!image_text_margin_loss(zeros,aligned,2,2,0.25,&out));
    assert(!image_text_margin_loss(nonfinite,aligned,2,2,0.25,&out));
    assert(!image_text_margin_loss(large,aligned,2,2,0.25,&out));
    assert(!image_text_margin_loss(NULL,aligned,2,2,0.25,&out));
    assert(!image_text_margin_loss(aligned,aligned,2,2,0.25,NULL));
    assert(out==99);
    puts("C image-text margin loss tests passed");
    return 0;
}
