#include "triplet_margin_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void near(double actual, double expected) {
    assert(fabs(actual - expected) < 1e-9);
}
int main(void) {
    const double a[] = {0,0}, p[] = {1,0}, n[] = {3,0};
    const double far[] = {3,4}, same[] = {0,0};
    const double bad[] = {1, NAN}, huge[] = {1000001, 0};
    const double neg[] = {-2, -2};
    double out = 99;
    assert(triplet_margin_loss(a,p,n,2,1,&out)); near(out,0);
    assert(triplet_margin_loss(a,p,n,2,3,&out)); near(out,1);
    assert(triplet_margin_loss(a,far,same,2,1,&out)); near(out,6);
    assert(triplet_margin_loss(a,a,a,2,0.5,&out)); near(out,0.5);
    assert(triplet_margin_loss(neg,neg,neg,2,0,&out)); near(out,0);
    double zeros[512] = {0}, ones[512];
    for (size_t i = 0; i < 512; ++i) ones[i] = 1;
    assert(triplet_margin_loss(zeros,zeros,ones,512,0,&out)); near(out,0);
    assert(triplet_margin_loss(zeros,ones,zeros,512,0,&out));
    near(out,sqrt(512.0));
    out = 99;
    assert(!triplet_margin_loss(NULL,p,n,2,1,&out));
    assert(!triplet_margin_loss(a,NULL,n,2,1,&out));
    assert(!triplet_margin_loss(a,p,NULL,2,1,&out));
    assert(!triplet_margin_loss(a,p,n,0,1,&out));
    assert(!triplet_margin_loss(a,p,n,513,1,&out));
    assert(!triplet_margin_loss(a,p,n,2,-1,&out));
    assert(!triplet_margin_loss(a,p,n,2,1001,&out));
    assert(!triplet_margin_loss(a,p,n,2,INFINITY,&out));
    assert(!triplet_margin_loss(a,bad,n,2,1,&out));
    assert(!triplet_margin_loss(a,huge,n,2,1,&out));
    assert(!triplet_margin_loss(a,p,n,2,1,NULL));
    assert(out == 99);
    puts("C triplet margin loss tests passed");
    return 0;
}
