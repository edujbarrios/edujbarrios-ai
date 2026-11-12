#include "soft_dice_loss.h"
#include <assert.h>
#include <math.h>
#include <stdio.h>

static void near(double actual, double expected) {
    assert(fabs(actual - expected) < 1e-12);
}
int main(void) {
    const double zero[] = {0,0}, perfect[] = {1,0};
    const double wrong[] = {0,1}, soft[] = {0.5,0.5}, two[] = {1,1};
    const uint8_t empty[] = {0,0}, mask[] = {1,0};
    const double bad[] = {NAN,0}, out_bounds[] = {1.01,0};
    const uint8_t bad_targets[] = {1,2};
    double result=99;
    assert(soft_dice_loss(zero,empty,2,&result)); near(result,0);
    assert(soft_dice_loss(perfect,mask,2,&result)); near(result,0);
    assert(soft_dice_loss(wrong,mask,2,&result)); near(result,2.0/3.0);
    assert(soft_dice_loss(soft,mask,2,&result)); near(result,1.0/3.0);
    assert(soft_dice_loss(two,empty,2,&result)); near(result,2.0/3.0);
    assert(soft_dice_loss(zero,mask,2,&result)); near(result,0.5);
    assert(soft_dice_loss(perfect,mask,1,&result)); near(result,0);
    double many[512]={0};
    uint8_t labels[512]={0};
    assert(soft_dice_loss(many,labels,512,&result)); near(result,0);
    for (size_t i=0;i<512;++i) {many[i]=1;labels[i]=1;}
    assert(soft_dice_loss(many,labels,512,&result)); near(result,0);
    result=99;
    assert(!soft_dice_loss(NULL,mask,2,&result));
    assert(!soft_dice_loss(perfect,NULL,2,&result));
    assert(!soft_dice_loss(perfect,mask,0,&result));
    assert(!soft_dice_loss(perfect,mask,513,&result));
    assert(!soft_dice_loss(bad,mask,2,&result));
    assert(!soft_dice_loss(out_bounds,mask,2,&result));
    assert(!soft_dice_loss(perfect,bad_targets,2,&result));
    assert(!soft_dice_loss(perfect,mask,2,NULL));
    assert(result==99);
    puts("C soft Dice loss tests passed");
    return 0;
}
