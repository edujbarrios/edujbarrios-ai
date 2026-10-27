#include "clip_contrastive_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>
int main(void) {
    const double images[]={1,0,0,1};
    const double swapped[]={0,1,1,0};
    const double same[]={1,0,1,0};
    const double zero[]={0,0,0,1};
    const double bad[]={1,0,NAN,1};
    const double outside[]={1,0,1001,1};
    double loss=99.0;
    assert(clip_contrastive_loss(images,images,2,2,1,&loss));
    assert(fabs(loss-log1p(exp(-1)))<1e-12);
    assert(clip_contrastive_loss(images,swapped,2,2,1,&loss));
    assert(fabs(loss-log1p(exp(1)))<1e-12);
    assert(clip_contrastive_loss(same,same,2,2,1,&loss));
    assert(fabs(loss-log(2))<1e-12);
    assert(clip_contrastive_loss(images,images,2,2,100,&loss));
    assert(loss>=0.0 && loss<1e-12);
    loss=99.0;
    assert(!clip_contrastive_loss(images,images,1,2,1,&loss));
    assert(!clip_contrastive_loss(images,images,2,0,1,&loss));
    assert(!clip_contrastive_loss(images,images,2,129,1,&loss));
    assert(!clip_contrastive_loss(images,images,2,2,0,&loss));
    assert(!clip_contrastive_loss(images,images,2,2,101,&loss));
    assert(!clip_contrastive_loss(images,images,2,2,NAN,&loss));
    assert(!clip_contrastive_loss(images,zero,2,2,1,&loss));
    assert(!clip_contrastive_loss(images,bad,2,2,1,&loss));
    assert(!clip_contrastive_loss(images,outside,2,2,1,&loss));
    assert(!clip_contrastive_loss(NULL,images,2,2,1,&loss));
    assert(!clip_contrastive_loss(images,images,2,2,1,NULL));
    assert(loss==99.0);
    puts("C CLIP contrastive loss tests passed");
    return 0;
}
