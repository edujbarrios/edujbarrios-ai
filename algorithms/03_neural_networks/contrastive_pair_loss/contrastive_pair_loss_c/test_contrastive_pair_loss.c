#include "contrastive_pair_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void near(double actual, double expected) {
    assert(fabs(actual - expected) < 1e-10);
}
int main(void) {
    const double zero[] = {0,0}, five[] = {3,4}, equal[] = {1,2};
    const double bad[] = {NAN,0}, huge[] = {1001,0};
    double result = 99;
    assert(contrastive_pair_loss(zero,five,2,1,6,&result)); near(result,12.5);
    assert(contrastive_pair_loss(zero,five,2,0,6,&result)); near(result,0.5);
    assert(contrastive_pair_loss(zero,five,2,0,3,&result)); near(result,0);
    assert(contrastive_pair_loss(equal,equal,2,0,2,&result)); near(result,2);
    assert(contrastive_pair_loss(equal,equal,2,1,2,&result)); near(result,0);
    assert(contrastive_pair_loss(zero,five,2,0,0,&result)); near(result,0);
    double a[512]={0}, b[512];
    for (size_t i = 0; i < 512; ++i) b[i]=1;
    assert(contrastive_pair_loss(a,b,512,1,0,&result)); near(result,256);
    assert(contrastive_pair_loss(a,b,512,0,1,&result)); near(result,0);
    result=99;
    assert(!contrastive_pair_loss(NULL,five,2,1,6,&result));
    assert(!contrastive_pair_loss(zero,NULL,2,1,6,&result));
    assert(!contrastive_pair_loss(zero,five,0,1,6,&result));
    assert(!contrastive_pair_loss(zero,five,513,1,6,&result));
    assert(!contrastive_pair_loss(zero,five,2,2,6,&result));
    assert(!contrastive_pair_loss(zero,five,2,1,-1,&result));
    assert(!contrastive_pair_loss(zero,five,2,1,1001,&result));
    assert(!contrastive_pair_loss(zero,five,2,1,INFINITY,&result));
    assert(!contrastive_pair_loss(bad,five,2,1,6,&result));
    assert(!contrastive_pair_loss(zero,huge,2,1,6,&result));
    assert(!contrastive_pair_loss(zero,five,2,1,6,NULL));
    assert(result == 99);
    puts("C contrastive pair loss tests passed");
    return 0;
}
